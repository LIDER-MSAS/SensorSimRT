#ifndef _M3DLMS100_H_
#define _M3DLMS100_H_

#include "VirtualSensor.h"

class M3DLMS100:public VirtualSensor
{
public:
	M3DLMS100(void);
	~M3DLMS100(void);
private:
	void GenerateRays();
};

#endif