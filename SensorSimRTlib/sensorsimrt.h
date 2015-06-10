#include <cuda_runtime_api.h>
#include <optix_world.h>


#include "config.h"
#include "VirtualSensor.h"
#include "VirtualScene.h"

#define OPTIX_ERRROR_CHECK(f)	{RTresult res=f;if(res!=RT_SUCCESS){const char* err;rtuTraversalGetErrorString(t, res, &err);printf("Optix error %d: %s\n",res,err);system("pause");exit(2);}}



class DLLIMPORT SensorSimRT
{public:

	VirtualScene *scene;

	RTUtraversal t;
	RTUtraversalresult* results;
	float3 *normals;
	float2 *barycentric;
	RTcontext context;

	float* rays_mapped_ptr;
	RTUtraversalresult* results_mapped_ptr;
	void *normals_mapped_ptr;
	void *barycentric_mapped_ptr;

	int cpu_threads;

	SensorSimRT(int cpu_threads=0);

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
	void GetIntensity(float *rays, float3* normals,float *intensity, unsigned int num_rays);
	void ApplyNoise(float *distances, unsigned int num_rays, float max_noise_in_m, float max_noise_in_percent_0_to_1);
	VirtualSensor *sensor;
};
