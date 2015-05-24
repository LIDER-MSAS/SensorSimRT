#include "common.h"

#include <cstdio>
#include <cstring>
#include <cstdlib>


/* Reads in a 4 byte int for the number of elements then reads that much elem_size byte
* sized data from the same file */
void readData(FILE* in, unsigned int* num_elems, void** data, size_t elem_size,const char* filename)
{
	free(*data);
	*data=0;

	size_t bytes_read, buffer_size;

	bytes_read=fread((void*)(num_elems), sizeof(char), sizeof(unsigned int), in);
	if (bytes_read != sizeof(unsigned int))
	{
		printf("ERROR: reading number from %s\n",filename);
		exit(2);
	}

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

void commonLoadIdentityMatrix(double m[16])
{
	static double identity[16] = { 1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f };

	memcpy(m, identity, 16*sizeof(double));
}
void commonTranslationMatrix(double x, double y, double z, double mTranslate[16])
{
	commonLoadIdentityMatrix(mTranslate);
	mTranslate[12] = x;
	mTranslate[13] = y;
	mTranslate[14] = z;
}
void commonMultiplyMatrix(const double m1[16], const double m2[16], double mProduct[16])
{
	mProduct[0]     = m1[0] * m2[0] + m1[4] * m2[1] + m1[8] * m2[2] + m1[12] * m2[3];
	mProduct[4]     = m1[0] * m2[4] + m1[4] * m2[5] + m1[8] * m2[6] + m1[12] * m2[7];
	mProduct[8]     = m1[0] * m2[8] + m1[4] * m2[9] + m1[8] * m2[10] + m1[12] * m2[11];
	mProduct[12]    = m1[0] * m2[12] + m1[4] * m2[13] + m1[8] * m2[14] + m1[12] * m2[15];

	mProduct[1]     = m1[1] * m2[0] + m1[5] * m2[1] + m1[9] * m2[2] + m1[13] * m2[3];
	mProduct[5]     = m1[1] * m2[4] + m1[5] * m2[5] + m1[9] * m2[6] + m1[13] * m2[7];
	mProduct[9]     = m1[1] * m2[8] + m1[5] * m2[9] + m1[9] * m2[10] + m1[13] * m2[11];
	mProduct[13]    = m1[1] * m2[12] + m1[5] * m2[13] + m1[9] * m2[14] + m1[13] * m2[15];

	mProduct[2]     = m1[2] * m2[0] + m1[6] * m2[1] + m1[10] * m2[2] + m1[14] * m2[3];
	mProduct[6]     = m1[2] * m2[4] + m1[6] * m2[5] + m1[10] * m2[6] + m1[14] * m2[7];
	mProduct[10]    = m1[2] * m2[8] + m1[6] * m2[9] + m1[10] * m2[10] + m1[14] * m2[11];
	mProduct[14]    = m1[2] * m2[12] + m1[6] * m2[13] + m1[10] * m2[14] + m1[14] * m2[15];

	mProduct[3]     = m1[3] * m2[0] + m1[7] * m2[1] + m1[11] * m2[2] + m1[15] * m2[3];
	mProduct[7]     = m1[3] * m2[4] + m1[7] * m2[5] + m1[11] * m2[6] + m1[15] * m2[7];
	mProduct[11]    = m1[3] * m2[8] + m1[7] * m2[9] + m1[11] * m2[10] + m1[15] * m2[11];
	mProduct[15]    = m1[3] * m2[12] + m1[7] * m2[13] + m1[11] * m2[14] + m1[15] * m2[15];     
}
void commonRotationMatrix(double angle, double x, double y, double z, double mMatrix[16])
{
	double vecLength, sinSave, cosSave, oneMinusCos;
	double xx, yy, zz, xy, yz, zx, xs, ys, zs;

	// If NULL vector passed in, this will blow up...
	if(x == 0.0f && y == 0.0f && z == 0.0f)
	{
		commonLoadIdentityMatrix(mMatrix);
		return;
	}

	// Scale vector
	vecLength = sqrt( x*x + y*y + z*z );

	// Rotation matrix is normalized
	x /= vecLength;
	y /= vecLength;
	z /= vecLength;

	sinSave = sin(angle);
	cosSave = cos(angle);
	oneMinusCos = 1.0f - cosSave;

	xx = x * x;
	yy = y * y;
	zz = z * z;
	xy = x * y;
	yz = y * z;
	zx = z * x;
	xs = x * sinSave;
	ys = y * sinSave;
	zs = z * sinSave;

	mMatrix[0] = (oneMinusCos * xx) + cosSave;
	mMatrix[4] = (oneMinusCos * xy) - zs;
	mMatrix[8] = (oneMinusCos * zx) + ys;
	mMatrix[12] = 0.0f;

	mMatrix[1] = (oneMinusCos * xy) + zs;
	mMatrix[5] = (oneMinusCos * yy) + cosSave;
	mMatrix[9] = (oneMinusCos * yz) - xs;
	mMatrix[13] = 0.0f;

	mMatrix[2] = (oneMinusCos * zx) - ys;
	mMatrix[6] = (oneMinusCos * yz) + xs;
	mMatrix[10] = (oneMinusCos * zz) + cosSave;
	mMatrix[14] = 0.0f;

	mMatrix[3] = 0.0f;
	mMatrix[7] = 0.0f;
	mMatrix[11] = 0.0f;
	mMatrix[15] = 1.0f;
}
void commonMakeMatrix(double transX, double transY, double transZ,	double RotX,	double RotY,	double RotZ, double m[16])
{
	double mTrans[16], mRotX[16], mRotY[16], mRotZ[16], mRes1[16], mRes2[16];
	commonTranslationMatrix(transX, transY, transZ, mTrans);
	commonRotationMatrix(commonDegToRad(RotX), 1.0f, 0.0f, 0.0f, mRotX);
	commonRotationMatrix(commonDegToRad(RotY), 0.0f, 1.0f, 0.0f, mRotY);
	commonRotationMatrix(commonDegToRad(RotZ), 0.0f, 0.0f, 1.0f, mRotZ);

	commonMultiplyMatrix(mTrans, mRotX, mRes1);
	commonMultiplyMatrix(mRes1, mRotY, mRes2);
	commonMultiplyMatrix(mRes2, mRotZ, m);
}void commonTransformPoint(const double vSrcVector[3], const double mMatrix[16], double vOut[3])
{
	vOut[0] = mMatrix[0] * vSrcVector[0] + mMatrix[4] * vSrcVector[1] + mMatrix[8] *  vSrcVector[2] + mMatrix[12];
	vOut[1] = mMatrix[1] * vSrcVector[0] + mMatrix[5] * vSrcVector[1] + mMatrix[9] *  vSrcVector[2] + mMatrix[13];
	vOut[2] = mMatrix[2] * vSrcVector[0] + mMatrix[6] * vSrcVector[1] + mMatrix[10] * vSrcVector[2] + mMatrix[14];    
}