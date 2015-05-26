#ifndef _ZF5010_H_
#define _ZF5010_H_

#include "VirtualSensor.h"

class ZF5010:public VirtualSensor
{
public:
	ZF5010(void);
	~ZF5010(void);
private:
	void GenerateRays();
};

#endif