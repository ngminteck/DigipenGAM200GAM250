/******************************************************************************/
/*!
\file   Mesh.cpp

\author Wong Zhi Jun(100%)
\par    email: w.zhijun\@digipen.edu
\par    DigiPen login: w.zhijun

\brief
This file contains function implementation for Mesh class

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/

#include "Mesh.h"
#include "Texture2D.h"
#include "Shader.h"

Mesh::Mesh(Vertex* vertexArray, const unsigned  vertexCount,
			unsigned* indexArray, const unsigned  indexCount)
	: m_VAO{ 0 }, m_VBO{ 0 }, m_EBO{ 0 },m_ModelMatrix{1.f}
{
	// Store vertex data
	for (size_t i = 0; i < vertexCount; ++i)
	{
		m_Vertices.push_back(vertexArray[i]);
	}
	for (size_t i = 0; i < indexCount; ++i)
		m_Indices.push_back(indexArray[i]);	

	Initialize();
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_EBO);
	glDeleteVertexArrays(1, &m_VAO);
}

void Mesh::Initialize()
{
	glGenVertexArrays(1, &m_VAO);				// Create VAO
	glBindVertexArray(m_VAO);					// Bind VAO

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);		// Bind VBO and Vertex data
	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), m_Vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);// Bind EBO and Indices data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), m_Indices.data(), GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);				 // Vertex Position  Location 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1);				 // Vertex Color	 Location 1
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
	glEnableVertexAttribArray(2);				 // Vertex Texcoord  Location 2
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));

	glBindVertexArray(0);						 // Unbind Vertex Array
}

void Mesh::Bind()
{
	glBindVertexArray(m_VAO);
}

void Mesh::Unbind()
{	
	glBindVertexArray(0);
}

void Mesh::Update(glm::vec3 trans, float rot, glm::vec3 scale)
{
	m_ModelMatrix = glm::mat4(1.0);
	// T * R * S
	m_ModelMatrix = glm::translate(m_ModelMatrix, glm::vec3(trans.x,trans.y ,trans.z));
	m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(rot), glm::vec3(0.0f, 0.0f, 1.0f));
	m_ModelMatrix = glm::scale(m_ModelMatrix, scale);
}
void Mesh::Update(Matrix4x4 mat4)
{
	m_ModelMatrix = { mat4[0],mat4[1],mat4[2],mat4[3],
					  mat4[4],mat4[5],mat4[6],mat4[7],
					  mat4[8],mat4[9],mat4[10],mat4[11],
					  mat4[12],mat4[13],mat4[14],mat4[15] };
}