#include "M3DLMS100.h"
#include "common.h"

M3DLMS100::M3DLMS100(void)
{
	width=541;
	height=250;
	num_rays=width*height;

	GenerateRays();
	
	range=18;
	max_noise_in_m=0.012f;//LMS100 statistical error
	max_noise_in_percent_0_to_1=0;
	
	UpdateRays();
}
M3DLMS100::~M3DLMS100(void)
{
	delete[] rays;
	delete[] rays_orig;
	delete[] distances;
}
void M3DLMS100::GenerateRays()
{
	delete[] rays_orig;
	delete[] rays;
	delete[] distances;
	
	width=541;
	height=250;
	num_rays=width*height;

	
	
	distances=new float[num_rays];
	rays=new float[6*num_rays];
	rays_orig=new float[6*num_rays];
	


	double angle_x_range=270;
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
