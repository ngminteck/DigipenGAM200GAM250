#version 440

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;
layout (location = 2) in vec2 vertex_texcoord;

out vec3 position;
out vec3 color;
out vec2 texcoord;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

uniform float currFrame;

void main(void)
{
	position = vec4(ModelMatrix * vec4(vertex_position, 1.f)).xyz;
	color = vertex_color;
	texcoord = vec2(vertex_texcoord.x+ currFrame, vertex_texcoord.y*-1);
	//texcoord = vec2((vertex_texcoord.x+currFrame.x)/maxFrame.x,
	//				-1.f*(vertex_texcoord.y + currFrame.y) / maxFrame.y);
	//texcoord = vec2((vertex_texcoord.x + currFrame), -1*vertex_texcoord.y);
	gl_Position =  ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(vertex_position, 1.f);
}