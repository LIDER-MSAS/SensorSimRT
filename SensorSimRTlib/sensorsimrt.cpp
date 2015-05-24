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
	

	free(results);
	results=(RTUtraversalresult*)malloc(sensor->num_rays*sizeof(RTUtraversalresult));
	

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
	
	
	GetResults(results,sensor->distances,sensor->num_rays,sensor->range);
	
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
		}

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
	

	free(results);
	results=(RTUtraversalresult*)malloc(sensor->num_rays*sizeof(RTUtraversalresult));
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
