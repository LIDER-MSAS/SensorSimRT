#include <cuda_runtime_api.h>
#include <optix_world.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>

#if defined(_WIN32)
# define WIN32_LEAN_AND_MEAN
# include<windows.h>
# include<mmsystem.h>
#else
# include<sys/time.h>
# include <unistd.h>
# include <dirent.h>
#endif

#define OPTIX_ERRROR_CHECK(f)	{RTresult res=f;if(res!=RT_SUCCESS){const char* err;rtuTraversalGetErrorString(t, res, &err);printf("Optix error %d: %s\n",res,err);exit(2);}}

double inv_f;
bool freqInitialized=false;
BOOL useHighResTimer=0;


void printUsage(const char* argv0)
{
	printf("Usage : %s [options]\n", argv0);
	printf(" --help | -h		Print this message\n");
	printf(" --cpu NUM			Use cpu raytracer, specifying number of threads\n");
	printf(" --warmup NUM		Specify number of warmup iterations\n");
	printf(" --iter NUM			Specify number of timed iterations\n");
	printf(" --rays <FILENAME>	Filename specifying the rays to trace\n");
	printf(" --mesh <FILENAME>	Filename specifying the mesh to intersect the rays against\n");
	printf(" --image <FILENAME>	Filename of the output depth image (.pgm)\n");
	printf(" --width NUM		Set image width.\n");
	printf(" --height NUM		Set image height.\n");
}

/* Reads in a 4 byte int for the number of elements then reads that much elem_size byte
* sized data from the same file */
void readData(FILE* in, unsigned int* num_elems, void** data, size_t elem_size,const char* filename)
{
	size_t bytes_read, buffer_size;

	bytes_read=fread((void*)(num_elems), sizeof(char), sizeof(unsigned int), in);
	if (bytes_read != sizeof(unsigned int))
	{
		printf("ERROR: reading number from %s\n",filename);
		exit(2);
	}
	printf("Reading in %u  ... \n", *num_elems);

	buffer_size=*num_elems * elem_size;

	if (*data == 0)
		*data=(float*)malloc(buffer_size);
	if (*data == 0)
	{
		printf("ERROR: allocating buffer of size %llu  when reading %s\n",
			(unsigned long long)buffer_size,filename);
		exit(2);
	}
	bytes_read=fread(*data, sizeof(char), buffer_size, in);
	if (bytes_read != buffer_size)
	{
		printf("ERROR: when attempting to read %llu bytes from %s, read only %llu bytes.\n",
			(unsigned long long)buffer_size,filename,
			(unsigned long long)bytes_read);
		exit(2);
	}
}

/*
* nrays (4 byte int)
* rays ({ float3 origin, float3 direction } * nrays)
*/
void readRays(const char* filename, unsigned int* nrays, float** rays)
{
	FILE* in=fopen(filename, "rb");
	if(!in) {
		printf("ERROR: Failed to open file '%s'\n", filename);
		exit(2);
	}

	readData(in, nrays, (void**)rays, 6 * sizeof(float), filename);
}


/*
* nverts (4 bypte int)
* verts (3 * float3 vertex * nverts)
* ntris (4 byte int)
* indices (3 * int3 vindex * ntris)
*/
void LoadMesh(const char* filename,
			  unsigned int* nverts, float** verts,
			  unsigned int* ntris, unsigned int** indices)
{
	FILE* in=fopen(filename, "rb");
	if(!in) {
		printf("ERROR: Failed to open file '%s'\n", filename);
		exit(2);
	}

	/* read in the vertices */
	readData(in, nverts, (void**)verts, 3 * sizeof(float), filename);

	/* read in the index data */
	readData(in, ntris, (void**)indices, 3 * sizeof(unsigned int), filename);
}

//Only for debugging purpose
void SaveDepthImage(RTUtraversalresult* results, unsigned int num_rays,
				 const char* filename,
				 unsigned int width, unsigned int height)
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
		if(results[i].prim_id != -1)
		{
			num_hits++;
			max_val=results[i].t>max_val ? results[i].t : max_val;
		}
	}

	fprintf(outfile, "P2\n%u %u\n255\n", width, height);
	for(j=height;j>0 && written<max;j--)
	{
		for (i=0;i<width && written<max;i++)
		{
			unsigned int ind=(j-1) * width + i;
			if (ind >= num_rays)
				continue;
			if(results[ind].prim_id != -1)
				fprintf(outfile, "%i\n", (int)(results[ind].t / max_val * 255.0f));
			else
				fprintf(outfile, "%i\n", 0);
			written++;
		}
	}
}

int main(int argc, char** argv)
{
	unsigned int num_warmup=50, num_timed=1000;
	unsigned int cpu_threads=0;
	char* rays_filename=0;
	char* mesh_filename=0;
	char* image_filename=0;
	unsigned int width=0, height=0;

	for(unsigned int i=1;i<(unsigned int)argc;++i)
	{
		const char* arg=argv[i];
		if(strcmp(arg, "--cpu") == 0)
		{
			cpu_threads=atoi(argv[++i]);
		}
		else if(strcmp(arg, "--rays") == 0)
		{
			rays_filename=argv[++i];
		}
		else if(strcmp(arg, "--mesh") == 0)
		{
			mesh_filename=argv[++i];
		}
		else if(strcmp(arg, "--image") == 0)
		{
			image_filename=argv[++i];
		}
		else if(strcmp(arg, "--width") == 0)
		{
			width=atoi(argv[++i]);
		}
		else if(strcmp(arg, "--height") == 0)
		{
			height=atoi(argv[++i]);
		}
		else if(strcmp(arg, "--warmup") == 0)
		{
			num_warmup=atoi(argv[++i]);
		}
		else if(strcmp(arg, "--iter") == 0)
		{
			num_timed=atoi(argv[++i]);
		}
		else if((strcmp(arg, "--help") == 0) || (strcmp(arg, "-h") == 0))
		{
			printUsage(argv[0]);
			exit(0);
		}
		else
		{
			printf("Unknown option '%s'\n", arg);
		}
	}

	if (!mesh_filename || !rays_filename || !width || !height)
	{
		printUsage(argv[0]);
		exit(-1);
	}

	printf("Parameters:\n");
	printf("Scene:\t\t\t%s\n",mesh_filename);
	printf("Rays:\t\t\t%s\n",rays_filename);
	printf("Width:\t\t\t%u\n",width);
	printf("Height:\t\t\t%u\n",height);
	printf("Warmup iterations:\t%u\n",num_warmup);
	printf("Iterations:\t\t%u\n",num_timed);

	RTUtraversal t=0;
	RTUtraversalresult* results=0;
	RTcontext context=0;

	float* rays_mapped_ptr=0;
	RTUtraversalresult* results_mapped_ptr=0;
	
	unsigned int num_rays, num_tris, num_verts;
	float* rays=0;
	float* verts=0;
	float* tris=0;
	unsigned int* indices=0;


	clock_t begin_time=clock();

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

	printf("Loading: %s\n",mesh_filename);
	LoadMesh(mesh_filename, &num_verts, &verts, &num_tris, &indices);

	printf("Number of verticies: %u\n",num_verts);
	printf("Number of triangles: %u\n",num_tris);

	readRays(rays_filename, &num_rays, &rays);
	OPTIX_ERRROR_CHECK(rtuTraversalMapRays(t, num_rays, &rays_mapped_ptr));
	memcpy(rays_mapped_ptr, rays, num_rays*sizeof(float3)*2);
	OPTIX_ERRROR_CHECK(rtuTraversalUnmapRays(t));

	OPTIX_ERRROR_CHECK(rtuTraversalSetMesh(t, num_verts, verts, num_tris, indices));

	results=(RTUtraversalresult*)malloc(num_rays*sizeof(RTUtraversalresult));

	double dt=double(clock () - begin_time) / CLOCKS_PER_SEC;
	printf("Setup time: %.3lf sec.\n",dt);

	printf("Preprocessing ... ");
	fflush(stdout);
	begin_time=clock();
	OPTIX_ERRROR_CHECK(rtuTraversalPreprocess(t));
	dt=double(clock()-begin_time)/CLOCKS_PER_SEC;
	printf("%.3lf sec.\n",dt);

	printf("Traversing %u iterations of warmup rays ... ", num_warmup);
	fflush(stdout);
	begin_time=clock();
	for(unsigned int i=0;i<num_warmup;++i)
	{
		OPTIX_ERRROR_CHECK(rtuTraversalTraverse(t));
	}
	dt=double(clock()-begin_time)/CLOCKS_PER_SEC;
	printf("%.3lf sec.\n",dt);

	printf("Traversing timed rays (%u iterations) ... ", num_timed);
	fflush(stdout);
	begin_time=clock();
	for(unsigned int i=0;i<num_timed;++i)
	{
		size_t ray_buf_size=num_rays*sizeof(float3)*2;
		OPTIX_ERRROR_CHECK(rtuTraversalMapRays(t, num_rays, &rays_mapped_ptr));
		memcpy(rays_mapped_ptr, rays, ray_buf_size);
		OPTIX_ERRROR_CHECK(rtuTraversalUnmapRays(t));

		OPTIX_ERRROR_CHECK(rtuTraversalTraverse(t));

		OPTIX_ERRROR_CHECK(rtuTraversalMapResults(t, &results_mapped_ptr));
		memcpy(results, results_mapped_ptr, num_rays * sizeof(RTUtraversalresult));
		OPTIX_ERRROR_CHECK(rtuTraversalUnmapResults(t));

	}
	dt=double(clock()-begin_time)/CLOCKS_PER_SEC;
	printf("%.3lf sec.\n",dt);

	printf("%.3lf ms per iteration\n", (dt) / (float)num_timed * 1000.0);
	printf("%.3lf MRay/sec\n", (float)num_timed*(float)num_rays / (float)(dt) / 1e6);

	OPTIX_ERRROR_CHECK(rtuTraversalMapResults(t, &results_mapped_ptr));

	if(image_filename)
		SaveDepthImage(results_mapped_ptr, num_rays, image_filename, width, height);

	OPTIX_ERRROR_CHECK(rtuTraversalUnmapResults(t));

	OPTIX_ERRROR_CHECK(rtuTraversalDestroy(t));

	free(verts);
	free(indices);
	free(results);
	free(rays);

	return 0;
}
