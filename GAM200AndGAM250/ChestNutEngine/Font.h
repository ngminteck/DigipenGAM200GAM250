/******************************************************************************/
/*!
\file   Font.h

\author Wong Zhi Jun(100%)
\par    email: w.zhijun\@digipen.edu
\par    DigiPen login: w.zhijun

\brief
This file contains function and stucts declaration for Font

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H 
#include <iostream>
#include "GraphicsSystem.h"
#include "Shader.h"
struct point {
	GLfloat x;
	GLfloat y;
	GLfloat s;
	GLfloat t;
};

struct Character {
	GLuint TextureID;   // ID handle of the glyph texture
	glm::ivec2 Size;    // Size of glyph
	glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
	GLuint Advance;    // Horizontal offset to advance to next glyph
};
struct FontCharacter
{
	float advancex;
	float advancey;
	float width;
	float height;
	float left;
	float top;
	float xoffset;
};

class FontTextureAtlas
{
private:
	GLuint m_FontAtlas;

	GLuint VAO;
	GLuint VBO;

	FT_Library m_FTLib;
	FT_Face m_Face;

	unsigned m_Width;
	unsigned m_Height;


	std::map<GLchar, Character> characters;
	//Character character[128];
public:
	FontTextureAtlas(const char * path, unsigned pixelsize);

	~FontTextureAtlas();

	void RenderText(Shader *shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
	//void RenderText(const char *text, float x, float y, float sx, float sy);
};