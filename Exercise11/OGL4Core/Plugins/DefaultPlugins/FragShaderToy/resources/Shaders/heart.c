//
// Created by inigo quilez - iq/2013
// License Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.
//
// taken from https://www.shadertoy.com/ and adapted
//

#version 330

uniform vec2 resolution;
uniform float time;

layout(location = 0) out vec4 frag_color;

void main(void)
{
    vec2 p = (2.0*gl_FragCoord.xy-resolution)/resolution.y;
    p.y -= 0.25;

    // animate
    float tt = mod(time,2.0)/2.0;
    float ss = pow(tt,.2)*0.5 + 0.5;
    ss -= ss*0.2*sin(tt*6.2831*5.0)*exp(-tt*6.0);
    p *= vec2(0.5,1.5) + ss*vec2(0.5,-0.5);

    
    float a = atan(p.x,p.y)/3.141593;
    float r = length(p);

    // shape
    float h = abs(a);
    float d = (13.0*h - 22.0*h*h + 10.0*h*h*h)/(6.0-5.0*h);

    // color
    float f = step(r,d) * pow(1.0-r/d,0.25);

    frag_color = vec4(f,0.0,0.0,1.0);
}
    
