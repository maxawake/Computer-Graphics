/*
 * BoxRenderer.h
 * 
 * Copyright (c) 2012,2013 by Universitaet Stuttgart (VISUS).
 * All rights reserved.
 * 
 * Authors: Guido Reina, Thomas Mueller
 * 
 * This file is part of OGL4Core.
 */

#include "RenderPlugin.h"
#include <string>
#include "glm.hpp"
#include <GL/gl3w.h>

#include "GLShader.h"
#include "ObjLoader.h"
#include "VertexArray.h"

class OGL4COREPLUGIN_API BoxRenderer : public RenderPlugin {
public:
    BoxRenderer(COGL4CoreAPI *Api);
    ~BoxRenderer(void);

    virtual bool Activate(void);
    virtual bool Deactivate(void);
    virtual bool Init(void);
    virtual bool Keyboard(unsigned char key, int x, int y);
    virtual bool Motion(int x, int y);
    virtual bool Mouse(int button, int state, int x, int y);
    virtual bool Render(void);
    virtual bool Resize(int w, int h);

    void FileChanged(FileEnumVar<BoxRenderer> &var);

protected:
    bool loadObject(const char* objFile);
    
    void doDepthTest( GLStateVar<BoxRenderer> &var );

private:
    bool draw;
    glm::mat4 viewTrafo;
    int viewTrafoHandle;
    glm::mat4 boxTrafo;
    glm::mat4 cubeTrafo;
    float  aspectRatio;
    char   objectName[256];
    
    APIVar<BoxRenderer, FloatVarPolicy>  fovY;
    APIVar<BoxRenderer, FloatVarPolicy>  objScale;    
    APIVar<BoxRenderer, BoolVarPolicy>   useTexs;
    APIVar<BoxRenderer, BoolVarPolicy>   useLambert;
    APIVar<BoxRenderer, BoolVarPolicy>   useLinearFiltering;
    APIVar<BoxRenderer, Color3FVarPolicy>  lightIntensity;
    FileEnumVar<BoxRenderer> files;
    
    glm::mat4 viewMX;
    glm::mat4 projMX;
    GLuint  va_box;  
    GLuint  vbo_box; 
    GLuint  ibo_box;
    
    GLShader shader;
    std::string vShaderName;
    std::string fShaderName;
    
    ObjLoader objLoader;
    int       numObjs;
    int*      objOffsets;
    VertexArray va;
    std::vector<GLuint> preloadTexIDs;

    GLStateVar<BoxRenderer> depthTest;
};

extern "C" OGL4COREPLUGIN_API RenderPlugin* OGL4COREPLUGIN_CALL CreateInstance(COGL4CoreAPI *Api) {
    return new BoxRenderer(Api);
}
