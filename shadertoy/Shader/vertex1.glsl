#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;
out vec3 pix_pos;


void main()
{
	gl_Position = vec4(aPos,1.0);
	pix_pos = gl_Position.xyz;
	ourColor = aColor;
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}