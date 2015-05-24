#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <ctime>
#include <iostream>

#if defined(_WIN32)
# define WIN32_LEAN_AND_MEAN
# include<windows.h>
# include<mmsystem.h>
#else
# include<sys/time.h>
# include <unistd.h>
# include <dirent.h>
#endif

#include "VirtualScene.h"
#include "common.h"
#include "tiny_obj_loader.h"

VirtualScene::VirtualScene()
{
	num_tris=0;
	num_verts=0;
	verts=0;
	indices=0;
	verts_orig=0;
	scale=0.001;
	z_up=1;
}


VirtualScene::~VirtualScene()
{
}

void VirtualScene::LoadMesh(const char* filename)
{
	FILE* in=fopen(filename, "rb");
	if(!in)
	{
		printf("ERROR: Failed to open file '%s'\n", filename);
		exit(2);
	}
	
	free(verts_orig);
	verts_orig=0;
	free(indices);
	indices=0;

	/* read in the vertices */
	readData(in, &num_verts, (void**)&verts_orig, 3 * sizeof(float), filename);

	/* read in the index data */
	readData(in, &num_tris, (void**)&indices, 3 * sizeof(unsigned int), filename);

	delete[] verts;
	verts=new float[3*num_verts];

	printf("Triangles: %d\n",num_tris);

	UpdateModel();
}


void VirtualScene::LoadObj(const char* filename)
{
	if(filename)
	{
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;

		printf("Loading %s...\t",filename);
		std::string err = tinyobj::LoadObj(shapes, materials, filename);
		printf("[DONE]\n");

		if (!err.empty())
		{
			std::cerr << err << std::endl;
			exit(1);
		}

		std::cout << "# of shapes    : " << shapes.size() << std::endl;

		std::vector<float> vertex_data;
		std::vector<unsigned int> indices_data;

		for (size_t i = 0; i < shapes.size(); i++)
		{
			assert((shapes[i].mesh.indices.size() % 3) == 0);
			for (size_t f = 0; f < shapes[i].mesh.indices.size() / 3; f++)
			{
				indices_data.push_back(shapes[i].mesh.indices[3*f+0]+vertex_data.size()/3);
				indices_data.push_back(shapes[i].mesh.indices[3*f+1]+vertex_data.size()/3);
				indices_data.push_back(shapes[i].mesh.indices[3*f+2]+vertex_data.size()/3);

			}

			assert((shapes[i].mesh.positions.size() % 3) == 0);
			for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++)
			{
				float x=shapes[i].mesh.positions[3*v+0];
				float y=shapes[i].mesh.positions[3*v+1];
				float z=shapes[i].mesh.positions[3*v+2];

				vertex_data.push_back(x);
				vertex_data.push_back(y);
				vertex_data.push_back(z);
			}
		}

		delete[] verts;
		delete[] verts_orig;
		delete[] indices;
		
		num_verts=vertex_data.size()/3;
		num_tris=indices_data.size()/3;
		verts=new float[vertex_data.size()];
		verts_orig=new float[vertex_data.size()];
		
		memcpy(verts_orig,vertex_data.data(),4*vertex_data.size());
		
		indices=new unsigned int[indices_data.size()];
		
		memcpy(indices,indices_data.data(),4*indices_data.size());
	}
	printf("Loaded %d triangles\n",num_tris);
	UpdateModel();
}

void VirtualScene::SetScale(double scale)
{
	this->scale=scale;
	UpdateModel();
}

void VirtualScene::UpdateModel()
{
	for(unsigned int i=0;i<num_verts;++i)
	{
		if(z_up)
		{
			verts[3*i+0]=verts_orig[3*i+0]*scale;
			verts[3*i+1]=verts_orig[3*i+1]*scale;
			verts[3*i+2]=verts_orig[3*i+2]*scale;
		}
		else
		{
			verts[3*i+0]=verts_orig[3*i+0]*scale;
			verts[3*i+1]= - verts_orig[3*i+2]*scale;
			verts[3*i+2]=verts_orig[3*i+1]*scale;
		}
	}
}

void VirtualScene::SetUpAxisToZ(bool z_up)
{
	this->z_up=z_up;

	printf("verts %p verts_orig %p\n",verts,verts_orig);

	UpdateModel();
}
