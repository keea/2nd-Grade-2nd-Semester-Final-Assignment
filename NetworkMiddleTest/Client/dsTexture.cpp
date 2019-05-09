//
//  dsTexture.m
//  spEngine
//
//  Created by Ju Woosuk on 11. 2. 10..
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "dsTexture.h"
#include "png.h"
#include "pngstruct.h"
#include "pnginfo.h"
#include <iostream>

#define png_infopp_NULL (png_infopp)NULL
#define int_p_NULL (int*)NULL
#define png_voidp_NULL (png_voidp)NULL
#define png_error_ptr_NULL (png_error_ptr)NULL


static const GLfloat TexCoords[] = 
{
	0, 0,
	1, 0,
	0, 1,
	1, 1,
};

static GLfloat TexRect[8];



bool loadPngImage(char *name, int &outWidth, int &outHeight, bool &outHasAlpha, GLubyte **outData) 
{  
     png_structp png_ptr;  
     png_infop info_ptr;  
     unsigned int sig_read = 0;  
     int color_type, interlace_type;  
     FILE *fp;  

     if ((fp = fopen(name, "rb")) == NULL)  
         return false;  

     /* Create and initialize the png_struct  
      * with the desired error handler  
      * functions.  If you want to use the  
      * default stderr and longjump method,  
      * you can supply NULL for the last  
      * three parameters.  We also supply the  
      * the compiler header file version, so  
      * that we know if the application  
      * was compiled with a compatible version  
      * of the library.  REQUIRED  
      */ 

     png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);  
     if (png_ptr == NULL) {  
         fclose(fp);  

         return false;  
     }  

     /* Allocate/initialize the memory  
      * for image information.  REQUIRED. */ 

     info_ptr = png_create_info_struct(png_ptr);  
     if (info_ptr == NULL) {  
         fclose(fp);  
         png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);  

         return false;  
     }  

     /* Set error handling if you are  
      * using the setjmp/longjmp method  
      * (this is the normal method of  
      * doing things with libpng).  
      * REQUIRED unless you  set up  
      * your own error handlers in  
      * the png_create_read_struct()  
      * earlier.  
      */ 

     if (setjmp(png_jmpbuf(png_ptr))) {  
         /* Free all of the memory associated  
          * with the png_ptr and info_ptr */ 

         png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);  
         fclose(fp);  

         /* If we get here, we had a  
          * problem reading the file */ 

         return false;  
     }  

     /* Set up the output control if  
     * you are using standard C streams */ 
     png_init_io(png_ptr, fp);  

     /* If we have already  
      * read some of the signature */ 

     png_set_sig_bytes(png_ptr, sig_read);  

     /*  
      * If you have enough memory to read  
      * in the entire image at once, and  
      * you need to specify only  
      * transforms that can be controlled  
      * with one of the PNG_TRANSFORM_*  
      * bits (this presently excludes  
      * dithering, filling, setting  
      * background, and doing gamma  
      * adjustment), then you can read the  
      * entire image (including pixels)  
      * into the info structure with this  
      * call  
      *  

      * PNG_TRANSFORM_STRIP_16 |  
      * PNG_TRANSFORM_PACKING  forces 8 bit  
      * PNG_TRANSFORM_EXPAND forces to  
      *  expand a palette into RGB  
      */ 

	 png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, png_voidp_NULL);
     outWidth = info_ptr->width;  
     outHeight = info_ptr->height;  

     switch (info_ptr->color_type) {  
         case PNG_COLOR_TYPE_RGBA:  
             outHasAlpha = true;  
             break;  

         case PNG_COLOR_TYPE_RGB:  
             outHasAlpha = false;  
             break;  

         default:  
             std::cout << "Color type " << info_ptr->color_type << " not supported" << std::endl;  
             png_destroy_read_struct(&png_ptr, &info_ptr, NULL);  
             fclose(fp);  

            return false;  
     }  

     unsigned int row_bytes = png_get_rowbytes(png_ptr, info_ptr);  
     *outData = (unsigned char*) malloc(row_bytes * outHeight);  

     png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);  

     for (int i = 0; i < outHeight; i++) {  
         // note that png is ordered top to  
         // bottom, but OpenGL expect it bottom to top  
        // so the order or swapped  

         //memcpy(*outData+(row_bytes * (outHeight-1-i)), row_pointers[i], row_bytes);  
		 memcpy(*outData + (row_bytes * i), row_pointers[i], row_bytes);
     }  

     /* Clean up after the read,  
      * and free any memory allocated */ 

     png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);  

     /* Close the file */ 
     fclose(fp);  

     /* That's it */ 

     return true;  
}  


dsTexture::dsTexture(std::string name) : dsResource(name)
{
	m_texID = 0;	

	m_xPos = 0.0f;
	m_yPos = 0.0f;
}

dsTexture::~dsTexture()
{
	if (m_texID) 
	{
		glDeleteTextures(1, &m_texID);
	}
}

bool dsTexture::LoadFromFilename(std::string name)
{
	return Load(name);
}

bool dsTexture::Load(std::string &name)
{
	GLubyte		   *ImageData;
	
	
	bool hasAlpha; 
	bool success = loadPngImage((char *)name.data(), m_width, m_height, hasAlpha, &ImageData); 

	if(success)
	{
		 glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 		  
		
		// Use OpenGL ES to generate a name for the texture.
		glGenTextures(1, &m_texID);
		// Bind the texture name. 
		glBindTexture(GL_TEXTURE_2D, m_texID);
		// Speidfy a 2D texture image, provideing the a pointer to the image data in memory
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ImageData);
		glTexImage2D(GL_TEXTURE_2D, 0, hasAlpha ? 4 : 3, m_width,  m_height, 0, hasAlpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE,  
			           ImageData);  				
		// Release the image data
		free(ImageData);
		
		// Set the texture parameters to use a minifying filter and a linear filer (weighted average)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
/*		
		// Enable use of the texture
		glEnable(GL_TEXTURE_2D);
		// Set a blending function to use
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// Enable blending
		glEnable(GL_BLEND);
*/
		return true;
	}
	
	return false;
}

void dsTexture::SetPosition(float x, float y)
{
	m_xPos = x;
	m_yPos = y;
}

void dsTexture::GetPosition(float &x, float &y)
{
	x = m_xPos;
	y = m_yPos;
}

void dsTexture::Draw(int srcX, int srcY, int srcWidth, int srcHeight, unsigned int tick)
{
	float x = (float)srcX / (float)m_width;
	float y = (float)srcY / (float)m_height;
	float width = (float)srcWidth / (float)m_width;
	float height = (float)srcHeight / (float)m_height;

	TexRect[0] = m_xPos;
	TexRect[1] = m_yPos;
	TexRect[2] = m_xPos + srcWidth;
	TexRect[3] = m_yPos;
	TexRect[4] = m_xPos;
	TexRect[5] = m_yPos + srcHeight;
	TexRect[6] = m_xPos + srcWidth;
	TexRect[7] = m_yPos + srcHeight;

	
	GLfloat TexCoords[] = { x, y, x + width, y, x, y + height, x + width, y + height };


	glBindTexture(GL_TEXTURE_2D, m_texID);

	glVertexPointer(2, GL_FLOAT, 0, TexRect);
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, TexCoords);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glPushMatrix();

	glTranslatef(m_xPos, m_yPos, 0.0f);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glPopMatrix();
}

void dsTexture::Draw(float x, float y, unsigned int tick)
{
	TexRect[0] = x;
	TexRect[1] = y;
	TexRect[2] = x + m_width;
	TexRect[3] = y;
	TexRect[4] = x;
	TexRect[5] = y + m_height;
	TexRect[6] = x + m_width;
	TexRect[7] = y + m_height;
	
	
	glBindTexture(GL_TEXTURE_2D, m_texID);
	
	glVertexPointer(2, GL_FLOAT, 0, TexRect);
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, TexCoords);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
}

void dsTexture::Draw(dsDrawOP &DrawOP)
{
	glBindTexture(GL_TEXTURE_2D, m_texID);
	
	glVertexPointer(2, GL_FLOAT, 0, DrawOP.rectDraw);
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, DrawOP.rectCoord);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);	
}


