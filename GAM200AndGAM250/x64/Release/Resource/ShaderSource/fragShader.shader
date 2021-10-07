#version 440

in vec2 texcoord;

out vec4 FragColor;

//Uniforms
uniform sampler2D texture0;

void main(void)
{
	FragColor = texture(texture0, texcoord);
}