#include "RenderPlugin.h"
#include "glm.hpp"

// This class is exported from the MostSimple.dll
class OGL4COREPLUGIN_API MostSimple : public RenderPlugin {
public:
    MostSimple(COGL4CoreAPI *Api);
    ~MostSimple(void);

    virtual bool Activate(void);
    virtual bool Deactivate(void);
    virtual bool Init(void);
    virtual bool Render(void);
};

extern "C" OGL4COREPLUGIN_API RenderPlugin* OGL4COREPLUGIN_CALL CreateInstance(COGL4CoreAPI *Api) {
    return new MostSimple(Api);
}
