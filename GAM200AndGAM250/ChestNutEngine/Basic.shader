#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
//out vec4 vertexColor;

void main()
{
	gl_Position = position;
	//vertexColor = vec4(0.5, 0.0, 0.0, 1.0);
};

#shader fragment
#version 330 core

out vec4 FragColor;
//in vec4 vertexColor;

void main()
{
	FragColor = vec4(1.0, 0.0, 1.0, 1.0);//vertexColor;
};