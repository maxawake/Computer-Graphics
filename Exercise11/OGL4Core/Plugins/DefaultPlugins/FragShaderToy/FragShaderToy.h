/*
 * FragShaderToy.h
 * 
 * Copyright (c) 2012,2013 by Universitaet Stuttgart (VISUS).
 * All rights reserved.
 * 
 * Authors: Guido Reina, Thomas Mueller
 * 
 * This file is part of OGL4Core.
 */

#include "RenderPlugin.h"
#include "glm.hpp"
#include "GLShader.h"

class OGL4COREPLUGIN_API FragShaderToy : public RenderPlugin {
public:
    FragShaderToy(COGL4CoreAPI *Api);
    ~FragShaderToy(void);

    virtual bool Activate(void);
    virtual bool Deactivate(void);
    virtual bool Idle(void);
    virtual bool Init(void);
    virtual bool Keyboard(unsigned char key, int x, int y);
    virtual bool Motion(int x, int y);
    virtual bool Mouse(int button, int state, int x, int y);
    virtual bool Render(void);
    virtual bool Resize(int w, int h);
    
    void FileChanged(FileEnumVar<FragShaderToy> &var);

private:
    FileEnumVar<FragShaderToy> shaderFiles;
    APIVar<FragShaderToy, FloatVarPolicy> aTime;
    APIVar<FragShaderToy, FloatVarPolicy> aTimeStep;
    APIVar<FragShaderToy, BoolVarPolicy>  play;
    APIVar<FragShaderToy, IntVarPolicy>   maxN;
    
    FPSCounterVar<FragShaderToy>  fpsCounter;
    
    GLuint  va_quad;
    GLuint  vbo_quad;
    
    GLShader shader;
    std::string vShaderName;
    std::string fShaderName;
};

extern "C" OGL4COREPLUGIN_API RenderPlugin* OGL4COREPLUGIN_CALL CreateInstance(COGL4CoreAPI *Api) {
    return new FragShaderToy(Api);
}
