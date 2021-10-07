/******************************************************************************/
/*!
\file   MainWindow.cpp

\author Wong Zhi Jun(100%)
\par    email: w.zhijun\@digipen.edu
\par    DigiPen login: w.zhijun

\brief
This file contains function and stucts definition for MainWindow

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/

#include "MainWindow.h"
#include "GraphicsSystem.h"

#include "imgui.h"
#include "imgui_impl_win32.h"


MainWindow::MainWindow(WNDPROC wndproc, int w, int h,std::string title, bool fs)
	:m_hRC{ NULL }, m_hInstance{ NULL }, m_WndProc{ wndproc },
	m_hWnd{ NULL }, m_hDC{ NULL }, m_WndWidth{w}, m_WndHeight{h},
	m_WndTitle{title},m_Fullscreen{fs}, m_Bits{32}
{
	/*------------------------------------------------*/
	/*	Window Parameters Set In MainWindow.h File"   */
	/*------------------------------------------------*/
}

MainWindow::~MainWindow() 
{
}
HDC* MainWindow::GetHDC()
{
	return &m_hDC;
}
HWND* MainWindow::GetHWND()
{
	return &m_hWnd;
}
int MainWindow::GetWndWidth()
{
	return m_WndWidth;
}

int * MainWindow::GetWndWidthPtr()
{
	return &m_WndHeight;
}

int MainWindow::GetWndHeight()
{
	return m_WndHeight;
}

int  * MainWindow::GetWndHeightPtr()
{
	return &m_WndHeight;
}

void MainWindow::ToggleFullScreenMode(GraphicsSystem* m_graphicsSystem)
{
	// Toggle between fullscreen and window mode
	WindowDestroy();				// Kill Our Current Window
	m_Fullscreen = !m_Fullscreen;	// Toggle Fullscreen / Windowed Mode
	WindowCreate(m_graphicsSystem);			// Recreate Our OpenGL Window
}

void MainWindow::WindowDestroy(void) 
{
	// Properly Kill The Window
	if (m_Fullscreen) 
	{													
		ChangeDisplaySettings(NULL, 0);					// If So Switch Back To The Desktop
		ShowCursor(FALSE);								// Show Mouse Pointer
	}

	if (m_hRC) 
	{
		if (!wglMakeCurrent(NULL, NULL)) 
		{
			MessageBoxA(NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}
		if (!wglDeleteContext(m_hRC)) 
		{
			MessageBoxA(NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}
		m_hRC = NULL;										// Set RC To NULL
	}
	if (m_hDC && !ReleaseDC(m_hWnd, m_hDC))
	{
		MessageBoxA(NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		m_hDC = NULL;										// Set DC To NULL
	}
	if (m_hWnd && !DestroyWindow(m_hWnd)) 
	{
		MessageBoxA(NULL, "Could Not Release m_hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		m_hWnd = NULL;									// Set m_hWnd To NULL
	}
	if (!UnregisterClassA(m_WndTitle.c_str(), m_hInstance)) 
	{
		MessageBoxA(NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		m_hInstance = NULL;								// Set m_hInstance To NULL
	}
}

bool MainWindow::WindowCreate(GraphicsSystem* graphics) 
{
	int PosX=0, PosY=0;											// Used to Center Window Position
	GLuint PixelFormat;											// Holds The Results After Searching For A Match
	WNDCLASS wc;												// Windows Class Structure
	DWORD dwExStyle;											// Window Extended Style
	DWORD dwStyle;												// Window Style
	RECT WindowRect;											// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left = (long)0;									// Set Left Value To 0
	WindowRect.right = (long)m_WndWidth;						// Set Right Value To Requested Width
	WindowRect.top = (long)0;									// Set Top Value To 0
	WindowRect.bottom = (long)m_WndHeight;						// Set Bottom Value To Requested Height

	m_hInstance = GetModuleHandle(NULL);						// Grab An Instance For Our Window
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;				// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc = (WNDPROC)m_WndProc;						// m_WndProc Handles Messages
	wc.cbClsExtra = 0;											// No Extra Window Data
	wc.cbWndExtra = 0;											// No Extra Window Data
	wc.hInstance = m_hInstance;									// Set The Instance
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);						// Load The Default Icon
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);					// Load The Arrow Pointer
	wc.hbrBackground = NULL;									// No Background Required For GL
	wc.lpszMenuName = NULL;										// We Don't Want A Menu
	wc.lpszClassName = m_WndTitle.c_str();						// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{									
		MessageBox(NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;											// Return FALSE
	}

	if (m_Fullscreen)												// Full screen Mode
	{
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth = m_WndWidth;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight = m_WndHeight;			// Selected Screen Height
		dmScreenSettings.dmBitsPerPel = m_Bits;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL, "The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?", "INFOMGEP", MB_YESNO | MB_ICONEXCLAMATION) == IDYES) 
				m_Fullscreen = false; // Windowed Mode Selected.  Fullscreen = FALSE
			else 
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL, "Program Will Now Close.", "ERROR", MB_OK | MB_ICONSTOP);
				return false;
			}
		}
	}

	if (m_Fullscreen)											// FullScreen Settings
	{											
		dwExStyle = WS_EX_APPWINDOW;						// Window Extended Style
		dwStyle = WS_POPUP;									// Windows Style
		ShowCursor(FALSE);									// Hide Mouse Pointer
	}
	else													// Windowed Settings
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;		// Window Extended Style
		dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU  ;						// Windows Style

		// Center Window
		PosX = (GetSystemMetrics(SM_CXSCREEN) - m_WndWidth) / 2;
		PosY = (GetSystemMetrics(SM_CYSCREEN) - m_WndHeight) / 2;
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);	// Adjust Window To True Requested Size

	// Create The Window
	m_hWnd = CreateWindowEx(dwExStyle,						// Extended Style For The Window
		m_WndTitle.c_str(),									// Class Name
		m_WndTitle.c_str(),									// Window Title
		dwStyle |											// Defined Window Style
		WS_CLIPSIBLINGS |									// Required Window Style
		WS_CLIPCHILDREN,									// Required Window Style
		PosX, PosY,											// Window Position
		WindowRect.right - WindowRect.left,					// Calculate Window Width
		WindowRect.bottom - WindowRect.top,					// Calculate Window Height
		NULL,												// No Parent Window
		NULL,												// No Menu
		m_hInstance,										// Instance
		this);												// Dont Pass Anything To WM_CREATE

	if (!m_hWnd)
	{
		WindowDestroy();									// Reset The Display
		MessageBox(NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;										// Return FALSE
	}


	static	PIXELFORMATDESCRIPTOR pfd =						// Pixel Format For The Window
	{
		sizeof(PIXELFORMATDESCRIPTOR),						// Size Of This Pixel Format Descriptor
		1,													// Version Number
		PFD_DRAW_TO_WINDOW |								// Format Must Support Window
		PFD_SUPPORT_OPENGL |								// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,									// Must Support Double Buffering
		(BYTE)PFD_TYPE_RGBA,								// Request An RGBA Format
		(BYTE)m_Bits,											// Select Our Color Depth
		(BYTE)0,(BYTE)0, (BYTE)0,(BYTE)0,(BYTE)0,(BYTE)0,	// Color Bits Ignored
		(BYTE)0,											// No Alpha Buffer
		(BYTE)0,											// Shift Bit Ignored
		(BYTE)0,											// No Accumulation Buffer
		(BYTE)0,(BYTE)0,(BYTE)0,(BYTE)0,					// Accumulation Bits Ignored
		(BYTE)32,											// 16Bit Z-Buffer (Depth Buffer)  
		(BYTE)0,											// No Stencil Buffer
		(BYTE)0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,										// Main Drawing Layer
		(BYTE)0,											// Reserved
		0, 0, 0												// Layer Masks Ignored
	};
	m_hDC = GetDC(m_hWnd);
	if (!m_hDC)												// Did We Get A Device Context?
	{						
		WindowDestroy();									// Reset The Display
		MessageBoxA(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	PixelFormat = ChoosePixelFormat(m_hDC, &pfd);
	if (!PixelFormat)										// Did Windows Find A Matching Pixel Format?
	{ 
		WindowDestroy();									// Reset The Display
		MessageBoxA(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	if (!SetPixelFormat(m_hDC, PixelFormat, &pfd))			// Are We Able To Set The Pixel Format?
	{	
		WindowDestroy();									// Reset The Display
		MessageBoxA(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	
	m_hRC = wglCreateContext(m_hDC);
	if (!m_hRC)												// Are We Able To Get A Rendering Context?
	{			
		WindowDestroy();									// Reset The Display
		MessageBoxA(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	if (!wglMakeCurrent(m_hDC, m_hRC))							// Try To Activate The Rendering Context
	{				
		WindowDestroy();									// Reset The Display
		MessageBoxA(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();								// Initialize GLEW
	if (GLEW_OK != err)										// Display Error if failed to initialize
	{
		WindowDestroy();
		MessageBoxA(NULL, "GLEW is not initialized!", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	//std::cout << glGetString(GL_VERSION) << std::endl;	// Display OpenGL Version

	ShowWindow(m_hWnd, SW_SHOW);								// Show The Window
	SetForegroundWindow(m_hWnd);								// Slightly Higher Priority
	SetFocus(m_hWnd);											// Sets Keyboard Focus To The Window
	graphics->ResizeScene(m_WndWidth, m_WndHeight);		// Set Up Our Perspective GL Screen
	
	if (!graphics->Initialize())							// Initialize Our Newly Created GL Window
	{										
		WindowDestroy();									// Reset The Display
		MessageBoxA(NULL, "Initialization Failed.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	return true;											// Success
}

