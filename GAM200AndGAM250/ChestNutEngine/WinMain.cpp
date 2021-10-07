/******************************************************************************/
/*!
\file   WinMain.cpp

\author Wong Zhi Jun(100%)
\par    email: w.zhijun\@digipen.edu
\par    DigiPen login: w.zhijun

\brief
This file contains function and stucts definition for WinMain

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/

#include "WinMain.h"	
#include "GameEngine.h"
#include "Utils.h"
#include <string>
#include <iostream>
#include "Overthrone.h"

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING


struct LevelState;

GameEngine * pEngine;									// Pointer to Game Engine
#if defined(DEBUG) |defined(_DEBUG)						// Debugging console only in DEBUG mode
int main(int argc, char* argv[])			
{
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);

	WinMain((HINSTANCE)GetModuleHandle(NULL), 0, 0, SW_SHOW);
}
#endif

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);	
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

#if defined(DEBUG) |defined(_DEBUG)						// Check for Memory Leaks In Debug Mode
#define _CRTDBG_MAP_ALLOC

	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetBreakAlloc(0);
#endif 

	MSG msg {};	// Used for PeekMessage	

	//Instantiate engine
	pEngine = new GameEngine{(WNDPROC)WndProc }; 

	//Instantiate Game State Manager
	GameStateManager * pGameState = new GameStateManager{}; 

	//Instantiate the game
	Overthrone OVER_THRONE{ pEngine, pGameState};

	//Initialize all systems
	OVER_THRONE.RunGame(msg, active);
	
	delete pGameState;

	pEngine->GetResourceManager()->GetPrefabActorFactory()->SaveAllPrefabActor();

	pEngine->DestroyWindow();

	delete pEngine;
	return 0;
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hWnd,UINT	uMsg,WPARAM	wParam,	LPARAM	lParam)	
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	switch (uMsg)										// Check For Windows Messages
	{
		case WM_MOVING:
			return 0;
		case WM_ACTIVATE:								// Window Activate Message
		{							
			if (!HIWORD(wParam)) active = TRUE;			// Check Minimization State ->Program Is Active
			else active = FALSE;						// Program Is No Longer Active
			return 0;									// Return To The Message Loop
		}
		case WM_SYSCHAR:								// Disable ALT+key beeping sound
			return 0;
		case WM_SYSCOMMAND:								// Intercept System Commands
		{						
			switch (wParam)								// Check System Calls
			{
			case SC_SCREENSAVE:							// Screensaver Trying To Start?
			case SC_MONITORPOWER:						// Monitor Trying To Enter Powersave?
			case WM_SYSKEYDOWN:							// Prevent freeze when pressing tab/enter	
			case WM_SYSKEYUP:
				return 0;								// Prevent From Happening
			}
			break;
		}
		case WM_DPICHANGED:
//			if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
			{
				//const int dpi = HIWORD(wParam);
				//printf("WM_DPICHANGED to %d (%.0f%%)\n", dpi, (float)dpi / 96.0f * 100.0f);
//				const RECT* suggested_rect = (RECT*)lParam;
//				::SetWindowPos(hWnd, NULL, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
			}
			break;
		case WM_CLOSE:									// Close Message?
		{							
			PostQuitMessage(0);							// Send A Quit Message e.g(exited with code 0....)
			return 0;	
		}
		case WM_SIZE:									// Resize The OpenGL Context
		{								
			pEngine->ResizeWindow(LOWORD(lParam), HIWORD(lParam));
			return 0;

		}
		case WM_SETFOCUS:
		{
			PostMessage(hWnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
			return 0;
		}
		case WM_KILLFOCUS:
		{
			PostMessage(hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
			return 0;
		}
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);	// Pass All Unhandled Messages To DefWindowProc
}

