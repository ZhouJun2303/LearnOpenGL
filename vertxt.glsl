#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
out vec3 ourPosition;
out vec2 TexCoord;
uniform vec3 offset;
uniform vec3 tempColor;

uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main()
{
	gl_Position = projection * view * model * vec4(offset.x + aPos.x, offset.y + aPos.y, aPos.z, 1.0);
	ourPosition = vec3(offset.x + aPos.x, offset.y + aPos.y, offset.z +  aPos.z);
	TexCoord = aTexCoord;
};