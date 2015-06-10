#ifndef _VIRTUAL_SENSOR_H_
#define _VIRTUAL_SENSOR_H_

#include "config.h"

class DLLIMPORT VirtualSensor
{
public:
	float *rays;
	float *intensity;
	unsigned int num_rays;
	unsigned int width;
	unsigned int height;

	float* distances;
	float range;
	
	float max_noise_in_m;
	float max_noise_in_percent_0_to_1;

	VirtualSensor();

	~VirtualSensor();


	//Only for debugging purpose
	void SaveDepthImage(const char* filename);
	
	void SavePointCloudASCII(const char* filename);
	void GetPointCloud(float *data);
	void SetPosition(float x, float y,float z);
	void Translate(float x, float y,float z);
	void SetMatrix(float m[16]);
	void SetNoise(double n);

	float frame[16];
protected:
	float* rays_orig;
	/*
	* nrays (4 byte int)
	* rays ({ float3 origin, float3 direction } * nrays)
	*/
	void readRays(const char* filename);
	void GetPointCloudHost(float *data);
	void UpdateRays();
};

#endif