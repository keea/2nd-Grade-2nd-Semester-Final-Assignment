//
//  dsResource.h

#ifndef _DSRESOURCE_H_
#define _DSRESOURCE_H_


#include "dsObject.h"

class dsResource : public dsObject
{	
public:
	dsResource(std::string name);
	virtual ~dsResource();
	
	virtual bool Load(std::string &name);
	virtual void Draw(float x, float y, float time);
};

#endif
