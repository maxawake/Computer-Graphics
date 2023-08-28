/*
 * FragShaderToy.cpp
 * 
 * Copyright (c) 2012,2013 by Universitaet Stuttgart (VISUS).
 * All rights reserved.
 * 
 * Authors: Guido Reina, Thomas Mueller
 * 
 * This file is part of OGL4Core.
 */

#include "stdafx.h"
#include "FragShaderToy.h"
#include "GL/gl3w.h"
#include "gtc/matrix_transform.hpp"

const static float quadVerts[] = {
  0.0f, 0.0f,      // 1. vertex (x,y)
  1.0f, 0.0f,      // 2. vertex (x,y)
  0.0f, 1.0f,      // 3. vertex (x,y)
  1.0f, 1.0f       // 4. vertex (x,y)
};

FragShaderToy::FragShaderToy(COGL4CoreAPI *Api) : RenderPlugin(Api),
        va_quad(0),vbo_quad(0) {
    this->myName = "default/FragShaderToy";
    this->myDescription = "The FragShaderToy draws a window filling quad.\n\n" \
                          "Select a fragment shader or add your own one to " \
                          "the resources/Shaders folder.";

    aTime = 0.0f;
    aTimeStep = 0.01f;
    play  = false;
    maxN  = 128;
}

FragShaderToy::~FragShaderToy() {

}

bool FragShaderToy::Activate(void) {
    glClearColor( 0.0, 0.5, 1.0, 1.0 );
    
    std::string pathName = this->GetCurrentPluginPath();
    shaderFiles.Set(this, "Shader", (pathName + "/resources/Shaders/").c_str(), ".c", &FragShaderToy::FileChanged);
    shaderFiles.Register();
    
    aTime.Set(this, "time");
    aTime.Register("step=0.01");
    
    aTimeStep.Set(this, "tStep");
    aTimeStep.Register("step=1e-3 min=1e-3 max=1");

    play.Set(this, "play");
    play.Register();
    
    maxN.Set(this, "maxN");
    maxN.Register();
    
    fpsCounter.Set(this,"FPS",20);
    fpsCounter.Register();
        
    vShaderName = pathName + std::string("/resources/vert.c");
    fShaderName = pathName + std::string("/resources/frag.c");
    //fShaderName = pathName + std::string("/mandel.c");
    shader.CreateProgramFromFile(vShaderName.c_str(),fShaderName.c_str());
    
    // generate vertex array
    glGenVertexArrays(1,&va_quad);
    glGenBuffers(1,&vbo_quad);    
    glBindVertexArray(va_quad);
       glBindBuffer(GL_ARRAY_BUFFER,vbo_quad);
       glEnableVertexAttribArray(0);
       glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*4*2,quadVerts,GL_STATIC_DRAW);
       glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,NULL);
    glBindVertexArray(0);
    
    shaderFiles.Select(0);
    return true;
}

bool FragShaderToy::Deactivate(void) {
    glDeleteBuffers(1,&vbo_quad);
    glDeleteVertexArrays(1,&va_quad);
    shader.RemoveAllShaders();    
    play = false;
    return true;
}

bool FragShaderToy::Idle(void) {
    if (play) {
        fpsCounter.Start();
        aTime += aTimeStep;
        PostRedisplay();
        fpsCounter.Stop();
    }
    return true;
}

bool FragShaderToy::Init(void) {
    if (gl3wInit()) {
       fprintf(stderr,"Error: Failed to initialize gl3w.\n");
       return false;
    }    
    return true;
}

bool FragShaderToy::Keyboard(unsigned char key, int x, int y) {
    switch(key) {
        case 'r': {
            shader.RemoveAllShaders();
            shader.CreateProgramFromFile(vShaderName.c_str(),fShaderName.c_str());
            return true;
        }
        case 'c': {
            
            return true;
        }
    }
    return false;
}

bool FragShaderToy::Motion(int x, int y) {
    return false;
}

bool FragShaderToy::Mouse(int button, int state, int x, int y) {
    return false;
}

bool FragShaderToy::Render(void) {
    glClear( GL_COLOR_BUFFER_BIT );
    glm::mat4 pvMatrix = glm::ortho(0.0f,1.0f,0.0f,1.0f);
   
    shader.Bind();
    glUniformMatrix4fv(shader.GetUniformLocation("mvp"),1,GL_FALSE,glm::value_ptr(pvMatrix));
    
    int w,h;
    GetWindowSize(w,h);
    glUniform2f(shader.GetUniformLocation("resolution"),static_cast<float>(w),static_cast<float>(h));
    glUniform1f(shader.GetUniformLocation("time"),(float)this->aTime);
    glUniform1i(shader.GetUniformLocation("maxN"),(int)this->maxN);

    glBindVertexArray(va_quad);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
    
    shader.Release();
    return false;
}

bool FragShaderToy::Resize(int w, int h) {
    return false;
}   

void FragShaderToy::FileChanged(FileEnumVar<FragShaderToy> &var) {
    if (var.GetSelectedFileName() != nullptr) {
        fShaderName = std::string(var.GetSelectedFileName());
    } else {
        fShaderName = "none";
    }
    shader.RemoveAllShaders();    
    if (fShaderName.compare("none")==0) {
        fShaderName = std::string(this->GetCurrentPluginPath()) + std::string("/resources/frag.c");
    }
    shader.CreateProgramFromFile(vShaderName.c_str(),fShaderName.c_str());
}

