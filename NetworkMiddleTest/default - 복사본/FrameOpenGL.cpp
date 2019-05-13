//////////////////////////////////////////////////////////////////////
// AniBuilderOpenGL.cpp
//////////////////////////////////////////////////////////////////////

#include "global.h"
#include "FrameOpenGL.h"
#include "glf.h"

#pragma comment(lib, ".\\opengl\\opengl32.lib")
#pragma comment(lib, ".\\opengl\\glu32.lib")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFrameOpenGL::CFrameOpenGL()
{
	m_width = 0;
	m_height = 0;
	m_fov = 50.f;
	m_nearZ = 0.01f;
	m_farZ = 4096.f;

	m_hGLRC = NULL;

	glfInit();
	glfLoadFont("fonts/chicago1.glf");
}

CFrameOpenGL::~CFrameOpenGL()
{
	glfClose();

	wglMakeCurrent (NULL, NULL) ; 
	wglDeleteContext (m_hGLRC); 
}



BOOL CFrameOpenGL::SetPixelFormat(HDC hDC)
{
	PIXELFORMATDESCRIPTOR pfd =											// pfd Tells Windows How We Want Things To Be
	{
		sizeof (PIXELFORMATDESCRIPTOR),									// Size Of This Pixel Format Descriptor
		1,																// Version Number
		PFD_DRAW_TO_WINDOW |											// Format Must Support Window
		PFD_SUPPORT_OPENGL |											// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,												// Must Support Double Buffering
		PFD_TYPE_RGBA,													// Request An RGBA Format
		16,													// Select Our Color Depth
		0, 0, 0, 0, 0, 0,												// Color Bits Ignored
		0,																// No Alpha Buffer
		0,																// Shift Bit Ignored
		0,																// No Accumulation Buffer
		0, 0, 0, 0,														// Accumulation Bits Ignored
		16,																// 16Bit Z-Buffer (Depth Buffer)  
		0,																// No Stencil Buffer
		0,																// No Auxiliary Buffer
		PFD_MAIN_PLANE,													// Main Drawing Layer
		0,																// Reserved
		0, 0, 0															// Layer Masks Ignored
	};

   int nPixelFormat = ::ChoosePixelFormat(hDC, &pfd);
	if(nPixelFormat == 0 || ::SetPixelFormat(hDC, nPixelFormat, &pfd) == FALSE)
	{
		MessageBox(NULL,"Error", "Error initializing OpenGL for Win32!", MB_OK);
		return FALSE;
	}

	return TRUE;
}

BOOL CFrameOpenGL::Create(HDC hDC)
{
	if(!SetPixelFormat(hDC))
		return FALSE;

	m_hGLRC = wglCreateContext(hDC);
	if(wglMakeCurrent(hDC, m_hGLRC) == FALSE)
		return FALSE;

	if(!InitGL())
		return FALSE;


	return TRUE;
}

BOOL CFrameOpenGL::InitGL()
{	
	glClearColor(0.5, 0.5, 0.5, 1.0);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//return TRUE;

	//glShadeModel(GL_SMOOTH);
	//glEnable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glFrontFace(GL_CW);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_COLOR_MATERIAL);

	


//	glMaterialfv(GL_FRONT, GL_AMBIENT, white);
//	glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
//	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
//	glLightfv(GL_LIGHT0, GL_AMBIENT, white);
//	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
//	glLightfv(GL_LIGHT0, GL_SPECULAR , white);
//	glLightfv(GL_LIGHT0, GL_POSITION, origin.v);
//	glEnable(GL_LIGHT0);

	//glEnable(GL_LINE_SMOOTH);
	//glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	//glEnable(GL_POINT_SMOOTH);
	//glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

	//glEnable(GL_POLYGON_OFFSET_FILL);
	//glPolygonOffset(1.f, 1.f);

	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	

	//glClearColor(0.5, 0.5, 0.5, 1.0);


	SetProjection();

	return TRUE;
}

void CFrameOpenGL::SetProjection()
{
//	if(m_ProjectionMode == mode) return; 

	glViewport(0, 0, m_width, m_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0.0f, m_width, m_height, 0.0f, -1.0f, 1.0f);
	//glOrthof(0, m_backingWidth, m_backingHeight, 0, -1.0f, 1.0f);
	//gluPerspective (45.0f, (GLfloat)(width)/(GLfloat)(height),			// Calculate The Aspect Ratio Of The Window
	//				1.0f, 100.0f);		

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void CFrameOpenGL::SetSize(int width, int height)
{
	if(height == 0) height = 1;

	m_width = width;
	m_height = height;

	SetProjection();
}

void CFrameOpenGL::SetViewport(int x, int y, int width, int height)
{
	glViewport(x, y, width, height);
}

void CFrameOpenGL::ResetViewport()
{
	glViewport(0, 0, m_width, m_height);
}


void CFrameOpenGL::BeginRender()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void CFrameOpenGL::EndRender(HDC hDC)
{
	SwapBuffers(hDC);
}

void CFrameOpenGL::GetUnProject(double x, double y, double z, double &ux, double &uy, double &uz)
{
	GLdouble modelMatrix[16];
	GLdouble projMatrix[16];
	GLint viewport[4];

	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);

	gluUnProject(x, y, z, modelMatrix, projMatrix, viewport, (double *)&ux, (double *)&uy, (double *)&uz);
}

void CFrameOpenGL::GetProject(double x, double y, double z, double &ux, double &uy, double &uz)
{
	GLdouble modelMatrix[16];
	GLdouble projMatrix[16];
	GLint viewport[4];

	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);

	gluProject(x, y, z, modelMatrix, projMatrix, viewport, (double *)&ux, (double *)&uy, (double *)&uz);
}


