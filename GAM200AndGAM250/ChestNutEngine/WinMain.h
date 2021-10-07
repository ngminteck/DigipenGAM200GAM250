/******************************************************************************/
/*!
\file   WinMain.h

\author Wong Zhi Jun(100%)
\par    email: w.zhijun\@digipen.edu
\par    DigiPen login: w.zhijun

\brief
This file contains function and stucts declaration for WinMain

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/

#pragma once
#define NOMINMAX
#include <Windows.h>
#undef NOMINMAX
#include <tchar.h>

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING


bool active = TRUE;									// Window Active Flag Set To TRUE By Default

int main(int argc, char* argv[]);					// Console for debugging

int CALLBACK WinMain(HINSTANCE	hInstance,			// Instance
					 HINSTANCE	hPrevInstance,		// Previous Instance
					 LPSTR		lpCmdLine,			// Command Line Parameters
					 int			nCmdShow);		// Window Show State

LRESULT CALLBACK WndProc(HWND	hWnd,				// Handle For This Window
						 UINT	uMsg,				// Message For This Window
						 WPARAM	wParam,				// Additional Message Information
						 LPARAM	lParam);			// Additional Message Information