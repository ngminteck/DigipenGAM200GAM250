/******************************************************************************/
/*!
\file   Texture2D.cpp

\author Wong Zhi Jun(100%)
\par    email: w.zhijun\@digipen.edu
\par    DigiPen login: w.zhijun

\brief
This file contains function and stucts implementation for Texture2D

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#define _CRT_SECURE_NO_WARNINGS
#include "Texture2D.h"

Texture2D::Texture2D()
:DataFileInfo()
{

}

Texture2D::Texture2D(std::string path, std::string filename, const char * dds_path, const char *sprite_name, bool animation_texture)
: DataFileInfo(path, filename), m_textureID{ 0 }, m_width{ 0 }, m_height{ 0 }, m_texName{ sprite_name }, m_animated_texture{ animation_texture }
{
	LoadDDS(dds_path);
}

Texture2D::Texture2D(const char * texFileName, const char * name)
	:m_textureID{0}, m_width{0}, m_height{0}, m_texName{name}
{
	LoadDDS(texFileName);
}
Texture2D::~Texture2D()
{
	for (auto& it : m_animationList)
	{
		delete it.second;
		it.second = nullptr;
	}
	glDeleteTextures(1, &m_textureID);
}

std::string Texture2D::GetTextureName() const
{
	return m_texName;
}

bool Texture2D::GetIsAnimatedTexture() const
{
	return m_animated_texture;
}

//void Texture2D::LoadTexture(const char* texFileName)
//{
//#if defined(DEBUG) |defined(_DEBUG)	
//	std::cout << "Loading : " << texFileName << "\n";
//#endif
//	//Load image data
//	unsigned char* image = SOIL_load_image(texFileName, &m_width, &m_height, 0, SOIL_LOAD_RGBA);
//	
//	// Generate and bind texture
//	glGenTextures(1, &m_textureID);
//	glBindTexture(GL_TEXTURE_2D, m_textureID);
//	// Set texture parameters
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//
//	// Define texture image
//	if (image)
//	{
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
//		glGenerateMipmap(GL_TEXTURE_2D);
//	}
//#if defined(DEBUG) |defined(_DEBUG)	
//	else
//		std::cout << "ERROR::TEXTURE_LOAD_FAILED\n";
//	std::cout << SOIL_last_result() << "\n\n";
//#endif
//	// Unbind texture
//	glBindTexture(GL_TEXTURE_2D, 0);
//	// Free loaded data
//	SOIL_free_image_data(image);
//}

void Texture2D::LoadDDS(const char* texFileName)
{
	const unsigned long FOURCC_DXT1 = 0x31545844; //(MAKEFOURCC('D','X','T','1'))
	const unsigned long FOURCC_DXT3 = 0x33545844; //(MAKEFOURCC('D','X','T','3'))
	const unsigned long FOURCC_DXT5 = 0x35545844; //(MAKEFOURCC('D','X','T','5'))
    unsigned char header[124];

    FILE *fp;

    /* try to open the file */
    fp = fopen(texFileName, "rb");
    if (fp == NULL)
        return;

    /* verify the type of file */
    char filecode[4];
    fread(filecode, 1, 4, fp);
    if (strncmp(filecode, "DDS ", 4) != 0) {
        fclose(fp);
        return;
    }

    /* get the surface desc */
    fread(&header, 124, 1, fp); 

    unsigned int height = m_height		= *(unsigned int*)&(header[8 ]);
    unsigned int width  = m_width       = *(unsigned int*)&(header[12]);
    unsigned int linearSize				= *(unsigned int*)&(header[16]);
    unsigned int mipMapCount			= *(unsigned int*)&(header[24]);
    unsigned int fourCC					= *(unsigned int*)&(header[80]);

	unsigned char * image;
	unsigned int bufsize;
	/* how big is it going to be including all mipmaps? */
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	image = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
	fread(image, 1, bufsize, fp);
	/* close the file pointer */
	fclose(fp);

	unsigned int format;
	switch (fourCC)
	{
	case FOURCC_DXT1:
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;
	case FOURCC_DXT3:
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;
	case FOURCC_DXT5:
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;
	default:
		free(image);
		return;
	}
	glGenTextures(1, &m_textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	/* load the mipmaps */
	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
	{
		unsigned int size = ((width + 3) / 4)*((height + 3) / 4)*blockSize;
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
			0, size, image + offset);

		offset += size;
		width = std::max(width/2,static_cast<unsigned>(1));
		height = std::max(height/2,static_cast<unsigned>(1));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	free(image);
	return;
}

GLuint Texture2D::GetTextureID()const
{
	return m_textureID;
}

void Texture2D::Bind()
{
	glBindTexture(GL_TEXTURE_2D, m_textureID);
}

void Texture2D::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

int Texture2D::GetWidth()
{
	return m_width;
}
int Texture2D::GetHeight()
{
	return m_height;
}

void Texture2D::AddAnimation(Animation*anim, std::string aniname)
{
	m_animationList[aniname] = anim;
}
Mesh* Texture2D::GetAnimationMesh(std::string aniname)
{
	if(m_animationList.find(aniname) != m_animationList.end())
		return m_animationList[aniname]->GetMesh();
	return nullptr;
}

Animation* Texture2D::GetAnimationData(std::string aniname)
{
	if(m_animationList.find(aniname)!= m_animationList.end())
		return m_animationList[aniname];
	return nullptr;
}

std::unordered_map<std::string, Animation*> & Texture2D::GetAnimationListContainer()
{
	return m_animationList;
}