/******************************************************************************/
/*!
\file   Editor.cpp

\author Ng Min Teck(100%)
\par    email: minteck.ng\@digipen.edu
\par    DigiPen login: minteck.ng

\brief
	Main editor init with docking

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/

#include "Editor.h"

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

Editor::Editor( InputSystem * input, ProfileHandler * profile, ResourceFactoryManager * resource_fac, EventHandler * ev, bool & pause, bool & gravity)
	:show_demo_window{ true }, m_input{ input }, m_ResourceFactoryManager{ resource_fac }, profile_handler{profile}
{

	command_manager = new CommandManager{ input };
	console = new EditorConsole{};
	editor_content_browser = new EditorContentBrowser{ console, m_ResourceFactoryManager, command_manager };
	editor_world_outliner = new EditorWorldOutliner{ console, m_ResourceFactoryManager, ev, command_manager, pause};
	editor_object_viewer = new Inspector{ console, m_ResourceFactoryManager, command_manager, ev, gravity};
	editor_object_selector = new EditorObjectSelector{ input, console , m_ResourceFactoryManager, command_manager };
	performance_viewer = new PerformanceViewer{ profile };
	profiler = new Profiler{ "Level Editor" };
	//profile_handler->m_profilers.push_back(profiler);
	profile_handler->m_profilers.push_back(editor_content_browser->m_profiler);
	profile_handler->m_profilers.push_back(editor_object_selector->m_profiler);
	profile_handler->m_profilers.push_back(editor_object_viewer->m_profiler);
	pause = true;
		
}

Editor::~Editor()
{
	SafeDelete(editor_content_browser);
	SafeDelete(editor_world_outliner);
	SafeDelete(editor_object_viewer);
	SafeDelete(editor_object_selector);
	SafeDelete(performance_viewer);
	SafeDelete(console);
	SafeDelete(command_manager);
	SafeDelete(profiler);
}

void Editor::InitEditor(HWND hWnd , Camera * cam)
{
	// Setup Dear ImGui binding
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();


	ImGuiIO & io = ImGui::GetIO();
    (void)io;

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;
	io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
	io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI

	io.ConfigResizeWindowsFromEdges = true;
	io.ConfigDockingWithShift = false;


	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplOpenGL3_Init(NULL);

	// Setup style
	ImGui::GetStyle().WindowRounding = 0.0f;
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	/***Initialize cameras******/
	editor_object_selector->m_cam = cam;
	editor_object_viewer->m_cam = cam;
	editor_world_outliner->m_cam = cam;
	m_cam = cam;

	editor_content_browser->Init();
}


void Editor::SetupEditorFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void Editor::InitDockingArea()
{
	
	static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_None;
	ImGuiIO & io = ImGui::GetIO();

	static bool opt_fullscreen_persistant = true;
	bool opt_fullscreen = opt_fullscreen_persistant;
	
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

//	ImGuiWindowFlags dock_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse  | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	// When using ImGuiDockNodeFlags_RenderWindowBg or ImGuiDockNodeFlags_InvisibleDockspace, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (opt_flags & ImGuiDockNodeFlags_RenderWindowBg)
		ImGui::SetNextWindowBgAlpha(0.0f);

//	float bottom_dock_size_x = viewport->Size.x;
	float bottom_dock_size_y = viewport->Size.y * 0.3f;

	float left_dock_size_x = viewport->Size.x * 0.2f;
	float left_dock_size_y = viewport->Size.y - bottom_dock_size_y;

	float right_dock_size_x = viewport->Size.x * 0.2f;
	float right_dock_size_y = viewport->Size.y - bottom_dock_size_y;

    float center_dock_size_x = viewport->Size.x - left_dock_size_x - right_dock_size_x;
//	float center_dock_size_y = viewport->Size.y - bottom_dock_size_y;

	float bottom_left_dock_size_x = left_dock_size_x;
	float bottom_left_dock_size_y = bottom_dock_size_y;

	float bottom_right_dock_size_x = right_dock_size_x;
	float bottom_right_dock_size_y = bottom_dock_size_y;

	float bottom_middle_dock_size_x = center_dock_size_x;
	float bottom_middle_dock_size_y = bottom_dock_size_y;


//	float bottom_dock_pos_x = viewport->Pos.x;
	float bottom_dock_pos_y = viewport->Pos.y + viewport->Size.y - bottom_dock_size_y;

	float left_dock_pos_x = viewport->Pos.x;
	float left_dock_pos_y = viewport->Pos.y;

	float right_dock_pos_x = viewport->Pos.x + viewport->Size.x - right_dock_size_x;
	float right_dock_pos_y = viewport->Pos.y;

	float center_dock_pos_x = viewport->Pos.x + left_dock_size_x;
//    float center_dock_pos_y = viewport->Pos.y;

	float bottom_left_dock_pos_x = left_dock_pos_x;
	float bottom_left_dock_pos_y = bottom_dock_pos_y;

	float bottom_right_dock_pos_x = right_dock_pos_x;
	float bottom_right_dock_pos_y = bottom_dock_pos_y;

	float bottom_middle_dock_pos_x = center_dock_pos_x;
	float bottom_middle_dock_pos_y = bottom_dock_pos_y;

	
	/*ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::SetNextWindowBgAlpha(1.0f);
	ImGui::Begin("Bottom Dock", NULL, window_flags);
	ImGui::SetWindowSize(ImVec2{ bottom_dock_size_x,bottom_dock_size_y});
	ImGui::SetWindowPos(ImVec2{ bottom_dock_pos_x, bottom_dock_pos_y });
	ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("Bottom Dockspace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);
	}
	
	ImGui::End();*/

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::SetNextWindowBgAlpha(1.0f);
	ImGui::Begin("Bottom Left Dock", NULL, window_flags);
	ImGui::SetWindowSize(ImVec2{ bottom_left_dock_size_x,bottom_left_dock_size_y });
	ImGui::SetWindowPos(ImVec2{ bottom_left_dock_pos_x, bottom_left_dock_pos_y });
	ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("Bottom Left Dockspace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);
	}

	ImGui::End();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::SetNextWindowBgAlpha(1.0f);
	ImGui::Begin("Bottom Right Dock", NULL, window_flags);
	ImGui::SetWindowSize(ImVec2{ bottom_right_dock_size_x,bottom_right_dock_size_y });
	ImGui::SetWindowPos(ImVec2{ bottom_right_dock_pos_x, bottom_right_dock_pos_y });
	ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("Bottom Right Dockspace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);
	}

	ImGui::End();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::SetNextWindowBgAlpha(1.0f);
	ImGui::Begin("Bottom Middle Dock", NULL, window_flags);
	ImGui::SetWindowSize(ImVec2{ bottom_middle_dock_size_x,bottom_middle_dock_size_y });
	ImGui::SetWindowPos(ImVec2{ bottom_middle_dock_pos_x, bottom_middle_dock_pos_y });
	ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("Bottom Middle Dockspace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);
	}

	ImGui::End();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::SetNextWindowBgAlpha(1.0f);
	ImGui::Begin("Left Dock", NULL, window_flags);
	ImGui::SetWindowSize(ImVec2{ left_dock_size_x ,left_dock_size_y });
	ImGui::SetWindowPos(ImVec2{ left_dock_pos_x ,left_dock_pos_y });
	ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("Left Dockspace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);
	}
	
	ImGui::End();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::SetNextWindowBgAlpha(1.0f);
	ImGui::Begin("Right Dock", NULL, window_flags);
	ImGui::SetWindowSize(ImVec2{ right_dock_size_x ,right_dock_size_y });
	ImGui::SetWindowPos(ImVec2{right_dock_pos_x  , right_dock_pos_y });
	ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("Right Dockspace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);
	}
	
	ImGui::End();

	/*
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::SetNextWindowBgAlpha(1.0f);
	ImGui::Begin("Center Dock", NULL, dock_flags);
	ImGui::SetWindowSize(ImVec2{ center_dock_size_x ,center_dock_size_y }, ImGuiCond_FirstUseEver);
	ImGui::SetWindowPos(ImVec2{ center_dock_pos_x  , center_dock_pos_y }, ImGuiCond_FirstUseEver);
	ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("Center Dockspace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);
	}

	ImGui::End();*/
}

void Editor::UpdateCamera()
{
	if (m_input->InputCheckCurr(VK_UP))
		m_cam->AddPosition(0, 1);
	if (m_input->InputCheckCurr(VK_DOWN))
		m_cam->AddPosition(0, -1);
	if (m_input->InputCheckCurr(VK_LEFT))
		m_cam->AddPosition(-1,0);
	if (m_input->InputCheckCurr(VK_RIGHT))
		m_cam->AddPosition(1, 0);
}


void Editor::Update()
{

	if (m_input->InputCheckCurr(CVK_C)) camera_mode = !camera_mode;
	if (camera_mode) UpdateCamera();
	command_manager->Update();
	editor_object_selector->Update();
	
}

void Editor::DrawEditor()
{
	InitDockingArea();

	//ImGuizmo::DrawGrid(cameraView, cameraProjection, identityMatrix, 10.f);

	//ImGui::ShowDemoWindow();
	
	profiler->UpdateStart();
	editor_content_browser->Draw();
	profiler->UpdateEnd();

	
	editor_world_outliner->Draw();
	editor_object_viewer->Draw();
	performance_viewer->Draw();

	
	console->Draw();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	
}

void Editor::UpdateEditorViewPort()
{
	// Update and Render additional Platform Windows
	ImGuiIO & io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();	
	}
}

void Editor::EndFrame()
{
	ImGui::EndFrame();
}

void Editor::ShutdownEditor()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

