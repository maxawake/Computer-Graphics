#version 420

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 proj_matrix;

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;

layout (location = 0) out vec4 out_color;

void main () 
{
  gl_Position = proj_matrix*view_matrix*model_matrix*position;
  out_color = color;
}
