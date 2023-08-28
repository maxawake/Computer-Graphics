/**
 * @brief  OpenGL VertexArray convenience class - header.
 * 
 * @file VertexArray.h
 * 
 * Copyright (c) 2012,2013 by Universitaet Stuttgart (VISUS).
 * All rights reserved.
 * 
 * Authors: Guido Reina, Thomas Mueller
 * 
 * This file is part of OGL4Core.
 */


#ifndef VISUS_VERTEX_ARRAY_H_INCLUDED
#define VISUS_VERTEX_ARRAY_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#ifdef _WIN32
#ifdef OGL4COREAPI_EXPORTS
// http://support.microsoft.com/kb/168958/en-us
#define OGL4COREAPI_API __declspec(dllexport)
#else
#define OGL4COREAPI_API __declspec(dllimport)
#endif
#define OGL4COREAPI_CALL __stdcall
#else
#define OGL4COREAPI_API 
#define OGL4COREAPI_CALL
#undef APIENTRY
#define APIENTRY
#endif

//#include "OGL4CoreAPI.h"

#include <iostream>
#include <cstring>

#include <GL/gl3w.h>

#include "Defs.h"
#include "GLShader.h"
#include "glm.hpp"

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <algorithm>

typedef struct char_info_t
{
    float adv_x;         //!< advance in x direction
    float adv_y;         //!< advance in y direction
    float bitmap_width;  
    float bitmap_height;
    float bitmap_left;
    float bitmap_top;
    float tx;            //!< x offset of glyph in texture coordinates
} char_info;
   
   
typedef struct font_atlas_t
{
   GLuint texID;        //!< texture id
   float  width;        //!< width of texture
   float  height;       //!< height of texture
   char_info  c[128];   //!< character information
} font_atlas;   
   
   
const std::string  vDTShaderText = std::string(
   "#version 330\n\n"  
   "uniform mat4 projMX;\n"
   "layout( location = 0) in  vec4 coord;\n"
   "out vec2 texpos;\n"
   "void main(void) {\n"
   "  gl_Position = projMX * vec4(coord.xy, 0, 1);\n"
   "  texpos = coord.zw;\n"
   "}\n");
   
const std::string  fDTShaderText = std::string(
   "#version 330\n\n"
   "uniform sampler2D tex;\n"
   "uniform vec3 color;\n"
   "in  vec2 texpos;\n"
   "layout( location = 0 ) out vec4 out_frag_color;\n"
   "void main(void) {\n"
   "  out_frag_color = vec4(1, 1, 1, texture2D(tex, texpos).r) * vec4(color,1.0);\n"
   //"  out_frag_color = vec4(1,1,0,1);\n"
   "}\n");
   
class RenderTextPimpl;
   
class OGL4COREAPI_API RenderText
{
public:    
    RenderText( const char* fontFilename, unsigned int size );
    RenderText( const char* fontFilename, unsigned int size, glm::vec3 color );
    ~RenderText();
    
    void  setColor      ( float r, float g, float b );   
    void  setColor      ( glm::vec3 col );
    void  getColor      ( float &r, float &g, float &b );
    
    void  render        ( const char* text, float x, float y, float sx = 1.0, float sy = 1.0 );
    void  printf        ( float x, float y, const char* fmt, ... );
    
    void  setWindowSize ( int w, int h );

protected:
// we are not publishing any of that, thank you very much
#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable: 4251)
#endif 
    
    void  init ();   
   
private:
    std::string  fontFilename_;
    unsigned int fontSize_;
    
    float        winWidth;
    float        winHeight;
    
    GLuint       va_;
    GLuint       vbo_;
    GLint        unifLoc_projMX_;
    GLint        unifLoc_tex_;
    GLint        unifLoc_color_;

    RenderTextPimpl* rtp;
    
    GLShader     shader;
    glm::mat4    projMX_;
    glm::vec3    color_;
    glm::vec4*   coords_;
};


#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* VISUS_VERTEX_ARRAY_H_INCLUDED */
