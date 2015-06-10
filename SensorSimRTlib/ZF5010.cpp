#include "ZF5010.h"
#include "common.h"

ZF5010::ZF5010(void)
{
	GenerateRays();
	
	range=187.3f;
	max_noise_in_m=0.0001f;
	max_noise_in_percent_0_to_1=0.0001f;
	
	UpdateRays();
}
ZF5010::~ZF5010(void)
{
	delete[] rays;
	delete[] rays_orig;
	delete[] distances;
}
void ZF5010::GenerateRays()
{
	delete[] rays_orig;
	delete[] rays;
	delete[] distances;
	delete[] intensity;
	
	width=5000;
	height=5000;
	num_rays=width*height;

	distances=new float[num_rays];
	rays=new float[6*num_rays];
	rays_orig=new float[6*num_rays];
	intensity=new float[num_rays];

	double angle_x_range=320;
	double angle_z_range=180;

	double dx=angle_x_range/width;
	double dy=angle_z_range/height;
	

	for(unsigned int y=0;y<height;++y)
	{
		for(unsigned int x=0;x<width;++x)
		{
			double anglex=(-angle_x_range/2+dx*x);
			double angley=(-angle_z_range/2+dy*y);

			double m[16],m2[16];

			double v[3]={1,0,0},v1[3];

			commonMakeMatrix(0,0,0,angley,anglex,0,m);
			commonMakeMatrix(0,0,0,0,-90,0,m2);

			commonTransformPoint(v,m,v1);
			commonTransformPoint(v1,m2,v);

			int index=6*(x+y*width);

			rays_orig[index+0]=0;
			rays_orig[index+1]=0;
			rays_orig[index+2]=0;

			rays_orig[index+3]=float(v[0]);
			rays_orig[index+4]=float(v[1]);
			rays_orig[index+5]=float(v[2]);
			
		}
	}
	
}
