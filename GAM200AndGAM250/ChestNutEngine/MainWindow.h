/******************************************************************************/
/*!
\file   MainWindow.h

\author Wong Zhi Jun(100%)
\par    email: w.zhijun\@digipen.edu
\par    DigiPen login: w.zhijun

\brief
This file contains function and stucts declaration for MainWindow

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/

#pragma once
#define NOMINMAX
#include <Windows.h>  // Header File For Windows
#undef NOMINMAX
#include <string>
#include <vector>
#include <iostream>


class GraphicsSystem;

class MainWindow 
{
private:
	HGLRC		m_hRC;						// Permanent Rendering Context
	HINSTANCE	m_hInstance;				// Holds The Instance Of The Application
	WNDPROC		m_WndProc;					// Holds The m_WndProc of Windows
	HWND		m_hWnd;						// Holds Our Window Handle
	HDC			m_hDC;						// Device Context

	bool m_Fullscreen;						// Fullscreen Flag
	std::string m_WndTitle;					// Name of the window

	int m_WndWidth;							// Width of the window
	int m_WndHeight;						// Height of the window
	int m_Bits;								// Number Of Bits To Use For Color (8/16/24/32)	

public:
	MainWindow(WNDPROC WndProc, int w =1280, int h = 800,
			   std::string title = "OverThrone",bool fs = true);	// Construtor
	~MainWindow();							// Destructor

	// Getters
	HDC* GetHDC();
	HWND* GetHWND();
	int  GetWndWidth();
	int  GetWndHeight();

	int  *GetWndWidthPtr();
	int  *GetWndHeightPtr();

	void ToggleFullScreenMode(GraphicsSystem*);	// Toggle between fullscreen and window mode
	bool WindowCreate(GraphicsSystem*);			// Create the OpenGL window
	void WindowDestroy(void);				// Destroy the OpenGL window
};