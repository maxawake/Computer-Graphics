// MostSimple.cpp 
//

#include "stdafx.h"
#include "GL/gl.h"
#include "MostSimple.h"


MostSimple::MostSimple(COGL4CoreAPI *Api) : RenderPlugin(Api) {
    this->myName = "default/MostSimple";
    this->myDescription = "renders a single triangle";
}

MostSimple::~MostSimple() {
}

bool MostSimple::Activate(void) {
    return true;
}

bool MostSimple::Deactivate(void) {
    return true;
}

bool MostSimple::Init(void) {
    return true;
}

bool MostSimple::Render(void) {
    glClearColor( 0.0, 0.5, 1.0, 1.0 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glColor3f(1.0f,1.0f,0.0f);
    glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f(0.5f, -0.5f);
        glVertex2f(0.0f,0.5f);
    glEnd();    
    return false;
}

