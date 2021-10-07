/******************************************************************************/
/*!
\file   SpriteSheetFactory.h

\author Ng Min Teck(50%)
\par    email: minteck.ng\@digipen.edu
\par    DigiPen login: minteck.ng

\author Wong Zhi Jun(50%)
\par    email: w.zhijun\@digipen.edu
\par    DigiPen login: w.zhijun

\brief
	the file loading for spritesheet and also tool to generate json for texture

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#pragma once
#include "Texture2D.h"

struct SpriteSheetstring
{
	std::string sprite_name;
	std::string texture_name;
};

class SpriteSheetFactory
{
public:
	SpriteSheetFactory();
	~SpriteSheetFactory();
	void ScanSpriteSheetFromDirectory();
	void ScanSpriteSheetFromDirectory(std::string filepath);
	void LoadSpriteSheetrFromJsonFile(const char * filepath);
	std::unordered_map<unsigned, Texture2D *>*GetContainer() { return &SpriteSheet_container; }

	void ScanNoAnimatedTextureAndOutputToJson();
	void ScanNoAnimatedTextureAndOutputToJson(std::string filepath);
	void GenerateTexturePNGToJson(std::string filepath);

private:
	unsigned SpriteSheet_container_key;
	std::unordered_map<unsigned, Texture2D *> SpriteSheet_container;
};

unsigned GetSpriteKeyBySpriteName(std::unordered_map<unsigned, Texture2D *> * container, std::string search);