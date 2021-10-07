/******************************************************************************/
/*!
\file   Mesh.h

\author Wong Zhi Jun(100%)
\par    email: w.zhijun\@digipen.edu
\par    DigiPen login: w.zhijun

\brief
This file contains function and stucts declaration for Mesh

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/

#pragma once
#include <iostream>
#include "GraphicsSystem.h"
#include "Matrix4x4.h"
#include "Vector2D.h"

class Mesh
{
private:
	GLuint m_VAO;		//	Vertex Array Object
	GLuint m_VBO;		//	Vertex Buffer Object
	GLuint m_EBO;		//	Element Buffer Object

	std::vector<Vertex> m_Vertices;
	std::vector<unsigned int> m_Indices;

	glm::mat4 m_ModelMatrix;

public:
	float x, y;

	Mesh(Vertex* vertexArray, const unsigned vertexCount,
		unsigned* indexArray, const unsigned indexCount);
	~Mesh();

	void Initialize();
	void Bind();
	void Unbind();
	void Update(glm::vec3 trans, float rot = 0.f, glm::vec3 scale = glm::vec3{ 1.f,1.f,0.f });
	void Update(Matrix4x4 mat4);

	glm::mat4& GetModelMatrix() { return m_ModelMatrix; }
	int GetIndexCount()const { return static_cast<int>(m_Indices.size()); }
};