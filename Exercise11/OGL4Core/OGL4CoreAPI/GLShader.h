/**
 * @brief  OpenGL Shader program convenience class - header.
 * 
 * @file GLShader.h
 * 
 * Copyright (c) 2012,2013 by Universitaet Stuttgart (VISUS).
 * All rights reserved.
 * 
 * Authors: Guido Reina, Thomas Mueller
 * 
 * This file is part of OGL4Core.
 */

#ifndef VISUS_GLSHADER_H_INCLUDED
#define VISUS_GLSHADER_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

//#include "OGL4CoreAPI.h"
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


#include <iostream>
#include <cstring>
#include <map>

#include <GL/gl3w.h>
  
/**
 * @brief  OpenGL Shader program convenience class.
 * 
 * A new shader program can be created most easily using one of the
 * constructors. The filenames of the shaders have to be given with
 * an absolute path. If there are no shader errors, the shader program
 * is automatically being linked.
 * 
 * To use the shader program, just use Bind() and Release(). Uniform 
 * variables can be accessed via 'GetUniformLocation()'.
 * 
 * When you leave your plugin, do not forget to call 'RemoveAllShaders()'.
 */
class OGL4COREAPI_API GLShader
{
public:
    GLShader();
    ~GLShader();
    
    /** Bind shader program
     */
    void    Bind();
    
    /** Bind shader program attribute location.
     * @param attribIndex   Index of attribute.
     * @param attribName    Name of attribute.
     * @param linkProg      Link program after binding attribute.
     */
    bool    BindAttribLocation( unsigned int attribIndex, const char* attribName, bool linkProg = false );
    
    /** Create empty shader program.
     *    You have to attach shaders and link the program by yourself.
     */
    void    CreateEmptyProgram();
    
    /** Create shader program from file.
     * @param vShaderName  File name of vertex shader.
     * @param fShaderName  File name of fragment shader.
     */
    bool    CreateProgramFromFile( const char* vShaderName, 
                                   const char* fShaderName, FILE* fptr = stderr );
    
    /** Create shader program from file.
     * @param vShaderName  File name of vertex shader.
     * @param gShaderName  File name of geometry shader.
     * @param fShaderName  File name of fragment shader.
     */
    bool    CreateProgramFromFile( const char* vShaderName, 
                                   const char* gShaderName, 
                                   const char* fShaderName );
    
    /** Create shader program from file.
     * @param vShaderName  File name of vertex shader.     
     * @param tcShaderName File name of tessellation control shader.
     * @param teShaderName File name of tessellation evaluation shader.
     * @param fShaderName  File name of fragment shader.
     */
    bool    CreateProgramFromFile( const char* vShaderName,
                                   const char* tcShaderName,
                                   const char* teShaderName,
                                   const char* fShaderName );

    /** Create shader program from file.
     * @param vShaderName  File name of vertex shader.     
     * @param tcShaderName File name of tessellation control shader.
     * @param teShaderName File name of tessellation evaluation shader.
     * @param gShaderName  File name of geometry shader.
     * @param fShaderName  File name of fragment shader.
     */
    bool    CreateProgramFromFile( const char* vShaderName,
                                   const char* tcShaderName,
                                   const char* teShaderName,
                                   const char* gShaderName, 
                                   const char* fShaderName );
                                                           
    /** Create shader program from strings.
     * @param vShaderText  String text of vertex shader.
     * @param vShaderLen   Length of vertex shader text.
     * @param fShaderText  String text of fragment shader.
     * @param fShaderLen   Length of fragment shader text.
     */
    bool    CreateProgramFromString( const char* vShaderText, const size_t vShaderLen,
                                     const char* fShaderText, const size_t fShaderLen, FILE* fptr = stderr );
    
    /** Attach shader from file.
     * @param shaderName  File name of shader.
     * @param shaderType  Type of shader.
     * @param linkProg    Link program after attaching shader.
     */
    bool    AttachShaderFromFile( const char* shaderName, GLenum shaderType, bool linkProg = false );
    
    /** Attach shader from string.
     * @param shaderText  String text of shader.
     * @param shaderLen   Length of shader text.
     * @param shaderType  Type of shader.
     * @param linkProg    Link program after attaching shader.
     */
    bool    AttachShaderFromString( const char* shaderText, const size_t shaderLen, GLenum shaderType, bool linkProg = false );
    
    /** Get shader program handle.
     */
    GLuint  GetProgHandle();
    
    /** Get uniform location of shader variable.
     * @param name  Uniform variable name
     */
    GLint   GetUniformLocation(const char* name);  
    
    /** Link shader program.
     *    Linking is only necessary if you build the shader program
     *    by yourself.
     */ 
    bool    Link(FILE* fptr = stderr);
    
    void    PrintInfo(FILE* fptr = stderr);
    
    /** Remove all shaders from shader program.
     */
    void    RemoveAllShaders();
    
    /** Release shader program.
     */
    void    Release();   
    
    void    ClearSubsStrings();
    void    AddSubsStrings( const char* phText, const char* subsText );
    
    /** Prepend the header text to every shader that is read from file.
     *    E.g.:  prepend the version number of GLSL to be used:
     *            #version 330
     */
    void    PrependHeaderText( const char* header );

protected:
// we are not publishing any of that, thank you very much
#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable: 4251)
#endif
   
    GLuint  createShaderFromFile(const char* shaderFilename, GLenum type, FILE* fptr = stderr );      
        
    /** Create shader from string.
     * @param shaderText  Shader text.
     * @param shaderLen   Length of shader text.
     * @param type        Shader type.
     * \return Shader id.
     */ 
    GLuint  createShaderFromString(const char* shaderText, const size_t shaderLen, GLenum type, FILE* fptr = stderr);   
    size_t  readShaderFromFile(const char* shaderFilename, std::string &shaderContent, FILE* fptr = stderr );
      
    /** Print shader information log.
     * @param shader  Shader id.
     */
    bool    printShaderInfoLog( GLuint shader, FILE* fptr = stderr );
    
    /** Print program information log.
     */
    bool    printProgramInfoLog( FILE* fptr = stderr );   
    
// ---------- private attributes --------
 private:
    GLuint  progHandle;
    
    std::string  headerText;
    std::map<std::string,std::string> subsStrings;
    std::map<std::string,std::string>::iterator subsStringsItr;
    
#ifdef _WIN32
#pragma warning(pop)
#endif   
};
  

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* VISUS_GLSHADER_H_INCLUDED */
