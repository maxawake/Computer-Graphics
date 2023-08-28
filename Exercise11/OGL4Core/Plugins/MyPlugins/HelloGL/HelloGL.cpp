// HelloGL.cpp 
//

#include "GLHelpers.h"
#include "stdafx.h"
#include "HelloGL.h"
#include "gl3w.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <cstdlib>
#define _USE_MATH_DEFINES
#include <math.h>

HelloGL::HelloGL(COGL4CoreAPI *Api) : RenderPlugin(Api) {
  this->myName = "MyPlugins/HelloGL";
  this->myDescription = "Renders some basic geometry";
  va_tree = 0;
}

HelloGL::~HelloGL() {
}

void GenerateTreeMesh(std::vector<glm::vec4> &verts_tree, 
		      std::vector<glm::vec4> &colors_tree, 
		      unsigned int &numVerts_tree, 
		      std::vector<unsigned int> &indices_tree, 
		      unsigned int &numIndices_tree)
{
  verts_tree.resize(0);
  colors_tree.resize(0);
  indices_tree.resize(0);
  numVerts_tree = 0;
  numIndices_tree = 0;

  // exercise 11.1b) ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // TODO:
  // Generiere Vertices für einen Baum; der Baum soll einen viereckigen Stamm 
  // und eine Krone haben, die aus einem oder mehreren Dreiecken gebaut ist. Der 
  // Stamm hat braune Farbe, die Krone grüne. Der untere Mittelpunkt vom 
  // Stamm ist der Ursprung des Koordinatensystems für den Baum. Er soll ca.
  // 1 bis 1.5 Einheiten hoch sein.
  // TODO: Generate vertices for a tree; the tree should consist of a rectangular
  // trunk, and a crown consisting of a/multiple triangle/s.
  // The color of the trunk should be brown-ish, the crown green.
  // The center at the bottom of the tree should be the origin of the coordinate system,
  // with the height of the tree being 1 to 1.5 units.

  verts_tree.push_back(glm::vec4(-0.25f,0.0f,0.0f,1.0f));
  verts_tree.push_back(glm::vec4( 0.25f,0.0f,0.0f,1.0f));
  verts_tree.push_back(glm::vec4( -0.25f, 0.5f, 0.0f,1.0f));
  verts_tree.push_back(glm::vec4( 0.25f, 0.5f, 0.0f,1.0f));

  verts_tree.push_back(glm::vec4( -0.5f,0.5f,0.0f,1.0f));
  verts_tree.push_back(glm::vec4( 0.5f, 0.5f, 0.0f,1.0f));
  verts_tree.push_back(glm::vec4( 0.0f, 1.5f, 0.0f,1.0f));


  colors_tree.push_back(glm::vec4(0.388f,0.212f,0.094f,1.0f));
  colors_tree.push_back(glm::vec4(0.329f,0.145f,0.0f,1.0f));
  colors_tree.push_back(glm::vec4(0.388f,0.212f,0.094f,1.0f));
  colors_tree.push_back(glm::vec4(0.329f,0.145f,0.0f,1.0f));

  colors_tree.push_back(glm::vec4(0.286f,0.522f,0.126f,1.0f));
  colors_tree.push_back(glm::vec4(0.286f,0.522f,0.126f,1.0f));
  colors_tree.push_back(glm::vec4(0.431f,0.777f,0.196f,1.0f));

  indices_tree.push_back(0);
  indices_tree.push_back(1);
  indices_tree.push_back(2);
  indices_tree.push_back(1);
  indices_tree.push_back(2);
  indices_tree.push_back(3);

  indices_tree.push_back(4);
  indices_tree.push_back(5);
  indices_tree.push_back(6);
  

  numVerts_tree = verts_tree.size();
  numIndices_tree = indices_tree.size();

  //~exercise  11.1b) ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

}

void GenerateGroundMesh(std::vector<glm::vec4> &verts_ground, 
		      std::vector<glm::vec4> &colors_ground, 
		      unsigned int &numVerts_ground, 
		      std::vector<unsigned int> &indices_ground, 
		      unsigned int &numIndices_ground)
{
  verts_ground.resize(0);
  colors_ground.resize(0);
  indices_ground.resize(0);

  verts_ground.push_back(glm::vec4(-5.0f,0.0f,-5.0f,1.0f));
  verts_ground.push_back(glm::vec4( 5.0f,0.0f,-5.0f,1.0f));
  verts_ground.push_back(glm::vec4( 5.0f,0.0f, 5.0f,1.0f));
  verts_ground.push_back(glm::vec4(-5.0f,0.0f, 5.0f,1.0f));

  colors_ground.push_back(glm::vec4(0.7f,0.7f,0.7f,1.0f));
  colors_ground.push_back(glm::vec4(0.7f,0.7f,0.7f,1.0f));
  colors_ground.push_back(glm::vec4(0.7f,0.7f,0.7f,1.0f));
  colors_ground.push_back(glm::vec4(0.7f,0.7f,0.7f,1.0f));

  indices_ground.push_back(0);
  indices_ground.push_back(2);
  indices_ground.push_back(1);
  indices_ground.push_back(2);
  indices_ground.push_back(0);
  indices_ground.push_back(3);

  numVerts_ground = verts_ground.size();
  numIndices_ground = indices_ground.size();
}

const std::string LoadShaderText(const char* filename)
{
  std::cout <<  filename << std::endl;
  std::string shaderText;
  std::ifstream file(filename);  
  std::string line;

  if (file.good()) {
    // read the shader file line by line
    while (getline(file, line)) {
      shaderText += line;
      shaderText += '\n';
    }
  }

  return shaderText;
}

GLuint HelloGL::CreateShaderProgram(const char *vertShaderFileName,
				    const char *fragShaderFileName)
{
  GLuint vertexShader;
  GLuint fragmentShader;
  GLuint program;

  const std::string vertexShaderText = LoadShaderText(vertShaderFileName);
  const std::string fragmentShaderText = LoadShaderText(fragShaderFileName);

  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  program = glCreateProgram();

  const GLchar *vertexShaderSources[1] = {vertexShaderText.c_str()};
  const GLchar *fragmentShaderSources[1] = {fragmentShaderText.c_str()};

  glShaderSource(vertexShader, 1, vertexShaderSources, NULL);
  glShaderSource(fragmentShader, 1, fragmentShaderSources, NULL);

  GLint success;
  const unsigned int MAX_INFO_LOG_SIZE = 2048;

  // Compile vertex shader and check if succeeded
  glCompileShader(vertexShader);
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    GLchar infoLog[MAX_INFO_LOG_SIZE];
    glGetShaderInfoLog(vertexShader, MAX_INFO_LOG_SIZE, NULL, infoLog);
    std::cerr << "Error in vertex shader compilation. Info log:" << std::endl;
    std::cerr << infoLog << std::endl;
    return 0;
  }
  // Compile fragment shader and check if succeeded
  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    GLchar infoLog[MAX_INFO_LOG_SIZE];
    glGetShaderInfoLog(fragmentShader, MAX_INFO_LOG_SIZE, NULL, infoLog);
    std::cerr << "Error in fragment shader compilation. Info log:" << std::endl;
    std::cerr << infoLog << std::endl;
    return 0;
  }

  // Attach shaders to program
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);

  // Link program and check if succeeded
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    GLchar infoLog[MAX_INFO_LOG_SIZE];
    glGetProgramInfoLog(program, MAX_INFO_LOG_SIZE, NULL, infoLog);
    std::cerr << "Error in program linkage. Info log:" << std::endl;
    std::cerr << infoLog << std::endl;
    return 0;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return program;
}

void HelloGL::GenerateGround()
{
  // holds vertex positions of the triangles the ground is made of
  std::vector<glm::vec4> verts_ground;
  // holds vertex indices; used to generate triangles from an array of vertices
  std::vector<unsigned int> indices_ground;
  // holds colors for the trunk and crown vertices
  std::vector<glm::vec4> colors_ground;
  
  // Generate geometry for the ground: traingle vertices, vertex indices
  GenerateGroundMesh(verts_ground, colors_ground, numVerts_ground,
		     indices_ground, numIndices_ground);
  
  // Generate buffers that will hold the vertex and index data
  glGenVertexArrays(1,&va_ground);
  glGenBuffers(1,&vbo_ground);
  glGenBuffers(1,&ibo_ground);
  glGenBuffers(1,&cbo_ground);
  
  // Bind vertex array; bind buffers to this array and load data 
  // from verts_ground and indices_ground
  glBindVertexArray(va_ground);
    glBindBuffer(GL_ARRAY_BUFFER,vbo_ground);
    // Enable vertex attribute array (in vertex shader accessible 
    // from layout (location = 0) in vec4 position;)
    glEnableVertexAttribArray(0);
    glBufferData(GL_ARRAY_BUFFER,sizeof(glm::vec4)*numVerts_ground, 
		 verts_ground.data(),GL_STATIC_DRAW);
    // Specify data format for this vertex attribute array
    glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,0,NULL);

    glBindBuffer(GL_ARRAY_BUFFER,cbo_ground);
    // Enable vertex attribute array
    glEnableVertexAttribArray(1);
    glBufferData(GL_ARRAY_BUFFER,sizeof(glm::vec4)*numVerts_ground, 
		 colors_ground.data(),GL_STATIC_DRAW);
    // Specify data format for this vertex attribute array
    glVertexAttribPointer(1,4,GL_FLOAT,GL_FALSE,0,NULL);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_ground);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*numIndices_ground, 
		 indices_ground.data(), GL_STATIC_DRAW);
  // Unbind vertex array
  glBindVertexArray(0);

  //-----------------------------------------
  // Create shader program for ground rendering
  std::string baseDir = this->GetCurrentPluginPath();
  std::string vertShaderFile = baseDir+"/resources/Shaders/ground-vertex.glsl";
  std::string fragShaderFile = baseDir+"/resources/Shaders/ground-fragment.glsl";

  shaderProgram_ground = CreateShaderProgram(vertShaderFile.c_str(), fragShaderFile.c_str());
}

void HelloGL::GenerateTreePositions()
{
  std::vector<glm::vec4> tree_positions;
  srand(0);

  for (int j = 0; j < 10; ++j) {
    for (int i = 0; i < 10; ++i) {
      float x = i-4.5f + (float(rand())/RAND_MAX - 0.5f);
      float y = 0.0f;
      float z = j-4.5f + (float(rand())/RAND_MAX - 0.5f);

      tree_positions.push_back(glm::vec4(x,y,z,0.0f));
    }
  }
  numTrees = tree_positions.size();

  if (va_tree == 0) {
    glGenVertexArrays(1,&va_tree);
  }
  glBindVertexArray(va_tree);
    // Instancing
    glGenBuffers(1, &vbo_tree_positions);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_tree_positions);
    glEnableVertexAttribArray(2);
    glBufferData(GL_ARRAY_BUFFER,sizeof(glm::vec4)*numTrees, tree_positions.data(),GL_STATIC_DRAW);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), NULL);
    glVertexAttribDivisor(2, 1);
  glBindVertexArray(0);
}

void HelloGL::GenerateTree()
{
  // holds vertex positions of the triangles the tree is made of
  std::vector<glm::vec4> verts_tree;
  // holds vertex indices; used to generate triangles from an array of vertices
  std::vector<unsigned int> indices_tree;
  // holds colors for the trunk and crown vertices
  std::vector<glm::vec4> colors_tree;

  // Generate geometry for the tree: traingle vertices, vertex indices
  GenerateTreeMesh(verts_tree, colors_tree, numVerts_tree,
		   indices_tree, numIndices_tree);

  //exercise  11.1b) ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // TODO:
  // Erstellen Sie die benötigten OpenGL Puffer fuer den Baum.
  // Orientieren Sie sich an der Funktion GenerateGround.
  // Als Handles werden die Variablen va_tree, vbo_tree, ibo_tree und cbo_tree
  // benutzt.
  // TODO: Create the necessary OpenGL buffer for the tree.
  // Look for help in the function GenerateGround. For handles, use the variables
  // va_tree, vbo_tree, ibo_tree,as well as cbo_tree.

  // Generate buffers that will hold the vertex and index data
  glGenVertexArrays(1,&va_tree);
  glGenBuffers(1,&vbo_tree);
  glGenBuffers(1,&ibo_tree);
  glGenBuffers(1,&cbo_tree);
  
  // Bind vertex array; bind buffers to this array and load data 
  // from verts_tree and indices_tree
  glBindVertexArray(va_tree);
    glBindBuffer(GL_ARRAY_BUFFER,vbo_tree);
    // Enable vertex attribute array (in vertex shader accessible 
    // from layout (location = 0) in vec4 position;)
    glEnableVertexAttribArray(0);
    glBufferData(GL_ARRAY_BUFFER,sizeof(glm::vec4)*numVerts_tree, 
		 verts_tree.data(),GL_STATIC_DRAW);
    // Specify data format for this vertex attribute array
    glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,0,NULL);

    glBindBuffer(GL_ARRAY_BUFFER,cbo_tree);
    // Enable vertex attribute array
    glEnableVertexAttribArray(1);
    glBufferData(GL_ARRAY_BUFFER,sizeof(glm::vec4)*numVerts_tree, 
		 colors_tree.data(),GL_STATIC_DRAW);
    // Specify data format for this vertex attribute array
    glVertexAttribPointer(1,4,GL_FLOAT,GL_FALSE,0,NULL);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_tree);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*numIndices_tree, 
		 indices_tree.data(), GL_STATIC_DRAW);
  // Unbind vertex array
  glBindVertexArray(0);

  //~exercise 11.1b) ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  //-----------------------------------------
  // Create shader program for tree rendering
  std::string baseDir = this->GetCurrentPluginPath();
  std::string vertShaderFile = baseDir+"/resources/Shaders/tree-vertex.glsl";
  std::string fragShaderFile = baseDir+"/resources/Shaders/tree-fragment.glsl";

  shaderProgram_tree = CreateShaderProgram(vertShaderFile.c_str(), fragShaderFile.c_str());
}

bool HelloGL::Activate(void) {

  GenerateGround();

  GenerateTree();

  GenerateTreePositions();

   // Initialize camera position: z = 3.0
  cameraPos = glm::vec4(0.0f, 0.5f, 3.0f, 1.0f);
  cameraRotX = cameraRotY = 0.0f;
  rotXMatrix = rotYMatrix = glm::mat4(1.0f);

  glEnable(GL_DEPTH_TEST);

  return true;
}

bool HelloGL::Deactivate(void) {

  glBindBuffer(GL_ARRAY_BUFFER,0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
  glDeleteBuffers(1,&vbo_tree);
  glDeleteBuffers(1,&ibo_tree);
  glDeleteVertexArrays(1,&va_tree);

  return true;
}

bool HelloGL::Init(void) {
  if (gl3wInit()) {
    printf("Cannot initialize gl3w! No function pointers!\n");
    return false;
  }
  if (!gl3wIsSupported(3, 2)) {
    std::cerr << "OpenGL 3.2 not supported" << std::endl;
    return false;
  }
  return true;
}

bool HelloGL::Keyboard(unsigned char key, int x, int y) 
{
  bool handled = false;
  switch(key) {
  case 'w': {
    cameraPos += rotYMatrix*glm::vec4(0.0f, 0.0f, -0.05f, 0.0f);
    return true;
  } 
  case 's': {
    cameraPos += rotYMatrix*glm::vec4(0.0f, 0.0f, 0.05f, 0.0f);
    return true;
  } 
  case 'a': {
    cameraPos += rotYMatrix*glm::vec4(-0.05f, 0.0f, 0.0f, 0.0f);
    return true;
  } 
  case 'd': {
    cameraPos += rotYMatrix*glm::vec4(0.05f, 0.0f, 0.0f, 0.0f);
    return true;
  } 
  }
  return false;
}

bool HelloGL::Mouse(int button, int state, int x, int y) {

  PostRedisplay();
  return false;
}

bool HelloGL::Motion(int x, int y) {

  if (IsLeftButtonPressed()) {

    int x_prev, y_prev;
    GetLastMousePos(x_prev, y_prev);
    int dx = x - x_prev;
    int dy = y - y_prev;

    cameraRotX += float(dy)*0.01f*M_PI/180.0f;
    cameraRotY += float(-dx)*0.01f*M_PI/180.0f;
  }

  PostRedisplay();

  return false;
}

void HelloGL::AssembleTransformMatrices()
{
  viewMX = glm::mat4(1.0f);
  const float initMatrix[16] = {1,0,0,-0,  0,1,0,0,  0,0,1,0,  0, -0.5,-3,1};
  viewMX = glm::make_mat4(initMatrix);

  // Aufgabe 11.1a) ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // TODO:
  // Erstellen Sie die View-Matrix aus den Variablen cameraRotX, cameraRotY 
  // und cameraPos. Hierzu sollten Sie die Rotations-Matrizen rot{YX}Maxtrix
  // verwenden. Beachten Sie die Reihenfolge der Multiplikation! 
  // Hinweis: OpenGL ist column-major, d.h. die Translation steht vor dem 
  // Hochladen auf die GPU in den Elementen mit den Indices 12,13,14.
  // TODO:.
  // Use the variables cameraRotX, cameraRotY, and cameraPos to create the 
  // view matrix. Here, use the rotation matrices rot{YX}Matrix in correct 
  // order. Note that OpenGL arrays are ordered column major wise so
  // translation entries are stored at indices 12, 13, 14.

  glm::mat4 translationMatrix = glm::translate(glm::mat4(), glm::vec3(-cameraPos));

  viewMX = translationMatrix * glm::mat4(1.0f);

  viewMX = glm::rotate(viewMX, (glm::mediump_float)cameraRotX*360.0f, glm::vec3(1.0f, 0.0f, 0.0f));
  viewMX = glm::rotate(viewMX, (glm::mediump_float)cameraRotY*360.0f, glm::vec3(0.0f, 1.0f, 0.0f));

  //~Aufgabe 11.1a) ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  // Set perspective projection
  int width, height;
  this->GetWindowSize(width,height);
  projMX = glm::perspective(45.0f, width/(float)height, 0.01f,100.0f);
}

void HelloGL::Transform(GLuint program)
{
  // Load transformation matrices to shader uniforms
  glUniformMatrix4fv(glGetUniformLocation(program, "proj_matrix"),
		     1,GL_FALSE,glm::value_ptr(projMX));
  glUniformMatrix4fv(glGetUniformLocation(program, "view_matrix"),
		     1,GL_FALSE,glm::value_ptr(viewMX));
  glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"),
		     1,GL_FALSE,glm::value_ptr(modelMX));
}

bool HelloGL::Render(void) {

  // set color used to clear the window
  glClearColor(0.0f, 0.5f, 1.0f, 1.0f);
  // clear color buffer and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Assemble the model, view and projection matrices
  AssembleTransformMatrices();

  // Specify shader program to be used for rendering
  glUseProgram(shaderProgram_tree);

  // Build transform matrices 
  Transform(shaderProgram_tree);

  if (numIndices_tree) {
      assert(va_tree);
      assert(vbo_tree);
      assert(ibo_tree);
      assert(cbo_tree);
  //Aufgabe 11.1c) ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // TODO:
  // Ändern Sie den Aufruf, um meherere Instanzen des Baums zu zeichnen.
  // TODO:
  // Change the following function call to draw multiple instances of a tree.
    glBindVertexArray(va_tree);
    glDrawElementsInstanced(GL_TRIANGLES, numIndices_tree, GL_UNSIGNED_INT, 0, numTrees);
    glBindVertexArray(0);
  //~Aufgabe 11.1c) ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  }

  glUseProgram(0);

  glUseProgram(shaderProgram_ground);

  // Build transform matrices 
  Transform(shaderProgram_ground);

  // Bind vertex array with tree mesh
  glBindVertexArray(va_ground);
  glDrawElements(GL_TRIANGLES, numIndices_ground, GL_UNSIGNED_INT, NULL);
  glBindVertexArray(0);

  glUseProgram(0);


  return false;
}

