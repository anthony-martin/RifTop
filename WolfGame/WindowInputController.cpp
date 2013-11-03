#include "stdafx.h"
#include "WindowInputController.h"


WindowInputController::WindowInputController(SystemWindowManager *windowManager)
	:m_WindowManager(windowManager),
	m_MoveWindow(true),
	m_InputMode(true)
{
}


WindowInputController::~WindowInputController(void)
{
}


LRESULT WindowInputController::Handle(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//note can use with OIS but need to pass more than just the mouse down message
	if(m_InputMode && 
		(msg == WM_SYSKEYDOWN || 
		msg == WM_SYSKEYUP ||
		msg == WM_KEYDOWN || 
		msg ==WM_KEYUP ||
		msg == WM_SYSCHAR  ||
		msg == WM_CHAR ))
	{
		m_WindowManager->MessageSelected(msg, wParam, lParam);
		return 0;
	}

	switch (msg) 
    { 
		case WM_LBUTTONDOWN:
		{
			if(m_MoveWindow)
			{
				m_WindowManager->MoveSelected();
				m_MoveWindow = false;
			}
			else
			{
				m_WindowManager->ReleaseSelected();
				m_MoveWindow = true;
			}
			return 1;
		}
		case WM_KEYDOWN:
		{
			if(wParam == VK_LWIN || wParam == VK_RWIN )
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
		case WM_KEYUP:
		{
			
			if(wParam == VK_LWIN || wParam == VK_RWIN )
			{
				m_InputMode = true;
				return 1;
			}
			if(wParam == VK_TAB )
			{
				m_WindowManager->RemoveThumbnails();
				return 1;
			}
			
		}
		default:
			return 0;
	}


	return 0;
}
