#include "RenderPlugin.h"
#include "glm.hpp"

// This class is exported from the HelloGL.dll
class OGL4COREPLUGIN_API HelloGL : public RenderPlugin {
 public:
  HelloGL(COGL4CoreAPI *Api);
  ~HelloGL(void);

  virtual bool Activate(void);
  virtual bool Deactivate(void);
  virtual bool Init(void);
  virtual bool Render(void);

  virtual bool Keyboard(unsigned char key, int x, int y);
  virtual bool Mouse(int button, int state, int x, int y);
  virtual bool Motion(int x, int y);

 private:

  GLuint CreateShaderProgram(const char *vertShaderFileName,
			     const char *fragShaderFileName);
  void Transform(GLuint program);
  void AssembleTransformMatrices();
  void GenerateTreePositions();
  
  void GenerateGround();
  void GenerateTree();

  glm::mat4 modelMX;
  glm::mat4 viewMX;
  glm::mat4 projMX;
  glm::vec4 cameraPos;
  float cameraRotX;
  float cameraRotY;
  glm::mat4 rotYMatrix;
  glm::mat4 rotXMatrix;

  GLuint va_tree;  // vertex array
  GLuint vbo_tree; // vertex buffer
  GLuint ibo_tree; // index buffer
  GLuint cbo_tree; // color buffer
  GLuint vbo_tree_positions; // instance position buffer

  unsigned int numVerts_tree;
  unsigned int numIndices_tree;
  unsigned int numTrees;

  GLuint va_ground;  // vertex array
  GLuint vbo_ground; // vertex buffer
  GLuint ibo_ground; // index buffer
  GLuint cbo_ground; // color buffer

  unsigned int numVerts_ground;
  unsigned int numIndices_ground;

  // shader program to render tree
  GLuint shaderProgram_tree;
  GLuint shaderProgram_ground;
};

extern "C" OGL4COREPLUGIN_API RenderPlugin* OGL4COREPLUGIN_CALL CreateInstance(COGL4CoreAPI *Api) {
  return new HelloGL(Api);
}
