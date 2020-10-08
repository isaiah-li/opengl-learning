#version 330 core
out vec4 fragColor;

in vec2 TexCoord;
in vec3 pix_pos;
// texture samplers
uniform sampler2D iChannel0;
uniform float iTime;
uniform vec3 iResolution;
uniform vec4 iDate;

// https://www.shadertoy.com/view/XdKXzy

#define PI 3.141592653589793238462643383

// from https://www.shadertoy.com/view/4s3XDn <3
float ln(vec2 p, vec2 a, vec2 b)
{
	vec2 pa = p - a;
	vec2 ba = b - a;
	float h = clamp(dot(pa, ba) / dot(ba, ba), 0.0, 1.0);
    return length(pa - ba * h);
}

// i think i should spend some time reading docs in order to minimize this.
// hints apreciated
// (Rotated LiNe)
float rln(vec2 uv, float start, float end, float perc) {
    float inp = perc * PI * 2.0;
	vec2 coord = vec2(sin(inp), cos(inp));
    return ln(uv, coord * start, coord * end);
}

// i need this to have an alphachannel in the output
// i intend to use an optimized version of this shader for a transparent desktop widget experiment
vec4 mixer(vec4 c1, vec4 c2) {
    // please tell me if you think this would boost performance.
    // the time i implemented mix myself it sure did reduce
    // the amount of operations but i'm not sure now
    // if (c2.a <= 0.0) return c1;
    // if (c2.a >= 1.0) return c2;
    return vec4(mix(c1.rgb, c2.rgb, c2.a), c1.a + c2.a);
    // in case you are curious how you could implement mix yourself:
    // return vec4(c2.rgb * c2.a + c1.rgb * (1.0-c2.a), c1.a+c2.a);
}
    
vec4 styleHandle(vec4 color, float px, float dist, vec3 handleColor, float width, float shadow) {
    if (dist <= width + shadow) {
        // lets draw the shadow
        color = mixer(color, vec4(0.0, 0.0, 0.0,
                                (1.0-pow(smoothstep(width, width + shadow, dist),0.2))*0.2));
        // now lets draw the antialiased handle
        color = mixer(color, vec4(handleColor, smoothstep(width, max(width - 3.0 * px, 0.0), dist)));
    }
    return color;
}

void main()
{
    vec2 R = iResolution.xy;
    // calculate the size of a pixel
    float px = 1.0 / R.y;
    // create percentages of the coordinate system
    // vec2 p = fragCoord.xy / R;
    // center the scene and add perspective
    //vec2 uv = (2.0 * fragCoord - R) / min(R.x, R.y);
	
    vec2 uv = pix_pos.xy;

    
    // lets scale the scene a bit down:
    uv *= 1.1;
    px *= 0.9;
    
    float width = 0.015;
    float dist = 1.0;
    float centerdist = length(uv);
    
    // static background to emulate the effect of the shaders alpha output channel
    vec4 color = vec4(1.0);
    float lines = abs(fract((uv.x*0.5-uv.y+fract(iDate.w*0.02))*5.0) -0.5);
    //color.rgb *= smoothstep(0.2, 0.3, lines) * 0.03 + 0.6;
    //color.rgb *= mod(uv.x-uv.y/*+fract(iDate.w*0.02)*/, 0.2) < 0.1 ? 0.5 : 0.48;
    
    
    
    float isRed = 1.0;
 
    
    
    if (centerdist < 1.0) {
        float time = (floor(iDate.w)+pow(fract(iDate.w),16.0));

        // hour
        color = styleHandle(color, px,
                            rln(uv, -0.05, 0.5, time / 3600.0 / 12.0),
                            vec3(0.0), 0.03, 0.02);

        // minute
        color = styleHandle(color, px,
                            rln(uv, -0.075, 0.7, time / 3600.0),
                            vec3(0.0), 0.02, 0.02);

        // second
        color = styleHandle(color, px,
                            min(rln(uv, -0.1, 0.9, time / 60.0), length(uv)-0.01),
                            vec3(1.0, 0.0, 0.0), 0.01, 0.02);
    }
    
    vec4 bg = vec4(1.0,1.0,0.0,1.0);
	color = mix(vec4(0.0),bg,color);
    fragColor = color;
}