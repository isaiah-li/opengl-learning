#version 330 core
out vec4 fragColor;

in vec2 TexCoord;
in vec3 pix_pos;
// texture samplers
uniform sampler2D iChannel0;
uniform float iTime;
uniform vec3 iResolution;


//https://www.shadertoy.com/view/lsBfDz

#define T texture(iChannel0,(s*p.zw+ceil(s*p.x))/2e2).y/(s+=s)*4.
void main(){
    vec4 p,d=vec4(.8,0,vec2(pix_pos.x,pix_pos.y*iResolution.y/iResolution.x)-.2),c=vec4(.6,.7,d);
    fragColor=c-d.w;
    for(float f,s,t=2e2+sin(dot(gl_FragCoord.xy,gl_FragCoord.xy));--t>0.;p=.05*t*d)
        p.xz+=iTime,
        s=2.,
        f=p.w+1.-T-T-T-T,
    	f<0.?fragColor+=(fragColor-1.-f*c.zyxw)*f*.4:fragColor;
}

