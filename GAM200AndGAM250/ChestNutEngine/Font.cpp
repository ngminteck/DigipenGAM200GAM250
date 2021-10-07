/******************************************************************************/
/*!
\file   Font.cpp

\author Wong Zhi Jun(100%)
\par    email: w.zhijun\@digipen.edu
\par    DigiPen login: w.zhijun

\brief
This file contains function and stucts declaration for Font

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/

#include "Font.h"
#include <algorithm>

FontTextureAtlas::FontTextureAtlas(const char * path, unsigned pixelsize)
	:m_Width{0},m_Height{0}
{
	if (FT_Init_FreeType(&m_FTLib))
	{
		std::cout << "ERROR::FREETYPE FAILED TO LOAD FREETYPELIBRARY\n";
		return;
	}
	if (FT_New_Face(m_FTLib, path, 0, &m_Face))
	{
		std::cout << "ERROR::FREETYPE FAILED TO OPEN FONT\n";
		return;
	}
	FT_Set_Pixel_Sizes(m_Face, 0, pixelsize);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (GLubyte c = 0; c < 128; c++)
	{                                                  
		// Load character glyph 
		if (FT_Load_Char(m_Face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			m_Face->glyph->bitmap.width,
			m_Face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			m_Face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character = {
			texture,
			glm::ivec2(m_Face->glyph->bitmap.width, m_Face->glyph->bitmap.rows),
			glm::ivec2(m_Face->glyph->bitmap_left, m_Face->glyph->bitmap_top),
			(GLuint)m_Face->glyph->advance.x
				};
		characters.insert(std::pair<GLchar, Character>(c, character));
	}

	//FT_GlyphSlot glyph = m_Face->glyph;
	//for (unsigned char c = 32; c < 128; c++)
	//{
	//	if (FT_Load_Char(m_Face, c, FT_LOAD_RENDER))
	//	{
	//		std::cout << "ERROR::FREETYPE FAILED TO LOAD GLYPH\n";
	//		continue;
	//	}
	//	m_Width += glyph->bitmap.width;
	//	m_Height = std::max(m_Height, glyph->bitmap.rows);
	//}
	//glActiveTexture(GL_TEXTURE0);
	//glGenTextures(1, &m_FontAtlas);
	//glBindTexture(GL_TEXTURE_2D, m_FontAtlas);
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_Width, m_Height,		//Create empty texture
	//			0, GL_ALPHA, GL_UNSIGNED_BYTE, 0);
	//int x = 0;
	//for (unsigned char c = 32; c < 128; c++)
	//{
	//	if (FT_Load_Char(m_Face, c, FT_LOAD_RENDER))
	//	{
	//		std::cout << "ERROR::FREETYPE FAILED TO LOAD GLYPH\n";
	//		continue;
	//	}
	//	glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, glyph->bitmap.width,		//Fill glyphs in empty texture
	//					glyph->bitmap.rows, GL_ALPHA, GL_UNSIGNED_BYTE, glyph->bitmap.buffer);
	//	x += glyph->bitmap.width;
	//	character[c].advancex	= glyph->advance.x >> 6;
	//	character[c].advancey	= glyph->advance.y >> 6;
	//	character[c].width		= glyph->bitmap.width;
	//	character[c].height		= glyph->bitmap.rows;
	//	character[c].left		= glyph->bitmap_left;
	//	character[c].top		= glyph->bitmap_top;
	//	character[c].xoffset = (float)x / m_Width;
	//}

	glBindTexture(GL_TEXTURE_2D, 0);
	FT_Done_Face(m_Face);
	FT_Done_FreeType(m_FTLib);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

FontTextureAtlas::~FontTextureAtlas()
{
	//glDeleteTextures(1, &m_FontAtlas);
}

//void FontTextureAtlas::RenderText(const char *text, float x, float y, float sx, float sy) 
//{
//	std::vector<point> coords;
//
//	for (const char *p = text; *p; p++) {
//		float x2 = x + character[*p].left* sx;
//		float y2 = -y - character[*p].top * sy;
//		float w = character[*p].width * sx;
//		float h = character[*p].height * sy;
//
//		/* Advance the cursor to the start of the next character */
//		x += character[*p].advancex * sx;
//		y += character[*p].advancey * sy;
//
//		/* Skip glyphs that have no pixels */
//		if (!w || !h)
//			continue;
//
//		coords.push_back(point { x2		, -y2		, character[*p].xoffset, 0 });
//		coords.push_back(point { x2 + w	, -y2		, character[*p].xoffset + character[*p].width / m_Width, 0 });
//		coords.push_back(point { x2		, -y2 - h	, character[*p].xoffset, character[*p].height / m_Height }); //remember: each glyph occupies a different amount of vertical space
//		coords.push_back(point { x2 + w	, -y2		, character[*p].xoffset + character[*p].width / m_Width, 0 });
//		coords.push_back(point { x2		, -y2 - h	, character[*p].xoffset	, character[*p].height / m_Height });
//		coords.push_back(point { x2 + w	, -y2 - h	, character[*p].xoffset + character[*p].width / m_Width, character[*p].height / m_Height});
//	}
//	glEnableClientState(GL_VERTEX_ARRAY);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	//glVertexPointer(3, GL_FLOAT, 0, 0);
//
//	glBufferData(GL_ARRAY_BUFFER, sizeof(coords), &coords[0], GL_DYNAMIC_DRAW);
//	glDrawArrays(GL_TRIANGLES, 0, coords.size());
//	std::cout << glGetError() << "\n";
//}

void FontTextureAtlas::RenderText(Shader *shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
	// Activate corresponding render state	
	shader->Bind();
	glUniform3f(glGetUniformLocation(shader->GetShaderID(), "textColor"), color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);
	// Iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = characters[*c];

		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}