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

#include "VirtualSensor.h"
#include "common.h"

VirtualSensor::VirtualSensor(void)
{
	num_rays=0;
	width=0;
	height=0;
	rays=0;
	rays_orig=0;
	distances=0;
	range=180;
	max_noise_in_m=0.012;
	max_noise_in_percent_0_to_1=0;
	intensity=0;

	//identity matrix
	for(int i=0;i<16;++i)
	{
		frame[i]=0;
	}
	frame[0]=frame[5]=frame[10]=frame[15]=1;
}


VirtualSensor::~VirtualSensor(void)
{
}

void VirtualSensor::readRays(const char* filename)
{
	FILE* in=fopen(filename, "rb");
	if(!in)
	{
		printf("ERROR: Failed to open file '%s'\n", filename);
		exit(2);
	}
	delete[] rays;
	readData(in, &num_rays, (void**)&rays_orig, 6 * sizeof(float), filename);
	rays=new float[6*num_rays];

	delete[] intensity;
	intensity=new float[3*num_rays];

	UpdateRays();
}

//Only for debugging purpose
void VirtualSensor::SaveDepthImage(const char* filename)
{
	FILE* outfile=fopen(filename, "wb");
	float max_val=0.0f;
	unsigned int i,j;
	int num_hits=0;
	unsigned int written=0u;
	unsigned int max=width * height;
	if(!outfile)
	{
		printf("Could not open file '%s' for writing. Skipping write.\n", filename);
		exit(0);
	}
	for(i=0;i<num_rays;++i)
	{
		num_hits++;
		max_val=distances[i]>max_val ? distances[i] : max_val;

	}

	fprintf(outfile, "P2\n%u %u\n255\n", width, height);
	for(j=height;j>0 && written<max;j--)
	{
		for (i=0;i<width && written<max;i++)
		{
			unsigned int ind=(j-1) * width + i;
			if (ind >= num_rays)
				continue;
			fprintf(outfile, "%i\n", (int)(distances[ind] / max_val * 255.0f));
			written++;
		}
	}
}
void VirtualSensor::SavePointCloudASCII(const char* filename)
{
	FILE* outfile=fopen(filename, "wt");
	if(!outfile)
	{
		printf("Could not open file '%s' for writing. Skipping write.\n", filename);
		return;
	}
	for(unsigned int i=0;i<num_rays;++i)
	{
		if(distances[i]>0)
		{
			float x=rays[6*i+3]*distances[i];
			float y=rays[6*i+4]*distances[i];
			float z=rays[6*i+5]*distances[i];

			fprintf(outfile,"%f %f %f\n",x,y,z);
		}
	}
	fclose(outfile);
}
void VirtualSensor::GetPointCloudHost(float *data)
{
	if(!data)
	{
		printf("Allocate data buffer! (VirtualSensor::GetPointCloudHost)\n");
		return;
	}

	for(unsigned int i=0;i<num_rays;++i)
	{
		if(distances[i]>0)
		{
			data[3*i+0]=rays[6*i+3]*distances[i];
			data[3*i+1]=rays[6*i+4]*distances[i];
			data[3*i+2]=rays[6*i+5]*distances[i];
		}
	}
}

void VirtualSensor::GetPointCloud(float *data)
{
	if(!data)
		return;

	GetPointCloudHost(data);
}
void VirtualSensor::SetPosition(float x, float y,float z)
{
	frame[12]=x;
	frame[13]=y;
	frame[14]=z;

	UpdateRays();
}
void VirtualSensor::Translate(float x, float y,float z)
{
	frame[12]+=x;
	frame[13]+=y;
	frame[14]+=z;

	UpdateRays();
}
void VirtualSensor::SetMatrix(float m[16])
{
	memcpy(frame,m,16*sizeof(float));

	UpdateRays();
}
void VirtualSensor::UpdateRays()
{
	for(int i=0;i<num_rays;++i)
	{
		rays[6*i+0]=rays_orig[6*i+0]+frame[12];
		rays[6*i+1]=rays_orig[6*i+1]+frame[13];
		rays[6*i+2]=rays_orig[6*i+2]+frame[14];
		rays[6*i+3]=rays_orig[6*i+3];
		rays[6*i+4]=rays_orig[6*i+4];
		rays[6*i+5]=rays_orig[6*i+5];
	}
}

void VirtualSensor::SetNoise(double n)
{
	max_noise_in_m=n;
}

