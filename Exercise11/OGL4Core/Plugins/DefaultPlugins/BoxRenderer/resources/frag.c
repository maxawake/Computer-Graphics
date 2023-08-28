#version 330

uniform sampler2D tex;
uniform vec3 light;
uniform vec3 Ka;
uniform vec3 Kd;
uniform int  useTexs;
uniform int  useLambert;
uniform int  showWhat;

layout(location = 0) out vec4 frag_color;

in vec4 mypos;
in vec3 localPos;
in vec3 normDir;
in vec2 texCoords;

void main() {
    float lambert = max(dot(normalize(mypos.xyz-localPos),normDir),0);
    if (useLambert==0) {
        lambert = 1.0;
    }
    frag_color = vec4(light*Kd,1);
   
    if (showWhat==1) {
        if (useTexs==1) {
            frag_color = vec4(light*(Ka+Kd*lambert),1)*texture2D(tex,texCoords);
        } else {
            frag_color = vec4(light*(Ka+Kd*lambert),1);
        }
    }
}
