#include "sensorsimrt.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <ctime>

#if defined(_WIN32)
# define WIN32_LEAN_AND_MEAN
# include<windows.h>
# include<mmsystem.h>
#else
# include<sys/time.h>
# include <unistd.h>
# include <dirent.h>
#endif

#include <numeric>      // std::inner_product


SensorSimRT::SensorSimRT(int cpu_threads)
{
	scene=0;
	sensor=0;
	this->cpu_threads=cpu_threads;
	t=0;
	results=0;
	normals=0;
	barycentric=0;
	context=0;
	rays_mapped_ptr=0;
	results_mapped_ptr=0;
	normals_mapped_ptr=0;
	barycentric_mapped_ptr=0;

	RTresult res=rtuTraversalCreate(&t, RTU_QUERY_TYPE_CLOSEST_HIT,
		RTU_RAYFORMAT_ORIGIN_DIRECTION_INTERLEAVED,
		RTU_TRIFORMAT_MESH,
		RTU_OUTPUT_NORMAL | RTU_OUTPUT_BARYCENTRIC,
		cpu_threads ? RTU_INITOPTION_CPU_ONLY : RTU_INITOPTION_NONE,
		context);

	if(res!=RT_SUCCESS)
	{
		printf("Could not create Optix context. Error %d\n",res);
		exit(1);
	}

	if(cpu_threads)
	{
		printf("CPU simulation using %d threads.\n", cpu_threads);
		OPTIX_ERRROR_CHECK(rtuTraversalSetOption(t, RTU_OPTION_INT_NUM_THREADS, (void*)(&cpu_threads)));
	}
	else
	{
		printf("GPU simulation\n");
	}


}

void SensorSimRT::InitSim()
{
	if(!sensor)
	{
		printf("NO SENSOR! - SensorSimRT::InitSim\n");
		throw;
	}
	
	
	OPTIX_ERRROR_CHECK(rtuTraversalMapRays(t, sensor->num_rays, &rays_mapped_ptr));
	
	memcpy(rays_mapped_ptr, sensor->rays, sensor->num_rays*sizeof(float3)*2);
	
	OPTIX_ERRROR_CHECK(rtuTraversalUnmapRays(t));
	

	delete[] results;
	results=new RTUtraversalresult [sensor->num_rays];
	delete[] normals;
	normals = new float3[sensor->num_rays];
	delete[] barycentric;
	barycentric = new float2[sensor->num_rays];

	UpdateOptixScene();
	
}
void SensorSimRT::Simulate()
{
	
	if(!sensor)
	{
		printf("NO SENSOR! - SensorSimRT::Simulate\n");
		throw;
	}
	size_t ray_buf_size=sensor->num_rays*sizeof(float3)*2;
	//printf("ray_buf_size=%d\n",ray_buf_size);


	
	OPTIX_ERRROR_CHECK(rtuTraversalMapRays(t, sensor->num_rays, &rays_mapped_ptr));
	
	memcpy(rays_mapped_ptr, sensor->rays, ray_buf_size);
	
	OPTIX_ERRROR_CHECK(rtuTraversalUnmapRays(t));
	

	OPTIX_ERRROR_CHECK(rtuTraversalTraverse(t));
	

	OPTIX_ERRROR_CHECK(rtuTraversalMapResults(t, &results_mapped_ptr));
	memcpy(results, results_mapped_ptr, sensor->num_rays * sizeof(RTUtraversalresult));
	OPTIX_ERRROR_CHECK(rtuTraversalUnmapResults(t));
	

	OPTIX_ERRROR_CHECK( rtuTraversalMapOutput( t, RTU_OUTPUT_NORMAL, &normals_mapped_ptr ) );
    memcpy( normals, normals_mapped_ptr, sensor->num_rays * sizeof(float3) );
    OPTIX_ERRROR_CHECK( rtuTraversalUnmapOutput( t, RTU_OUTPUT_NORMAL ) );

	OPTIX_ERRROR_CHECK( rtuTraversalMapOutput( t, RTU_OUTPUT_BARYCENTRIC, &barycentric_mapped_ptr ) );
	memcpy( barycentric, barycentric_mapped_ptr, sensor->num_rays * sizeof(float2) );
    OPTIX_ERRROR_CHECK( rtuTraversalUnmapOutput( t, RTU_OUTPUT_BARYCENTRIC ) );

	
	GetResults(results,sensor->distances,sensor->num_rays,sensor->range);
	GetIntensity(sensor->rays, normals, sensor->intensity,sensor->num_rays);
	
	ApplyNoise(sensor->distances,sensor->num_rays,sensor->max_noise_in_m,sensor->max_noise_in_percent_0_to_1);
}
void SensorSimRT::GetResults(RTUtraversalresult* results,float *distances, unsigned int num_rays, float maxdist)
{
	for(unsigned int i=0;i<num_rays;++i)
	{
		if(results[i].prim_id != -1 && results[i].t<=maxdist)
		{
			distances[i]=results[i].t;
		}
		else
		{
			distances[i]=0;
			results[i].prim_id=-1;
			results[i].t=0;
		}

	}
}
void SensorSimRT::GetIntensity(float *rays, float3* normals,float *intensity, unsigned int num_rays)
{
	for(unsigned int i=0;i<num_rays;++i)
	{
		float v0[]={rays[6*i+3],rays[6*i+4],rays[6*i+5]};
		float v1[]={normals[i].x,normals[i].y,normals[i].z};

		float prod=v0[0]*v1[0]+v0[1]*v1[1]+v0[2]*v1[2];
		//printf("prod = %f\n",prod);
		intensity[i]=fabs(prod);
	}
}

void SensorSimRT::ApplyNoise(float *distances, unsigned int num_rays, float max_noise_in_m, float max_noise_in_percent_0_to_1)
{
	for(unsigned int i=0;i<num_rays;++i)
	{
		if(distances[i])
			distances[i]+=max_noise_in_m*(-500+rand()%1000)/1000.0f+max_noise_in_percent_0_to_1*distances[i]*(-500+rand()%1000)/1000.0f;
	}
}

void SensorSimRT::LoadNewSceneObj(const char* obj)
{
	if(!scene)
	{
		scene=new VirtualScene;
	}
	scene->LoadObj(obj);
	
	UpdateOptixScene();
}

void SensorSimRT::SetSceneScale(double scale)
{
	scene->SetScale(scale);
}

void SensorSimRT::SetSceneUpAxisToZ(bool z_up)
{
	scene->SetUpAxisToZ(z_up);
}
void SensorSimRT::UpdateOptixScene()
{
	OPTIX_ERRROR_CHECK(rtuTraversalSetMesh(t, scene->num_verts, scene->verts, scene->num_tris, scene->indices));
	printf("Preprocessing...\t");
	fflush(stdout);
	OPTIX_ERRROR_CHECK(rtuTraversalPreprocess(t));
	printf("[DONE]\n");
}
void SensorSimRT::SetSensor(VirtualSensor *sensor)
{
	delete this->sensor;
	this->sensor=sensor;
	
	
	OPTIX_ERRROR_CHECK(rtuTraversalMapRays(t, sensor->num_rays, &rays_mapped_ptr));
	
	memcpy(rays_mapped_ptr, sensor->rays, sensor->num_rays*sizeof(float3)*2);
	
	OPTIX_ERRROR_CHECK(rtuTraversalUnmapRays(t));
	
	
	delete[] results;
	results=new RTUtraversalresult [sensor->num_rays];
	delete[] normals;
	normals = new float3[sensor->num_rays];
	delete[] barycentric;
	barycentric = new float2[sensor->num_rays];
}
void SensorSimRT::GetPointCloud(unsigned int &size, float *data)
{
	if(!sensor)
	{
		printf("NO SENSOR! - SensorSimRT::GetPointCloud\n");
		throw;
	}
	if(!data)
	{
		size=0;
		fprintf(stderr,"ERROR in SensorSimRT::GetPointCloud - data is null\n");
		return;
	}

	if(sensor)
	{
		size=sensor->num_rays;
		sensor->GetPointCloud(data);
	}

}
