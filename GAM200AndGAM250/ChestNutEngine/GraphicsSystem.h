/******************************************************************************/
/*!
\file   Graphics.h

\author Wong Zhi Jun(100%)
\par    email: w.zhijun\@digipen.edu
\par    DigiPen login: w.zhijun

\brief
This file contains function and stucts declaration for Graphics

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#pragma once
#define NOMINMAX
#include <Windows.h>
#undef NOMINMAX
#include <vector>
#include <map>

#include "gl/glew.h"
#pragma warning(push)
#pragma warning(disable:4201)   // suppress even more warnings about nameless structs
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#pragma warning (pop)
#include "Utils.h"
#include "System.h"
#include "GraphicsContainer.h"


class Shader;
class Mesh;
class Texture2D;
class Camera;
class SceneActor;
class FontTextureAtlas;
class CameraComponent;

struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 texcoord;
};


class GraphicsSystem: public System
{
private:
	int	m_Height;
	int m_Width;
	float m_Aspect;

	glm::vec3 * default_cam_pos;
	glm::vec3 * default_cam_front;
	glm::vec3 * default_cam_up;

	float * default_CamOffsetPosX;
	float * default_CamOffsetPosY;


public:
	GraphicsSystem(int width, int height);
	~GraphicsSystem();

	Camera* default_camera;
	glm::mat4 orthoMatrix;
	glm::mat4 ProjectionMatrix;
	glm::mat4 ViewMatrix;
	Shader* DefaultShader;
	Shader* FontShader;
	Profiler profiler;

	FontTextureAtlas* fonttext;

	bool m_GUIMode;

	float load_timer;
	float hidden_timer;

	int Initialize(void);					// Initialize the OpenGL settings

	GLvoid ResizeScene(int, int);	    // Resize the window
	void startDrawingScene(void);

	void drawScene(std::vector<GraphicsContainer>*, SceneActor * current_scene_camera_actor, float dt, std::map<unsigned, SceneActor *> * loading_container,bool debug=false);
	void drawLoadScene(std::map<unsigned, SceneActor *> * loading_container);
	void drawLogo(PrefabActor * logo);

    void stopDrawingScene(HDC);
	void SetDefaultCameraZoom(float x);


};

