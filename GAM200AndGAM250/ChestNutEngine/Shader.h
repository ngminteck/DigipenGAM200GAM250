/******************************************************************************/
/*!
\file   Shader.h

\author Wong Zhi Jun(100%)
\par    email: w.zhijun\@digipen.edu
\par    DigiPen login: w.zhijun

\brief
This file contains function and stucts declaration for Shader

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/

#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "GraphicsSystem.h"

class Shader
{
private:
	//Member variables
	unsigned int m_id;
	//Private member Functions
	std::string		ParseShaderSource(const char* fileName);
	unsigned int	CompileShader(GLenum type, const char* fileName);
	void			LinkProgram(unsigned vertexShader, unsigned fragmentShader);

public:
	Shader() {}
	Shader(const char* vertexFile, const char* fragmentFile);
	~Shader();

	unsigned int GetShaderID();
	void Bind();
	void Unbind();

	void set1i(GLint value, const GLchar* name);
	void set1f(GLfloat value, const GLchar* name);
	void setVec2i(glm::ivec2 value, const GLchar* name);
	void setVec2f(glm::fvec2 value, const GLchar* name);
	void setVec3f(glm::fvec3 value, const GLchar* name);
	void setVec4f(glm::fvec4 value, const GLchar* name);
	void setMat3fv(glm::mat3 value, const GLchar* name, GLboolean transpose = GL_FALSE);
	void setMat4fv(glm::mat4 &value, const GLchar* name, GLboolean transpose = GL_FALSE);
};

