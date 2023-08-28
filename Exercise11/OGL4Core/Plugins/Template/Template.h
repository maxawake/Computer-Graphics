#include "RenderPlugin.h"
#include "glm.hpp"

// This class is exported from the [Template].dll
class OGL4COREPLUGIN_API [Template] : public RenderPlugin {
public:
    [Template](COGL4CoreAPI *Api);
    ~[Template](void);

    virtual bool Activate(void);
    virtual bool Deactivate(void);
    virtual bool Init(void);
    virtual bool Render(void);

private:
    APIVar<[Template], BoolVarPolicy> draw;
    glm::mat4 trafo;
};

extern "C" OGL4COREPLUGIN_API RenderPlugin* OGL4COREPLUGIN_CALL CreateInstance(COGL4CoreAPI *Api) {
    return new [Template](Api);
}
