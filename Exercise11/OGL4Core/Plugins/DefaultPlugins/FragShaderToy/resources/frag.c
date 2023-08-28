#version 330

uniform sampler2D tex;
uniform int  useTexs;

layout(location = 0) out vec4 frag_color;

in vec2 texCoords;

void main() {
    vec4 color = vec4(0.4,0,0,1);
    color.rg = texCoords;
    frag_color = color;
}
