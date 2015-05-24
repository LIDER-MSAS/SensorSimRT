#ifndef _VIRTUALSCENE_H_
#define _VIRTUALSCENE_H_

#include "config.h"

class DLLIMPORT VirtualScene
{
public:
	unsigned int num_tris, num_verts;
	float* verts;
	float* verts_orig;
	unsigned int* indices;

	VirtualScene();

	~VirtualScene();

	/*
	* nverts (4 bypte int)
	* verts (3 * float3 vertex * nverts)
	* ntris (4 byte int)
	* indices (3 * int3 vindex * ntris)
	*/
	void LoadMesh(const char* filename);
	void LoadObj(const char* filename);
	void SetScale(double scale);
	void SetUpAxisToZ(bool z_up);
private:
	void UpdateModel();
	double scale;
	bool z_up;
};


#endif