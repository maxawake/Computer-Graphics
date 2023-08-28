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
  
/**
 * @brief  OpenGL VertexArray convenience class.
 *
 *   The VertexArray represents a vertex array object consisting of
 *   ... buffer objects...
 *
 *   Initialize a new vertex array object by setting the number of vertices 
 *   and uploading the vertices:
 *
 *   Example:
 *      VertexArray  va;
 *      va.Create(4);
 *      va.SetArrayBuffer( 0, GL_FLOAT, 2, data );
 *
 *   Use the vertex array object via  Bind() and Release().
 *
 *   Do not forget to delete the vertex array object by means of
 *      va.Delete();
 */
class OGL4COREAPI_API VertexArray
{
public:
    VertexArray();        
    ~VertexArray();
    
    /** Bind vertex array. */
    void  Bind();
    
    /** Create vertex array. 
     */
    void  Create( const unsigned int numVerts );
    
    /** Delete vertex array.
     */
    void  Delete();
    
    /** Create vertex buffer object for vertices.
     * 
     * @param idx       Vertex attribute index.
     * @param type      Type of buffer object: GL_FLOAT,...
     * @param dim       Dimension of data entry: (2,3,4)
     * @param data      Pointer to data.
     * @param usage     Usage of vertex buffer
     */
    bool  SetArrayBuffer( GLuint idx, GLenum type, unsigned int dim, 
                          const void* data, GLenum usage = GL_STATIC_DRAW );

    // deprecated
    bool  SetArrayBuffer( GLuint idx, GLenum type, size_t tsize, unsigned int dim, 
                          const void* data, GLenum usage = GL_STATIC_DRAW );
    
    bool  SetElementBuffer( GLuint idx, const unsigned int numElems, 
                            const void* data, GLenum usage = GL_STATIC_DRAW );
    
    /** Update array buffer
     * 
     * @param idx        Vertex attribute index.
     * @param offset     Offset in numbers.
     * @param num        Number of elements.
     * @param data       pointer to data.
     */
    bool  SetSubArrayBuffer( GLuint idx, unsigned int offset, unsigned int num, const void* data );

    /** Get part of array buffer
     * 
     * @param idx        Vertex attribute index.
     * @param offset     Offset in numbers.
     * @param num        Number of elements.
     * @param data       pointer to data.
     */
    bool  GetSubArrayBuffer( GLuint idx, unsigned int offset, unsigned int num, void* data );

    /** Get maximum number of available vertex attributes. */
    int   GetMaxVertexAttribs();
    
    unsigned int GetNumVertices();

    /** Release vertex array. */
    void  Release();

protected:
// we are not publishing any of that, thank you very much
#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

    void  deleteAll();
    
private:
    GLuint va;    //!< Handle to vertex array.
    
    int*    isBOSet;    //!< List of 
    GLuint* vbo;        //!< List of handle to VBOs        
    GLenum* vboType;    //!< List of type of vertex data (GL_FLOAT,...)
    GLenum* vboUsage;   //!< List of usage types.
    size_t* sizeOfData;  //!< List of data size (sizeof(float),...)
    int*    dim;
        
    unsigned int numVertices;     //!< Number of vertices.
    
    int maxVertexAttrib;     

#ifdef _WIN32
#pragma warning(pop)
#endif   
};
  

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* VISUS_VERTEX_ARRAY_H_INCLUDED */

