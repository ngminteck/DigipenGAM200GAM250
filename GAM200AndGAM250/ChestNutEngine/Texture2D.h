/******************************************************************************/
/*!
\file   Texture2D.h

\author Wong Zhi Jun(100%)
\par    email: w.zhijun\@digipen.edu
\par    DigiPen login: w.zhijun

\brief
This file contains function and stucts declaration for Texture2D

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#pragma once

#include "GL/glew.h"
//#include "SOIL.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include "Animation.h"

#include "DataFileInfo.h"

class Texture2D : public DataFileInfo
{
private:
	GLuint m_textureID;							// ID to texture
	int m_width;								// Texture width
	int m_height;								// Texture height
	std::string m_texName;
	bool m_animated_texture;

	std::unordered_map<std::string, Animation*> m_animationList;	//List of all the animation data
public:
	Texture2D();
	Texture2D(std::string path, std::string filename, const char * dds_path, const char *sprite_name, bool animation_texture);
	Texture2D(const char * texFileName, const char *name);	// Constructor 
	~Texture2D();								// Deconstructor

	//void LoadTexture(const char* texFileName);
	void LoadDDS(const char* texFileName);
	GLuint GetTextureID()const;

	void Bind();
	void Unbind();

	int GetWidth();
	int GetHeight();

	std::string GetTextureName() const;
	bool GetIsAnimatedTexture() const;

	void AddAnimation(Animation*anim ,std::string aniname);
	Mesh* GetAnimationMesh(std::string aniname);
	Animation* GetAnimationData(std::string aniname);

	std::unordered_map<std::string, Animation*> & GetAnimationListContainer();
};