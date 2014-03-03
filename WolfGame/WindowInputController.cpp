#include "stdafx.h"
#include "WindowInputController.h"


WindowInputController::WindowInputController(SystemWindowManager *windowManager,
											MouseCursor *mosueCursor)
	:m_WindowManager(windowManager),
	m_MoveWindow(false),
	m_InputMode(true),
	m_MosueCursor(mosueCursor)
{
}


WindowInputController::~WindowInputController(void)
{
}


LRESULT WindowInputController::HandleInputMode(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) 
    {
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
		{
			if(wParam == VK_OEM_3 )
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
			
			if(wParam == VK_OEM_3 )
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
			if(wParam == VK_OEM_3 )
			{
				m_InputMode = false;
				break;
			}
		}
		case WM_SYSKEYUP:
		case WM_KEYUP:
		{
			
			if(wParam == VK_OEM_3 )
			{
				m_InputMode = true;
				break;
			}
			if(wParam == VK_TAB )
			{
				m_WindowManager->RemoveThumbnails();
				return 1;
			}
		}
	}
	
	if(m_InputMode && 
		(
		msg == WM_KEYDOWN || 
		msg == WM_KEYUP ))
	{
		m_WindowManager->PostMessageSelected(msg, wParam, lParam);
		m_WindowManager->SendMessageSelected(WM_NCHITTEST, NULL, lParam);
		return 0;
	}


	return 0;
}


bool WindowInputController::mouseMoved( const OIS::MouseEvent &arg )
{
	//step one move the cursor
	m_MosueCursor->mouseInput(Ogre::Vector2(arg.state.X.rel, arg.state.Y.rel));
	//step 2 do whatever else we wanted to do
	if(!m_InputMode)
	{
		{
			if(m_MoveWindow)
			{
				short zDelta = arg.state.Z.rel;
				m_WindowManager->ScaleSelected(zDelta / WHEEL_DELTA);
			}
			return true;
		}
	}
	//otherwise we just send messages to the window
	
	Vector2 relMousePos;
	if(m_WindowManager->CheckWindowCollision( false, &relMousePos))
	{
		m_WindowManager->PostMessageSelected(WM_MOUSEMOVE, 0, relMousePos);
		if(arg.state.Z.rel != 0)
		{
			LPARAM mouseMove = arg.state.Z.rel<<16;
			m_WindowManager->PostMessageSelected(WM_MOUSEWHEEL, mouseMove, relMousePos);
		}
	}
    return true;
}

bool WindowInputController::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	if(!m_InputMode)
	{
		if(id == OIS::MouseButtonID::MB_Left)
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
	}
	//otherwise send input to window
	Vector2 relMousePos;
	if(m_WindowManager->CheckWindowCollision( true, &relMousePos))
	{
		LPARAM MouseActive = WM_LBUTTONDOWN<<16|HTCLIENT;
		if(id == OIS::MouseButtonID::MB_Left)
		{
			m_WindowManager->SendMessageSelected(WM_SETCURSOR, NULL, false);
			m_WindowManager->PostMessageSelected(WM_LBUTTONDOWN, MK_LBUTTON, relMousePos);
			m_WindowManager->SendMessageSelected(WM_NCHITTEST, relMousePos);
		}
	}
    return true;
}

bool WindowInputController::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	if(!m_InputMode)
	{
		return false;
	}

	Vector2 relMousePos;
	if(m_WindowManager->CheckWindowCollision( false, &relMousePos))
	{
		LPARAM MouseActive = WM_LBUTTONUP<<16|HTCLIENT;
		if(id == OIS::MouseButtonID::MB_Left)
		{
			m_WindowManager->SendMessageSelected(WM_SETCURSOR, NULL, MouseActive);
			m_WindowManager->PostMessageSelected(WM_LBUTTONUP, MK_LBUTTON, relMousePos);
			m_WindowManager->SendMessageSelected(WM_NCHITTEST, relMousePos);
		}
	}
    return true;
}