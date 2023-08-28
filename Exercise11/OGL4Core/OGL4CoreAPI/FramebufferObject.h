/**
 * @brief  OpenGL FramebufferObject convenience class - header.
 * 
 * @file FramebufferObject.h
 * 
 * Copyright (c) 2012,2013 by Universitaet Stuttgart (VISUS).
 * All rights reserved.
 * 
 * Authors: Guido Reina, Thomas Mueller
 * 
 * This file is part of OGL4Core.
 */

#ifndef VISUS_FRAMEBUFFER_OBJECT_H_INCLUDED
#define VISUS_FRAMEBUFFER_OBJECT_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "OGL4CoreAPI.h"
#include "GLShader.h"

#include <iostream>
#include <cstring>

#include <GL/gl3w.h>
  
/**
 * @brief  OpenGL FramebufferObject convenience class.
 */
class OGL4COREAPI_API FramebufferObject
{
public:
    FramebufferObject();
    ~FramebufferObject();
    
    enum AttachmentState {
        ATTACHMENT_DISABLED = 0,
        ATTACHMENT_RENDERBUFFER,
        ATTACHMENT_TEXTURE,
        ATTACHMENT_EXTERNAL_TEXTURE
    };

    typedef struct ColorAttachParams_t {
        GLenum internalFormat;
        GLenum format;
        GLenum type;
        
        ColorAttachParams_t() {
            internalFormat = GL_RGBA8;
            format = GL_RGBA;
            type = GL_UNSIGNED_BYTE;
        }
    } ColorAttachParams;


    typedef struct DepthAttachParams_t {
        GLenum format;
        AttachmentState state;
        GLuint externalID;
        
        DepthAttachParams_t() {
            format = GL_DEPTH_COMPONENT24;
            state  = ATTACHMENT_RENDERBUFFER;
            externalID = 0;
        }
    } DepthAttachParams;


    typedef struct StencilAttachParams_t {
        GLenum format;
        AttachmentState state;
        
        StencilAttachParams_t() {
            format = GL_STENCIL_INDEX;
            state  = ATTACHMENT_DISABLED;
        }
    } StencilAttachParams;


    /**
     * Get the maximum number of color attachments.
     */
    unsigned int GetMaxColorAttachments();

    /** 
     * Bind color texture.
     * @param which   Color attachment ID.
     * @return false  Color attachment is no texture or ID is out of range.
     */
    bool  BindColorTexture( const unsigned int which = 0 );

    /**
     * Bind depth texture if possible.
     * @return false   Depth attachment is no texture.
     */
    bool  BindDepthTexture();

    bool  Create( const unsigned int width, const unsigned int height,
                  const unsigned int cntColorAttachs,
                  const ColorAttachParams *cap,
                  const DepthAttachParams &dap,
                  const StencilAttachParams &sap );

    /**
     * Create a framebuffer object with the specified dimension having one
     * color attachment and a depth attachment. The depth attachment is
     * realized as a renderbuffer object.
     *
     * This is just a convenience method.
     *
     * @param width                Width of the framebuffer.
     * @param height               Height of the framebuffer.
     * @param colorInternalFormat  Internal format for the color attachment.
     * @param colorFormat          Format of the color attachment.
     * @param colorType            Datatype of the color attachment.
     * @param depthAttach          State of the depth attachment.
     * @param depthFormat          Format of the depth attachment.
     * @param stencilAttach
     * @param stencilFormat
     */
    inline bool  Create( const unsigned int width, const unsigned int height,
                  const GLenum colorInternalFormat = GL_RGBA8,
                  const GLenum colorFormat = GL_RGBA,
                  const GLenum colorType = GL_UNSIGNED_BYTE,
                  const AttachmentState depthAttach = ATTACHMENT_RENDERBUFFER,
                  const GLenum depthFormat = GL_DEPTH_COMPONENT24,
                  const AttachmentState stencilAttach = ATTACHMENT_DISABLED,
                  const GLenum stencilFormat = GL_STENCIL_INDEX) {
        ColorAttachParams cap;
        cap.internalFormat = colorInternalFormat;
        cap.format = colorFormat;
        cap.type = colorType;

        DepthAttachParams dap;
        dap.format = depthFormat;
        dap.state = depthAttach;

        StencilAttachParams sap;
        sap.format = stencilFormat;
        sap.state = stencilAttach;
        return this->Create( width, height, 1, &cap, dap, sap);
    }

    bool  Disable();
    
    /**
     * Convenience function to draw the color texture onto a standard quad 
     * using orthographic projection. Note that you have to set the viewport
     * yourself. 
     * 
     * @param colorAttach  
     * @param minFilter
     * @param magFilter
     */
    void  DrawColorTexture( const unsigned int colorAttach = 0, 
                            const GLint minFilter = GL_LINEAR,
                            const GLint magFilter = GL_LINEAR );                            

    /**
     * Convenience function to draw the depth buffer if it is a texture 
     * onto a standard quad using orthographic projection. Note that you
     * have to set the viewport yourself.
     * 
     * @param minFilter
     * @param magFilter
     */
    void  DrawDepthTexture( const GLint minFilter = GL_LINEAR,
                            const GLint magFilter = GL_LINEAR );   

    /** 
     * Enable the framebuffer object using the color attachment
     * with index 'colorAttach'.
     *
     * @param colorAttach   Index of the color attachment.
     */
    bool  Enable( const unsigned int colorAttach = 0 );

    /**
     * Enable the framebuffer object using multiple color attachments.
     *
     * @param cntColorAttachs   Number of color attachments.
     * @param colorAttachments  Pointer to the list of color attachments.
     */
    bool  EnableMultiplev( const unsigned int cntColorAttachs,
                           const unsigned int *colorAttachments );

    /**
     *  Get number of color attachments.
     */
    inline unsigned int GetCntColorAttachments() const {
        return this->cntColorAttachments;
    }

    /** 
     *  Get color texture handle ID.
     *
     * @param colorAttach  Index of color attachment.
     */
    GLuint GetColorTextureID( const unsigned int colorAttach = 0 );

    /**
     *  Get depth texture handle ID.
     */
    GLuint GetDepthTextureID() const;

    /**
     *  Get framebuffer object ID.
     */
    inline GLuint GetID() const {
        return this->fboID;
    }

    /**
     *  Get height of framebuffer object.
     */
    inline unsigned int GetHeight() const {
        return this->height;
    }

    /**
     * Get width of framebuffer object.
     */
    inline unsigned int GetWidth() const {
        return this->width;
    }


    bool IsValid() const;

    /**
     *  Delete framebuffer object and all of its color and depth attachments.
     */
    void Release();
    
    bool SetColorTexture( const GLenum colorFormat, const GLenum colorType, 
                          void* data, const unsigned int colorAttach = 0 );
    
    /**
     *  Unbind 2D texture.
     *   That is just a synonym for glBindTexture( GL_TEXTURE_2D, 0 );
     */
    void UnbindTexture();

protected:
// we are not publishing any of that, thank you very much
#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

    static const unsigned int ATTACH_IDX_DEPTH;     //!< Index of the depth attachment in 'attachmentOthers'
    static const unsigned int ATTACH_IDX_STENCIL;   //!< Index of the stencil attachment in 'attachmentOthers'

    typedef struct AttachmentProps_t {
        GLuint id;
        AttachmentState state;
    } AttachmentProps;
   
    void createRenderbuffer( GLuint &outID, const GLenum format );
    void createTexture( GLuint &outID, const GLenum internalFormat,
                        const GLenum format, const GLenum type );

    bool isComplete() const;

    void readTexture( void *outData, const GLuint id, const GLenum format,
                      const GLenum type );

    void saveState();
    
private:
    unsigned int cntColorAttachments;    //!< Number of color attachments.
    AttachmentProps *attachmentColor;    //!< Properties of the color attachments.
    AttachmentProps attachmentOther[2];  //!< Properties of the depth and stencil attachments.

    GLuint fboID;    //!< ID of the framebuffer.
    
    GLsizei height;  //!< Height of the FBO in pixels.
    GLsizei width;   //!< Width of the FBO in pixels;

    GLenum  oldDrawBuffer;   //!< Draw buffer to restore when disabling FBO.
    GLenum  oldReadBuffer;   //!< Read buffer to restore when disabling FBO.
    GLint   oldViewport[4];  //!< Viewport to restore when disabling FBO.

    GLShader  drawShader;
    glm::mat4 orthoMX;
    GLuint    va,vbo;
#ifdef _WIN32
#pragma warning(pop)
#endif   
};
  

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* VVISUS_FRAMEBUFFER_OBJECT_H_INCLUDED */

