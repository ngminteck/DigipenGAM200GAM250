/******************************************************************************/
/*!
\file   Graphics.cpp

\author Wong Zhi Jun(100%)
\par    email: w.zhijun\@digipen.edu
\par    DigiPen login: w.zhijun

\brief
This file contains function and stucts declaration for Graphics

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include "GraphicsSystem.h"

#include "Shader.h"
#include "Mesh.h"
#include "Texture2D.h"
#include "Camera.h"
#include "MovementComponent.h"
#include "SceneActor.h"
#include "System.h"
#include "Font.h"

#include "MainWindow.h"
#include "Utils.h"

#include "CameraComponent.h"


GraphicsSystem::GraphicsSystem(int width,int height )
	:m_Height{ height }, m_Width{ width }, ProjectionMatrix{1.0f},ViewMatrix{1.0f},m_Aspect{(float)m_Width/(float)m_Height}, profiler{ "Graphics System" },
	default_cam_pos{ new glm::vec3(0.0f,0.0f,3.3f) }, default_cam_front{ new glm::vec3(0.0f,0.0f,-1.0f) }, default_cam_up{ new glm::vec3(0.0f,1.0f,0.0f) },
	default_CamOffsetPosX{ new float(0.0f) }, default_CamOffsetPosY{ new float(0.0f) }, load_timer{ 0.0f }, hidden_timer{ 0.0f }
{
}
GraphicsSystem::~GraphicsSystem()
{
	SafeDelete(DefaultShader);
	SafeDelete(default_camera);
	SafeDelete(default_cam_pos);
	SafeDelete(default_cam_front);
	SafeDelete(default_cam_up);
	SafeDelete(default_CamOffsetPosX);
	SafeDelete(default_CamOffsetPosY);
	SafeDelete(FontShader);
	SafeDelete(fonttext);
}

int GraphicsSystem::Initialize(void)
{
	glShadeModel(GL_SMOOTH);													// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);										// Black Background
	glClearDepth(1.0f);															// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);													// Enables Depth Testing

	glDepthFunc(GL_LEQUAL);														// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);							// Perspective Calculations

	glEnable(GL_BLEND);															// Enable blending of pixels
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);							// Transparent texture backgrounds
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL/*GL_LINE*/);						// Toggle meshs

	DefaultShader	= new Shader("Resource/ShaderSource/vertexShader.shader"	, "Resource/ShaderSource/fragShader.shader");	// Create shader program
	//FontShader		= new Shader("Resource/ShaderSource/fontVertexShader.shader", "Resource/ShaderSource/fontFragShader.shader");
	default_camera			= new Camera{ default_cam_pos, default_cam_front,default_cam_up, default_CamOffsetPosX,default_CamOffsetPosY , false,0.0f,0.0f,0.0f,0.0f};

	orthoMatrix = glm::ortho(0.0f, (float)m_Width, 0.f, (float)m_Height);		// For texture rendering
	
	DefaultShader->Bind();															// Bind shader program

	ViewMatrix = default_camera->GetViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(DefaultShader->GetShaderID(), "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));
	glUniformMatrix4fv(glGetUniformLocation(DefaultShader->GetShaderID(), "ViewMatrix"), 1, GL_FALSE, glm::value_ptr(ViewMatrix));

	//FontShader->Bind();
	//fonttext = new FontTextureAtlas("Resource/Font/arial.ttf", 48);
	//glUniformMatrix4fv(glGetUniformLocation(FontShader->GetShaderID(), "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(orthoMatrix));

	glBindTexture(GL_TEXTURE_2D, 0);											// Unbind texture

	return TRUE;																// Initialization  SUCCESS
}


GLvoid GraphicsSystem::ResizeScene(int width, int height)
{
	if (height == 0) height = 1;												// To prevent divide by 0
	m_Width = width;															// Calculate new Projection matrix
	m_Height = height;
	m_Aspect = static_cast<float>(width) / static_cast<float>(height);	
	ProjectionMatrix = glm::perspective(glm::radians(90.f), m_Aspect, 0.1f, 500.f);
	glViewport(0, 0, width, height);											// Set new viewport size
}

void GraphicsSystem::startDrawingScene(void)
{
	profiler.UpdateStart();
	// Start drawing the scene
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen ,Depth and Stencil Buffer
	glClearColor(1.f, 1.f, 1.f, 1.0f);										// Fill White
}

void GraphicsSystem::drawScene(std::vector<GraphicsContainer>*container, SceneActor * current_scene_camera_actor, float dt, std::map<unsigned, SceneActor *> * loading_container, bool debug)
{
	ViewMatrix = default_camera->GetViewMatrix();
	Vector2D cammin = default_camera->GetMin();
	Vector2D cammax = default_camera->GetMax();
	if (!debug && current_scene_camera_actor != nullptr)
	{
		auto & comps = current_scene_camera_actor->GetPrefabActor()->GetComponents();

		if (comps.find(ComponentType::CameraComponent) != comps.end())
		{
			CameraComponent* cam_comp = dynamic_cast<CameraComponent*>(comps.find(ComponentType::CameraComponent)->second);
			cam_comp->GetCamera()->Update(dt);
			cammin = cam_comp->GetCamera()->GetMin();
			cammax = cam_comp->GetCamera()->GetMax();
			ViewMatrix = cam_comp->GetCamera()->GetViewMatrix();
		}
	}

	if (load_timer > 0)
	{
		if (debug)
		{
			load_timer = 0.0f;
		}
		else
		{
			hidden_timer += dt;
			load_timer -= dt;
			drawLoadScene(loading_container);
			return;
		}
	}

	DefaultShader->Bind();
		glUniformMatrix4fv(glGetUniformLocation(DefaultShader->GetShaderID(), "ViewMatrix"), 1, GL_FALSE, glm::value_ptr(ViewMatrix));
		glUniformMatrix4fv(glGetUniformLocation(DefaultShader->GetShaderID(), "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));
		m_GUIMode = false;

		std::unordered_map<ComponentType, Component*>* comp;
		SpriteComponent*	spritecomp = nullptr;
		TransformComponent* transComp = nullptr;
		AABBComponent*		aabbComp = nullptr;
		for (auto& actor : *container)
		{
			if (!actor.scene_actor->GetActive())
				continue;

			comp = actor.prefab_actor->GetComponentsPtr();
			spritecomp = nullptr;
			transComp = nullptr;
			aabbComp = nullptr;

			if ((*comp).find(ComponentType::TransformComponent) != (*comp).end())
				transComp = dynamic_cast<TransformComponent*>((*comp)[ComponentType::TransformComponent]);
			if ((*comp).find(ComponentType::SpriteComponent) != (*comp).end())
				spritecomp = dynamic_cast<SpriteComponent*>((*comp)[ComponentType::SpriteComponent]);
			if (!spritecomp || !transComp)
				continue;
			if ((*comp).find(ComponentType::AABBComponent) != (*comp).end())
				aabbComp = dynamic_cast<AABBComponent*>((*comp)[ComponentType::AABBComponent]);
			//if ((!m_GUIMode && spritecomp->GetLayer() == 3) || (!m_GUIMode && spritecomp->GetLayer() == 4) || (!m_GUIMode && spritecomp->GetLayer() == 5))
			if (!m_GUIMode && spritecomp->IsOrthCam())
			{
				glm::mat4 GUIview{ 1 };
				glUniformMatrix4fv(glGetUniformLocation(DefaultShader->GetShaderID(), "ViewMatrix"), 1, GL_FALSE, glm::value_ptr(GUIview));
				glUniformMatrix4fv(glGetUniformLocation(DefaultShader->GetShaderID(), "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(orthoMatrix));
				m_GUIMode = true;
			}

			if (!debug && aabbComp)
			{
				Vector2D objmin = aabbComp->GetMin();
				Vector2D objmax = aabbComp->GetMax();

				if (objmin.x > cammax.x || cammin.x > objmax.x)
					continue;
				if (objmax.y < cammin.y || cammax.y < objmin.y)
					continue;
			}

			Texture2D* tex = spritecomp->GetTexture();
			if (!tex)
				continue;
			Mesh* mesh = spritecomp->GetMesh();
			Animation* anim = spritecomp->GetAnimation();

			mesh->Update(transComp->GetFinalMatrix());
			tex->Bind();
			mesh->Bind();

			float offset = anim->GetOffset().x* spritecomp->GetFrameCount();
			DefaultShader->set1f(offset, "currFrame");

			//else
			//	DefaultShader->set1f(0, "currFrame");

			DefaultShader->setMat4fv(mesh->GetModelMatrix(), "ModelMatrix");
			glDrawElements(GL_TRIANGLES, mesh->GetIndexCount(), GL_UNSIGNED_INT, 0);
			if (debug)
			{
				tex->Unbind();
				if (aabbComp)
				{
					mesh->Update(transComp->GetAABBMatrix());
					mesh->Bind();
					DefaultShader->setMat4fv(mesh->GetModelMatrix(), "ModelMatrix");
					glDrawElements(GL_LINE_LOOP, mesh->GetIndexCount(), GL_UNSIGNED_INT, 0);
				}
			}
		}
	
	
  profiler.UpdateEnd();
}

void GraphicsSystem::drawLoadScene(std::map<unsigned, SceneActor *> * loading_container)
{
	ViewMatrix = default_camera->GetViewMatrix();

	DefaultShader->Bind();
	glUniformMatrix4fv(glGetUniformLocation(DefaultShader->GetShaderID(), "ViewMatrix"), 1, GL_FALSE, glm::value_ptr(ViewMatrix));
	glUniformMatrix4fv(glGetUniformLocation(DefaultShader->GetShaderID(), "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));
	m_GUIMode = false;

	std::unordered_map<ComponentType, Component*>* comp;
	SpriteComponent*	spritecomp = nullptr;
	TransformComponent* transComp = nullptr;
	
	for (auto& actor : *loading_container)
	{
		if (!actor.second->GetActive())
			continue;

		comp = actor.second->GetPrefabActor()->GetComponentsPtr();
		spritecomp = nullptr;
		transComp = nullptr;
	

		if ((*comp).find(ComponentType::TransformComponent) != (*comp).end())
			transComp = dynamic_cast<TransformComponent*>((*comp)[ComponentType::TransformComponent]);
		if ((*comp).find(ComponentType::SpriteComponent) != (*comp).end())
			spritecomp = dynamic_cast<SpriteComponent*>((*comp)[ComponentType::SpriteComponent]);
		if (!spritecomp || !transComp)
			continue;

		if (hidden_timer > 0.48f && spritecomp->GetSpriteName().compare("RIGHT_Checkpoint_Lit")==0)
		{
			hidden_timer = 0.0f;
			unsigned frame = spritecomp->GetFrameCount() + 1;
			if (frame > 5)
			{
				frame = 1;
			}
			spritecomp->SetFrameCount(frame);
		}
		
		if (!m_GUIMode && spritecomp->IsOrthCam())
		{
			glm::mat4 GUIview{ 1 };
			glUniformMatrix4fv(glGetUniformLocation(DefaultShader->GetShaderID(), "ViewMatrix"), 1, GL_FALSE, glm::value_ptr(GUIview));
			glUniformMatrix4fv(glGetUniformLocation(DefaultShader->GetShaderID(), "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(orthoMatrix));
			m_GUIMode = true;
		}


		Texture2D* tex = spritecomp->GetTexture();
		if (!tex)
			continue;
		Mesh* mesh = spritecomp->GetMesh();
		Animation* anim = spritecomp->GetAnimation();

		mesh->Update(transComp->GetFinalMatrix());
		tex->Bind();
		mesh->Bind();

		float offset = anim->GetOffset().x* spritecomp->GetFrameCount();
		DefaultShader->set1f(offset, "currFrame");

		//else
		//	DefaultShader->set1f(0, "currFrame");

		DefaultShader->setMat4fv(mesh->GetModelMatrix(), "ModelMatrix");
		glDrawElements(GL_TRIANGLES, mesh->GetIndexCount(), GL_UNSIGNED_INT, 0);
		
	}
}

void GraphicsSystem::drawLogo(PrefabActor * logo)
{
	ViewMatrix = default_camera->GetViewMatrix();

	DefaultShader->Bind();
	glUniformMatrix4fv(glGetUniformLocation(DefaultShader->GetShaderID(), "ViewMatrix"), 1, GL_FALSE, glm::value_ptr(ViewMatrix));
	glUniformMatrix4fv(glGetUniformLocation(DefaultShader->GetShaderID(), "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));
	m_GUIMode = false;

	std::unordered_map<ComponentType, Component*>* comp;
	SpriteComponent*	spritecomp = nullptr;
	TransformComponent* transComp = nullptr;

	
		comp = logo->GetComponentsPtr();
		spritecomp = nullptr;
		transComp = nullptr;


		if ((*comp).find(ComponentType::TransformComponent) != (*comp).end())
			transComp = dynamic_cast<TransformComponent*>((*comp)[ComponentType::TransformComponent]);
		if ((*comp).find(ComponentType::SpriteComponent) != (*comp).end())
			spritecomp = dynamic_cast<SpriteComponent*>((*comp)[ComponentType::SpriteComponent]);
		if (!spritecomp || !transComp)
			return;

		if (!m_GUIMode && spritecomp->IsOrthCam())
		{
			glm::mat4 GUIview{ 1 };
			glUniformMatrix4fv(glGetUniformLocation(DefaultShader->GetShaderID(), "ViewMatrix"), 1, GL_FALSE, glm::value_ptr(GUIview));
			glUniformMatrix4fv(glGetUniformLocation(DefaultShader->GetShaderID(), "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(orthoMatrix));
			m_GUIMode = true;
		}

		Texture2D* tex = spritecomp->GetTexture();
		if (!tex)
			return;
		Mesh* mesh = spritecomp->GetMesh();
		Animation* anim = spritecomp->GetAnimation();

		mesh->Update(transComp->GetFinalMatrix());
		tex->Bind();
		mesh->Bind();

		float offset = anim->GetOffset().x* spritecomp->GetFrameCount();
		DefaultShader->set1f(offset, "currFrame");


		DefaultShader->setMat4fv(mesh->GetModelMatrix(), "ModelMatrix");
		glDrawElements(GL_TRIANGLES, mesh->GetIndexCount(), GL_UNSIGNED_INT, 0);

}

void GraphicsSystem::stopDrawingScene(HDC hDC)
{
	// Stop drawing the scene
	SwapBuffers(hDC);	// Swap Buffers (Double Buffering)

}

void GraphicsSystem::SetDefaultCameraZoom(float x)
{
	 default_camera->SetPosition(default_camera->GetPosition().x, default_camera->GetPosition().y, default_camera->GetPosition().z + x);
}



