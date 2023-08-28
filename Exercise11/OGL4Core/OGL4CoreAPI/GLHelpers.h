/**
 * @brief  OpenGL helper functions.
 * 
 * @file GLHelpers.h
 * 
 * Copyright (c) 2012,2013 by Universitaet Stuttgart (VISUS).
 * All rights reserved.
 * 
 * Authors: Guido Reina, Thomas Mueller
 * 
 * This file is part of OGL4Core.
 */

#ifndef VISUS_GL_HELPERS_H_INCLUDED
#define VISUS_GL_HELPERS_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include <GL/gl3w.h>
#include "glm.hpp"
#include <memory>

#ifdef _WIN32
#define OGL4COREPLUGIN_API __declspec(dllexport)
#define OGL4COREPLUGIN_CALL __cdecl
#else
#define OGL4COREPLUGIN_API
#define OGL4COREPLUGIN_CALL
#endif

//#define HAVE_KHR_DEBUG

#if (defined _DEBUG) && (!defined HAVE_KHR_DEBUG)
#define GLCE(x) glGetError(); (x); checkError(__FILE__, __LINE__);
#else
#define GLCE(x) (x)
#endif

GLenum checkError(std::unique_ptr<char> file, int line);

#ifdef HAVE_KHR_DEBUG
void APIENTRY DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
    const GLchar* message, GLvoid* userParam);
#endif    

/** Enable or disable sync to vblank.
 *   This function might not work on linux machines.
 * @param enable
 */
bool OGL4COREPLUGIN_API EnableVSync( bool enable );

/** Query if vsync is enabled or not.
 *   This function might not work on linux machines.
 */
bool OGL4COREPLUGIN_API IsVSyncEnabled();

/** Read single pixel color from color buffer.
 *    Take care that rgb is a non-null pointer.
 * @param x  Horizontal pixel coordinate.
 * @param y  Vertical pixel coordinate (from top).
 * @param rgb  Pointer to rgb color.
 * @param mode  Specifies a color buffer: GL_FRONT_LEFT,...,GL_COLOR_ATTACHMENTi (see glReadBuffer for details).
 */
void OGL4COREPLUGIN_API ReadPixelRGB( int x, int y, unsigned char *rgb, GLenum mode = GL_FRONT );

/** Read single pixel color from color buffer as float values.
 * @param x  Horizontal pixel coordinate.
 * @param y  Vertical pixel coordinate (from top).
 * @param mode  Specifies a color buffer: GL_FRONT_LEFT,...,GL_COLOR_ATTACHMENTi (see glReadBuffer for details).
 * @return Pixel color given as float values [0,1].
 */
glm::vec3 OGL4COREPLUGIN_API ReadPixelRGBf( int x, int y, GLenum mode = GL_FRONT );

/** Grab current framebuffer and save it to disk as png-image.
 * @param filename   Image file name.
 * @param x          Image origin (x).
 * @param y          Image origin (y).
 * @param width      Image width.
 * @param height     Image height.
 * @param mode       Specifiy color buffer.
 * @return true if image was written to file.
 */
bool OGL4COREPLUGIN_API GrabFramebuffer( const char* filename, 
                                         const unsigned int x, const unsigned int y, 
                                         const unsigned int width, const unsigned int height,
                                         GLenum mode = GL_FRONT );

/** Load ppm image from file and upload it onto GPU.
 *    NOTE that you have to delete the texture yourself when finished.
 * @param filename  Image file name (.ppm).
 * @param width     Image width.
 * @param height    Image height.
 * @return Texture handle.
 */
GLuint OGL4COREPLUGIN_API  LoadPPMTexture( const char* filename, int &width, int &height,
                                           GLint wrap = GL_REPEAT, GLint filter = GL_LINEAR );

bool   OGL4COREPLUGIN_API  LoadPPMDataAsFloat( const char* filename, int &width, int &height, float*& data );
                                           

GLuint OGL4COREPLUGIN_API  LoadPNGTexture( const char* filename, int &width, int &height,
                                           GLint wrap = GL_REPEAT, GLint filter = GL_LINEAR );

/** Upload data array to GPU texture.
 * @param width    Image width.
 * @param height   Image height.
 * @param wrap     Wrap image.
 * @param filter   Filter.
 */
GLuint OGL4COREPLUGIN_API  SetRGBTexture( const int width, const int height, const int numChan, const unsigned char* data, 
                                          GLint wrap = GL_REPEAT, GLint filter = GL_LINEAR );

GLuint OGL4COREPLUGIN_API  GetNoiseTexture( const unsigned int width, const unsigned int height, bool filter = true );

GLuint OGL4COREPLUGIN_API  GetSpotNoiseTexture( const unsigned int width, const unsigned int height, 
                                                unsigned int numSpots, bool filter = true );


/** Flip RGB image horizontally.
 *    The destination buffer will be allocated (new) here. You have to deallocate (delete) the buffer yourself!
 * @param src   Pointer to image source.
 * @param dst   empty pointer to image destination.
 * @param width  Image width.
 * @param height Image height.
 * @param numChannels  Number of channels (should be 3 for rgb image).
 */
void OGL4COREPLUGIN_API  FlipImageData( unsigned char* src, unsigned char *&dst, 
                                        const unsigned int width, const unsigned int height, const unsigned int numChannels );

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* VISUS_GL_HELPERS_H_INCLUDED */

