//
//  dsResource.mm
//  spEngine
//
//  Created by Ju Woosuk on 11. 2. 11..
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "dsResource.h"


dsResource::dsResource(std::string name) : dsObject(name)
{
}

dsResource::~dsResource() 
{
}

bool dsResource::Load(std::string &name) 
{ 
	return true; 
}

void dsResource::Draw(float x, float y, float time)
{
}

