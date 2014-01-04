#include "stdafx.h"
#include "WindowInputController.h"


WindowInputController::WindowInputController(SystemWindowManager *windowManager)
	:m_WindowManager(windowManager),
	m_MoveWindow(false),
	m_InputMode(true)
{
}


WindowInputController::~WindowInputController(void)
{
}


LRESULT WindowInputController::HandleInputMode(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) 
    {
		case WM_MOUSEWHEEL:
		{
			if(m_MoveWindow)
			{
				short zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
				m_WindowManager->ScaleSelected(zDelta / WHEEL_DELTA);
			}
			return 1;
		}
		case WM_LBUTTONDOWN:
		{
			if(!m_MoveWindow)
			{
				m_WindowManager->MoveSelected();
				m_MoveWindow = true;
			}
			else
			{
				m_WindowManager->ReleaseSelected();
				m_MoveWindow = false;
			}
			return 1;
		}
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
		{
			if(wParam == VK_MENU )
			{
				m_InputMode = false;
				return 1;  
			}

			if(wParam == VK_TAB )
			{
				m_WindowManager->ShowThumbnails();
				return 1;
			}
			
		}
		case WM_SYSKEYUP:
		case WM_KEYUP:
		{
			
			if(wParam == VK_MENU || wParam == VK_RWIN )
			{
				m_InputMode = true;
				return 0;
			}
			if(wParam == VK_TAB )
			{
				m_WindowManager->RemoveThumbnails();
				return 1;
			}
			
		}
	}
	return 0;
}

LRESULT WindowInputController::Handle(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(!m_InputMode)
	{
		return HandleInputMode(hwnd, msg, wParam, lParam);
	}

	switch (msg) 
    {
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
		{
			if(wParam == VK_MENU )
			{
				m_InputMode = false;
				return 1;  
			}
		}
		case WM_SYSKEYUP:
		case WM_KEYUP:
		{
			
			if(wParam == VK_MENU || wParam == VK_RWIN )
			{
				m_InputMode = true;
				return 0;
			}
			if(wParam == VK_TAB )
			{
				m_WindowManager->RemoveThumbnails();
				return 1;
			}
		}
	}
		//note can use with OIS but need to pass more than just the mouse down message
	if(m_InputMode && 
		(
		msg == WM_KEYDOWN || 
		msg == WM_KEYUP ||
		msg == WM_MOUSEHWHEEL ||
		msg == WM_MOUSEWHEEL))
	{
		m_WindowManager->MessageSelected(msg, wParam, lParam);
		return 0;
	}

	return 0;
}
