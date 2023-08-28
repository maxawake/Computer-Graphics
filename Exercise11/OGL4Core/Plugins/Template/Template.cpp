// [Template].cpp 
//

#include "stdafx.h"
#include "GL/gl.h"
#include "[Template].h"


[Template]::[Template](COGL4CoreAPI *Api) : RenderPlugin(Api) {
    this->myName = "[Template]";
    this->myDescription = "renders a single triangle";
    draw = true;        
}

[Template]::~[Template]() {

}

bool [Template]::Activate(void) {
    draw.Set(this,"draw", nullptr);
    draw.Register(nullptr);
    
    this->AddManipulator("trafo", &this->trafo);
    return true;
}

bool [Template]::Deactivate(void) {
    return true;
}

bool [Template]::Init(void) {
    return true;
}

bool [Template]::Render(void) {
    glClearColor( 0.0, 0.5, 1.0, 1.0 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(this->trafo));
    if (this->draw) {
        glColor3f(1.0f,1.0f,0.0f);
        glBegin(GL_TRIANGLES);
            glVertex2f(-0.5f, -0.5f);
            glVertex2f(0.5f, -0.5f);
            glVertex2f(0.0f,0.5f);
        glEnd();
    }

    return false;
}

