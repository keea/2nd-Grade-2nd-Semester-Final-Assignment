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
	
	GLuint m_texID;
	
public:
	dsTexture(std::string name);
	virtual ~dsTexture();
	
	bool LoadFromFilename(std::string name);
	bool Load(std::string &name);
	
	
	void Draw(float x, float y, unsigned int tick);
	void Draw(dsDrawOP &DrawOP);
	void Draw(float dstX, float dstY, float srcX, float srcY, float width, float height, unsigned int tick);
	void DrawRotation(float x, float y, float angle);
	int GetWidth() { return m_width; }
	int GetHeight() { return m_height; }
	
protected:	
	virtual void Update(float time) {}
};

#endif