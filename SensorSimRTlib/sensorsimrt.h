#include <cuda_runtime_api.h>
#include <optix_world.h>


#include "config.h"
#include "VirtualSensor.h"
#include "VirtualScene.h"

#define OPTIX_ERRROR_CHECK(f)	{RTresult res=f;if(res!=RT_SUCCESS){const char* err;rtuTraversalGetErrorString(t, res, &err);printf("Optix error %d: %s\n",res,err);system("pause");exit(2);}}



class DLLIMPORT SensorSimRT
{public:


	RTUtraversal t;
	RTUtraversalresult* results;
	RTcontext context;

	float* rays_mapped_ptr;
	RTUtraversalresult* results_mapped_ptr;

	int cpu_threads;

	SensorSimRT(int cpu_threads=0):cpu_threads(cpu_threads),
		t(0),
		results(0),
		context(0),
		rays_mapped_ptr(0),
		results_mapped_ptr(0)
	{
		scene=0;
		sensor=0;

		RTresult res=rtuTraversalCreate(&t, RTU_QUERY_TYPE_CLOSEST_HIT,
			RTU_RAYFORMAT_ORIGIN_DIRECTION_INTERLEAVED,
			RTU_TRIFORMAT_MESH,
			RTU_OUTPUT_NONE,
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


	void InitSim();
	void Simulate();
	void LoadNewSceneObj(const char* obj);
	void SetSceneScale(double scale);
	void SetSceneUpAxisToZ(bool z_up);
	void UpdateOptixScene();
	void SetSensor(VirtualSensor *sensor);
	void GetPointCloud(unsigned int &size, float *data);
private:
	void GetResults(RTUtraversalresult* results,float *distances, unsigned int num_rays, float maxdist);
	void ApplyNoise(float *distances, unsigned int num_rays, float max_noise_in_m, float max_noise_in_percent_0_to_1);
	VirtualScene *scene;
	VirtualSensor *sensor;
};
