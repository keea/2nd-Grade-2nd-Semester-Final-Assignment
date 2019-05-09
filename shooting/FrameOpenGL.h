//////////////////////////////////////////////////////////////////////
// AniBuilderOpenGL.h
//////////////////////////////////////////////////////////////////////

#ifndef _ANIBUILDEROPENGL_H_
#define _ANIBUILDEROPENGL_H_

#include "gl.h"
#include "glu.h"


class CFrameOpenGL  
{
protected:
	float    m_ratio;
	float    m_fov;
	float    m_nearZ;
	float    m_farZ;

	int      m_width;
	int      m_height;
	
	HGLRC    m_hGLRC;


public:
	CFrameOpenGL();
	virtual ~CFrameOpenGL();

	BOOL Create(HDC hDC);
	BOOL SetPixelFormat(HDC hDC);
	BOOL InitGL();
	void SetSize(int width, int height);
	void SetViewport(int x, int y, int width, int height);
	void ResetViewport();

	void BeginRender();
	void EndRender(HDC hDC);

	void GetUnProject(double x, double y, double z, double &ux, double &uy, double &uz);
	void GetProject(double x, double y, double z, double &ux, double &uy, double &uz);
	
	void SetProjection();
};
#endif
