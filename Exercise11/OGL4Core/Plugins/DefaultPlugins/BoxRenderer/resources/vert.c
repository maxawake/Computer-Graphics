#version 330

layout(location = 0) in vec4  in_position;
layout(location = 1) in vec3  in_normal;
layout(location = 2) in vec2  in_texcoord;

uniform mat4 model_matrix;
uniform mat4 inv_view_matrix;
uniform mat4 view_matrix;
uniform mat4 proj_matrix;
uniform vec3 translate;
uniform float scale;

out vec4 mypos;
out vec3 localPos;
out vec3 normDir;
out vec2 texCoords;

void main()
{
    vec4 vert = in_position - vec4(translate,0);
    vert.xyz *= scale;
    
    gl_Position = proj_matrix * view_matrix * model_matrix * vert;   
    
    normDir   = normalize((model_matrix*vec4(in_normal,0)).xyz);
    localPos  = (model_matrix * vert).xyz;
    texCoords = in_texcoord;

    mypos = inv_view_matrix * vec4(0,0,0,1);
}
