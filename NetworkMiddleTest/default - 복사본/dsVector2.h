//
//  dsVector2.h

#ifndef _DSVECTOR2_H_
#define _DSVECTOR2_H_


class dsVector2 
{
public:
	float x, y;
	
	
	dsVector2();
	dsVector2(float _x, float _y);
	~dsVector2();
	
	dsVector2 &operator = (const dsVector2 vec);	
	dsVector2 &operator = (const float scalar);	
	bool operator == (const dsVector2 &vec) const;
	bool operator != (const dsVector2 &vec) const;
	dsVector2 operator + (const dsVector2 &vec) const;
	dsVector2 operator - (const dsVector2 &vec) const;
	dsVector2 operator * (const dsVector2 &vec) const;
	dsVector2 operator * (const float scalar) const;
	dsVector2 operator / (const dsVector2 &vec) const;
	dsVector2 operator / (const float scalar) const;
	dsVector2 operator - () const;
	dsVector2 &operator += (const dsVector2 &vec);
	dsVector2 &operator += (const float scalar);
	dsVector2 &operator -= (const dsVector2 &vec);
	dsVector2 &operator -= (const float scalar);
	dsVector2 &operator *= (const dsVector2 &vec);
	dsVector2 &operator *= (const float scalar);
	dsVector2 &operator /= (const dsVector2 &vec);
	dsVector2 &operator /= (const float scalar);
	
	float Length() const;
	float SquaredLength() const;
	float dotProduct(const dsVector2 &vec) const;
	float crossProduct(const dsVector2 &vec) const;
	void MakeFloor( const dsVector2& cmp );
	void MakeCeil( const dsVector2& cmp );
	void Normalise();
	dsVector2 MiddlePoint(const dsVector2 &vec) const;
};

#endif