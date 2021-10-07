/******************************************************************************/
/*!
\file   Shader.cpp

\author Wong Zhi Jun(100%)
\par    email: w.zhijun\@digipen.edu
\par    DigiPen login: w.zhijun

\brief
This file contains function implementation for Shader class

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "Shader.h"

Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
	unsigned vertexShader=0;
	unsigned fragShader=0;

	vertexShader = CompileShader(GL_VERTEX_SHADER, vertexFile);		// Load all shader code
	fragShader	 = CompileShader(GL_FRAGMENT_SHADER, fragmentFile);

	LinkProgram(vertexShader, fragShader);							// Link shader codes to program
	glDeleteShader(vertexShader);									// Delete shader
	glDeleteShader(fragShader);
}


Shader::~Shader()
{
	glDeleteProgram(m_id);
}


std::string Shader::ParseShaderSource(const char* fileName)
{
	std::string tmp{};
	std::string src{};
	std::ifstream in_file;

	in_file.open(fileName);					// Open shader file
	if (in_file.is_open())					// Retrieve source code
		while (std::getline(in_file, tmp))
			src += tmp + "\n";
	else									// Print debug message if failed to open file
		std::cout << "ERROR::SHADER::COULD_NOT_OPEN_FILE: " << fileName << "\n";
	in_file.close();
	return src;
}
unsigned Shader::CompileShader(GLenum type, const char* fileName)
{
	char infoLog[512];
	int result= 0;

	GLuint shader = glCreateShader(type);				// Create shader
	std::string str_src = ParseShaderSource(fileName);	// Parse shader source code
	const char* src = str_src.c_str();

	glShaderSource(shader, 1, &src, NULL);				// Input the shader source code
	glCompileShader(shader);							// Compile the shader source code
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);	// Get compile status

	if (!result)										// Print debug message if linking failed
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COULD_NOT_COMPILE_SHADER: " << fileName << "\n" << infoLog << "\n";
	}
	return shader;
}
void Shader::LinkProgram(unsigned vertexShader, unsigned fragShader)
{
	char infoLog[512];									// For debug message
	int result;											// For debug status

	m_id = glCreateProgram();								// Create program

	glAttachShader(m_id, vertexShader);					// Attach shaders to program
	glAttachShader(m_id, fragShader);
	
	glLinkProgram(m_id);									// Link Program
	glValidateProgram(m_id);								// Check if program can execute in current opengl state
	glGetProgramiv(m_id, GL_LINK_STATUS, &result);		// Get link status

	if (!result)										// Print debug message if linking failed
	{
		glGetProgramInfoLog(m_id, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COULD_NOT_LINK_PROGRAM" << "\n" << infoLog << "\n";
	}
	glUseProgram(0);
}

unsigned int Shader::GetShaderID()
{
	return m_id;
}
void Shader::Bind()
{
	glUseProgram(m_id);
}

void Shader::Unbind()
{
	glUseProgram(0);
}

void Shader::set1i(GLint value, const GLchar* name)
{
	glUniform1i(glGetUniformLocation(m_id, name), value);
}

void Shader::set1f(GLfloat value, const GLchar* name)
{
	glUniform1f(glGetUniformLocation(m_id, name), value);
}
void Shader::setVec2i(glm::ivec2 value, const GLchar* name)
{
	glUniform2iv(glGetUniformLocation(m_id, name), 1, glm::value_ptr(value));
}
void Shader::setVec2f(glm::fvec2 value, const GLchar* name)
{
	glUniform2fv(glGetUniformLocation(m_id, name), 1, glm::value_ptr(value));
}

void Shader::setVec3f(glm::fvec3 value, const GLchar* name)
{
	glUniform3fv(glGetUniformLocation(m_id, name), 1, glm::value_ptr(value));
}

void Shader::setVec4f(glm::fvec4 value, const GLchar* name)
{
	glUniform4fv(glGetUniformLocation(m_id, name), 1, glm::value_ptr(value));
}

void Shader::setMat3fv(glm::mat3 value, const GLchar* name, GLboolean transpose)
{
	glUniformMatrix3fv(glGetUniformLocation(m_id, name), 1, transpose, glm::value_ptr(value));
}

void Shader::setMat4fv(glm::mat4 &value, const GLchar* name, GLboolean transpose)
{
	glUniformMatrix4fv(glGetUniformLocation(m_id, name), 1, transpose, glm::value_ptr(value));
}