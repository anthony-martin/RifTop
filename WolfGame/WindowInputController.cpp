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
		msg == WM_MOUSEWHEEL||
		msg == WM_MOUSEMOVE))
	{
		m_WindowManager->PostMessageSelected(msg, wParam, lParam);
		m_WindowManager->SendMessageSelected(WM_NCHITTEST, NULL, lParam);
		return 0;
	}

	if(m_InputMode &&
		msg == WM_RBUTTONDOWN || 
		msg == WM_RBUTTONUP ||
		msg == WM_LBUTTONDOWN || 
		msg == WM_LBUTTONUP )
	{
		LPARAM MouseActive = msg<<16|HTCLIENT;

		m_WindowManager->SendMessageSelected(WM_IME_SETCONTEXT, true, 0xC000000F);
		m_WindowManager->SendMessageSelected(WM_IME_NOTIFY, IMN_OPENSTATUSWINDOW, NULL);
		m_WindowManager->SendMessageSelected(WM_SETCURSOR, NULL, MouseActive);
		m_WindowManager->PostMessageSelected(msg, MK_LBUTTON, lParam);
		m_WindowManager->SendMessageSelected(WM_NCHITTEST, lParam);
		return 0;
	}

	return 0;
}
