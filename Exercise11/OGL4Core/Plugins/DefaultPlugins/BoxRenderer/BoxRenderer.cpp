/*
 * BoxRenderer.cpp
 * 
 * Copyright (c) 2012,2013 by Universitaet Stuttgart (VISUS).
 * All rights reserved.
 * 
 * Authors: Guido Reina, Thomas Mueller
 * 
 * This file is part of OGL4Core.
 */

#include "stdafx.h"
#include "BoxRenderer.h"
#include "PpmBitmapCodec.h"
#include "BitmapImage.h"
#include "Defs.h"
#include "GLHelpers.h"

#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>


BoxRenderer::BoxRenderer(COGL4CoreAPI *Api) : RenderPlugin(Api), draw(true) {
    this->myName = "default/BoxRenderer";
    this->myDescription = "The BoxRenderer is a default plugin for test purposes and renders a single box.";
}

BoxRenderer::~BoxRenderer() {
}

void BoxRenderer::FileChanged(FileEnumVar<BoxRenderer> &var) {
    fprintf(stderr,"File: %s\n",var.GetSelectedFileName());
    loadObject(var.GetSelectedFileName());
}

bool BoxRenderer::Activate(void) {
    viewTrafoHandle = this->AddManipulator("View", &this->viewTrafo, Manipulator::MANIPULATOR_ORBIT_VIEW_3D,false);
    this->AddManipulator("Object", &this->boxTrafo, Manipulator::MANIPULATOR_OBJ_MOVE_SCALE_ROT);
    this->SelectCurrentManipulator(1);
    //this->SetManipulatorDolly(viewTrafoHandle,-12.0);
    //this->SetManipulatorRotation(viewTrafoHandle, glm::vec3(0,0,1), 45);

    std::string pathName = this->GetCurrentPluginPath();

    files.Set(this, "Object", (pathName+"/resources/objects").c_str(), ".obj", &BoxRenderer::FileChanged);
    files.Register(nullptr);
    files.SetKeyShortcutIncr("n");

    objScale.Set(this, "ObjScale", nullptr);
    objScale.Register("step=0.01 min=0.001 max=10.0");

    useTexs.Set(this, "UseTexs");
    useTexs.Register();
    useTexs.SetKeyShortcut("x");
    useTexs.SetHelpMessage("Toggle texture usage.");
    
    useLinearFiltering.Set(this,"Lin.Filter");
    useLinearFiltering.Register();
    
    useLambert.Set(this,"Lambert");
    useLambert.Register();
    useLambert.SetKeyShortcut("l");

    fovY.Set(this, "FoVY");
    fovY.Register("step=0.1 min=1 max=120.0");

    lightIntensity.Set(this, "Light");
    lightIntensity.Register();
    
    depthTest.Set(this,"DepthTest",GL_DEPTH_TEST,&BoxRenderer::doDepthTest);
    depthTest.Register();
    depthTest = true;

    
    this->fovY     = 30.0f;
    this->objScale = 0.5f;
    this->useTexs  = true;
    this->useLambert = true;
    this->useLinearFiltering = true;
    this->lightIntensity = glm::vec3(0.7);


    vShaderName = pathName + std::string("/resources/vert.c");
    fShaderName = pathName + std::string("/resources/frag.c");
    shader.CreateProgramFromFile(vShaderName.c_str(),fShaderName.c_str());

    glGenVertexArrays(1,&va_box);
    glGenBuffers(1,&vbo_box);
    glGenBuffers(1,&ibo_box);

    glBindVertexArray(va_box);
       glBindBuffer(GL_ARRAY_BUFFER,vbo_box);
       glEnableVertexAttribArray(0);
       glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*4*ogl4_num4dBoxVerts,ogl4_4dBoxVerts,GL_STATIC_DRAW);
       glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,0,NULL);
       glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_box);
       glBufferData(GL_ELEMENT_ARRAY_BUFFER, ogl4_numBoxEdges*2*sizeof(GLuint), ogl4_BoxEdges, GL_STATIC_DRAW);
    glBindVertexArray(0);

    int width,height;
    this->GetWindowSize(width,height);
    projMX = glm::perspective(45.0f, width/(float)height, 0.01f,100.0f );

    files.Find("box.obj");
    return true;
}

bool BoxRenderer::Deactivate(void) {
    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    glDeleteBuffers(1,&vbo_box);
    glDeleteBuffers(1,&ibo_box);
    glDeleteVertexArrays(1,&va_box);

    va.Delete();

    for (auto x: preloadTexIDs) {
        glDeleteTextures(1, &x);
    }
    objLoader.ClearAll();

    preloadTexIDs.clear();
    shader.RemoveAllShaders();

    glDisable(GL_DEPTH_TEST);
    return true;
}

bool BoxRenderer::Init(void) {
    if (gl3wInit()) {
       fprintf(stderr,"Error: Failed to initialize gl3w.\n");
       return false;
    }
    return true;
}

bool BoxRenderer::Keyboard(unsigned char key, int x, int y) {
    switch(key) {
        case 'r': {
            fprintf(stderr,"Reload shaders...\n");
            shader.RemoveAllShaders();
            shader.CreateProgramFromFile(vShaderName.c_str(),fShaderName.c_str());
            return true;
        }
        case 's': {
            int w,h;
            this->GetWindowSize(w,h);
            GrabFramebuffer("test.png",0,0,w,h);
            return true;
        }
    }
    return false;
}

bool BoxRenderer::Motion(int x, int y) {
    return false;
}

bool BoxRenderer::Mouse(int button, int state, int x, int y) {
    return false;
}

bool BoxRenderer::Render(void) {
    glClearColor( 0.1f, 0.1f, 0.1f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    projMX = glm::perspective((float)this->fovY, aspectRatio, 0.01f,100.0f );

    shader.Bind();

    glUniformMatrix4fv(shader.GetUniformLocation("proj_matrix"),1,GL_FALSE,glm::value_ptr(projMX));
    glUniformMatrix4fv(shader.GetUniformLocation("inv_view_matrix"),1,GL_FALSE,glm::value_ptr(glm::inverse(viewTrafo)));
    glUniformMatrix4fv(shader.GetUniformLocation("view_matrix"),1,GL_FALSE,glm::value_ptr(viewTrafo));
    glUniformMatrix4fv(shader.GetUniformLocation("model_matrix"),1,GL_FALSE,glm::value_ptr(glm::mat4()));
    glUniform3f(shader.GetUniformLocation("translate"),0.5f,0.5f,0.5f);
    glUniform1f(shader.GetUniformLocation("scale"),1.0f);

    glUniform1i(shader.GetUniformLocation("useTexs"),0);
    glUniform1i(shader.GetUniformLocation("showWhat"),0);
    glUniform3f(shader.GetUniformLocation("Kd"),1.0f,1.0f,1.0f);
    glUniform3f(shader.GetUniformLocation("light"),1.0f,1.0f,1.0f);
    glBindVertexArray(va_box);
    glDrawElements(GL_LINES,ogl4_numBoxEdges*2,GL_UNSIGNED_INT,(void*)0);
    glBindVertexArray(0);

    glm::vec4 cov = objLoader.CenterOfVertices();
    glUniform3f(shader.GetUniformLocation("translate"),cov.x,cov.y,cov.z);
    glUniform1f(shader.GetUniformLocation("scale"),this->objScale);
    glUniform1i(shader.GetUniformLocation("useTexs"),(this->useTexs?1:0));
    glUniform1i(shader.GetUniformLocation("useLambert"),(this->useLambert?1:0));
    glUniform1i(shader.GetUniformLocation("showWhat"),1);

    glm::vec3 light = lightIntensity;
    glUniform3f(shader.GetUniformLocation("light"),light[0],light[1],light[2]);

    glUniformMatrix4fv(shader.GetUniformLocation("model_matrix"),1,GL_FALSE,glm::value_ptr(boxTrafo));

    unsigned int *objOffsets = objLoader.GetDrawOffsets();
    if (objOffsets!=nullptr) {
        obj_material_t* mat;

        va.Bind();
        for(unsigned int i=0; i<objLoader.GetNumDrawObjects(); i++ ) {
            mat = objLoader.GetMaterial(i);
            
            if (mat!=NULL) {
                glUniform3f(shader.GetUniformLocation("Ka"),mat->Ka[0],mat->Ka[1],mat->Ka[2]);
                glUniform3f(shader.GetUniformLocation("Kd"),mat->Kd[0],mat->Kd[1],mat->Kd[2]);
                glUniform1i(shader.GetUniformLocation("useTexs"),(this->useTexs?1:0));
                if (mat->mapID>=0 && mat->mapID<preloadTexIDs.size()) {
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_2D, preloadTexIDs[mat->mapID]);
                    if (this->useLinearFiltering) {
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    } else {
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                    }
                    glUniform1i(shader.GetUniformLocation("tex"),0);
                } else {
                    glUniform1i(shader.GetUniformLocation("useTexs"),0);
                }
            } else {
                glUniform3f(shader.GetUniformLocation("Ka"),0.3f,0.3f,0.3f);
                glUniform3f(shader.GetUniformLocation("Kd"),0.8f,0.8f,0.8f);
            }
            glDrawArrays(GL_TRIANGLES,objOffsets[i],(objOffsets[i+1]-objOffsets[i]));
        }
        va.Release();
    }
    glBindTexture(GL_TEXTURE_2D,0);

    shader.Release();
    return false;
}

bool BoxRenderer::Resize(int width, int height) {
    aspectRatio = width/static_cast<float>(height);
    projMX = glm::perspective((float)this->fovY, aspectRatio, 0.01f,100.0f );
    return true;
}

bool BoxRenderer::loadObject( const char* objFile ) {
    objLoader.ClearAll();
    if (objFile==nullptr) {
        return true;
    }

    std::string oFileName = std::string(objFile);
    size_t fileLen = oFileName.length();
    
#ifdef _WIN32
    size_t posSep  = oFileName.find_last_of("\\");
    std::string objFileName = oFileName.substr(posSep,fileLen-posSep);
    std::string objFolder   = oFileName.substr(0,posSep);
#else
    size_t posSep  = oFileName.find_last_of("/");
    std::string objFileName = oFileName.substr(posSep+1,fileLen-posSep);
    std::string objFolder   = oFileName.substr(0,posSep);
    std::cerr << objFileName << " " << objFolder << std::endl;
#endif

    objLoader.ReadObjFile(objFolder.c_str(),objFileName.c_str());

    float *verts, *norm, *tc;
    if (!objLoader.GenDrawObjects(verts,norm,tc)) {
        fprintf(stderr,"Cannot extract object data.\n");
        return true;
    }

    unsigned int numTextures = objLoader.GetNumTextures();
    for(unsigned int i=0; i<numTextures; i++) {
        preloadTexIDs.push_back(0);
    }

    int texIDref;
    for(unsigned int i=0; i<numTextures; i++) {
        const char* filename = objLoader.GetTextureName(i,texIDref);
        if (filename==NULL)
            continue;

        std::string imageFilename = objFolder + "/" + std::string(filename);
        if (texIDref>=0 && texIDref < static_cast<int>(preloadTexIDs.size())) {
        std::cerr << imageFilename << std::endl;

            PpmBitmapCodec ppmCodec;
            ppmCodec.Image() = new BitmapImage();
            if (ppmCodec.Image() == NULL)
                continue;

            if (ppmCodec.LoadFromFile(imageFilename.c_str())) {
                BitmapImage* image = ppmCodec.Image();

                GLuint texID = 0;
                glGenTextures(1,&texID);
                glBindTexture(GL_TEXTURE_2D,texID);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->Width(), image->Height(), 0, GL_RGB, GL_UNSIGNED_BYTE, image->PeekDataAs<char>());
                glBindTexture(GL_TEXTURE_2D,0);

                preloadTexIDs[texIDref] = texID;
            }
            delete ppmCodec.Image();
            ppmCodec.Image() = NULL;
        }
    }

    va.Delete();

    va.Create(objLoader.GetNumDrawVertices());
    va.SetArrayBuffer(0,GL_FLOAT,4,verts);
    va.SetArrayBuffer(1,GL_FLOAT,3,norm);
    va.SetArrayBuffer(2,GL_FLOAT,2,tc);

    delete [] verts;
    delete [] norm;
    delete [] tc;
    return true;
}

void BoxRenderer::doDepthTest( GLStateVar<BoxRenderer> &var ) {
    fprintf(stderr,"Zonk\n");
}
