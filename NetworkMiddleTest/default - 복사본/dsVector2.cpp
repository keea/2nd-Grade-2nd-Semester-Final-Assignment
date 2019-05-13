//
//  dsVector2.mm
//  spEngine
//
//  Created by Ju Woosuk on 11. 3. 2..
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <math.h>
#include "dsVector2.h"



dsVector2::dsVector2()
{
}

dsVector2::dsVector2(float _x, float _y) : x(_x), y(_y)
{
}

dsVector2::~dsVector2()
{
}

dsVector2 &dsVector2::operator = (const dsVector2 vec)
{
	x = vec.x;
	y = vec.y;
	
	return *this;
}

dsVector2 &dsVector2::operator = (const float scalar)
{
	x = scalar;
	y = scalar;
	
	return *this;
}

bool dsVector2::operator == (const dsVector2 &vec) const
{
	return (x == vec.x && y == vec.y);
}

bool dsVector2::operator != (const dsVector2 &vec) const
{
	return (x != vec.x || y != vec.y);
}

dsVector2 dsVector2::operator + (const dsVector2 &vec) const 
{
	dsVector2 rVec;
	
	rVec.x = x + vec.x;
	rVec.y = y + vec.y;
	
	return rVec;
}

dsVector2 dsVector2::operator - (const dsVector2 &vec) const
{
	dsVector2 rVec;
	
	rVec.x = x - vec.x;
	rVec.y = y - vec.y;
	
	return rVec;
}

dsVector2 dsVector2::operator * (const dsVector2 &vec) const
{
	dsVector2 rVec;
	
	rVec.x = x * vec.x;
	rVec.y = y * vec.y;
	
	return rVec;
}

dsVector2 dsVector2::operator * (const float scalar) const
{
	dsVector2 rVec;
	
	rVec.x = x * scalar;
	rVec.y = y * scalar;
	
	return rVec;
}


dsVector2 dsVector2::operator / (const dsVector2 &vec) const
{
	//assert(vec.x != 0.0f && vec.y != 0.0f);
	
	dsVector2 rVec;
	
	rVec.x = x / vec.x;
	rVec.y = y / vec.y;
	
	return rVec;
}

dsVector2 dsVector2::operator / (const float scalar) const
{
	//assert(scalar != 0.0f);
	
	dsVector2 rVec;
	float v = 1.0/scalar;
	
	rVec.x = x * v;
	rVec.y = y * v;
	
	return rVec;
}

dsVector2 dsVector2::operator - () const
{
	dsVector2 rVec;
	
	rVec.x = -x;
	rVec.y = -y;
	
	return rVec;
}


dsVector2 &dsVector2::operator += (const dsVector2 &vec)
{
	x += vec.x;
	y += vec.y;
	
	return *this;
}

dsVector2 &dsVector2::operator += (const float scalar)
{
	x += scalar;
	y += scalar;
	
	return *this;
}

dsVector2 &dsVector2::operator -= (const dsVector2 &vec)
{
	x -= vec.x;
	y -= vec.y;
	
	return *this;
}

dsVector2 &dsVector2::operator -= (const float scalar)
{
	x -= scalar;
	y -= scalar;
	
	return *this;
}

dsVector2 &dsVector2::operator *= (const dsVector2 &vec)
{
	x *= vec.x;
	y *= vec.y;
	
	return *this;
}

dsVector2 &dsVector2::operator *= (const float scalar)
{
	x *= scalar;
	y *= scalar;
	
	return *this;
}

dsVector2 &dsVector2::operator /= (const dsVector2 &vec)
{
	//assert(vec._x != 0.0f && vec._y != 0.0f);
	
	x /= vec.x;
	y /= vec.y;
	
	return *this;
}

dsVector2 &dsVector2::operator /= (const float scalar)
{
	//assert(scalar != 0.0f);
	float v = 1.0/scalar;
	
	x *= v;
	y *= v;
			
	return *this;
}

float dsVector2::Length() const
{
	return sqrt(x*x + y*y);
}

float dsVector2::SquaredLength() const
{
	return (x*x + y*y);
}

float dsVector2::dotProduct(const dsVector2 &vec) const
{
	return (x*vec.x + y*vec.y);
}

float dsVector2::crossProduct(const dsVector2 &vec) const
{
	return x*vec.y - y*vec.x;
}

void dsVector2::MakeFloor( const dsVector2& cmp )
{
	if( cmp.x < x ) x = cmp.x;
	if( cmp.y < y ) y = cmp.y;            
}

void dsVector2::MakeCeil( const dsVector2& cmp )
{
	if( cmp.x > x ) x = cmp.x;
	if( cmp.y > y ) y = cmp.y;           
}

void dsVector2::Normalise()
{
	float length = sqrt(x*x + y*y);
	if(length > 1e-8)
	{
		float v = 1.0/length;
		x *= v;
		y *= v;
	}
}

dsVector2 dsVector2::MiddlePoint(const dsVector2 &vec) const
{
	return dsVector2( (x+vec.x)*0.5, (y+vec.y)*0.5);
}