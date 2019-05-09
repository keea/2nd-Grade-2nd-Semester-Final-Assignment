//
//  dsTexture.h

#ifndef _DSTEXTURE_H_
#define _DSTEXTURE_H_

#include "global.h"
#include "dsResource.h"

class dsTexture : public dsResource
{
public :
	int m_width;
	int m_height;

	float m_xPos;
	float m_yPos;
	
	GLuint m_texID;
	
public:
	dsTexture(std::string name);
	virtual ~dsTexture();
	
	bool LoadFromFilename(std::string name);
	bool Load(std::string &name);

	void SetPosition(float x, float y);
	void GetPosition(float &x, float &y);
	
	
	void Draw(int srcX, int srcY, int srcWidth, int srcHeight, unsigned int tick);
	void Draw(float x, float y, unsigned int tick);
	void Draw(dsDrawOP &DrawOP);
	
protected:	
	virtual void Update(float time) {}
};

#endif