#ifndef _COMMON_H_
#define _COMMON_H_

#define _USE_MATH_DEFINES
#include <math.h>
#include <cstdio>
#define commonDegToRad(x)	((x)*M_PI/180.0)
#define commonRadToDeg(x)	((x)*180.0/M_PI)

void readData(FILE* in, unsigned int* num_elems, void** data, size_t elem_size,const char* filename);

void commonLoadIdentityMatrix(double m[16]);
void commonTranslationMatrix(double x, double y, double z, double mTranslate[16]);
void commonMultiplyMatrix(const double m1[16], const double m2[16], double mProduct[16]);
void commonRotationMatrix(double angle, double x, double y, double z, double mMatrix[16]);
void commonMakeMatrix(double transX, double transY, double transZ, double RotX, double RotY, double RotZ, double m[16]);
void commonTransformPoint(const double vSrcVector[3], const double mMatrix[16], double vOut[3]);
#endif