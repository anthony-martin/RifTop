#include "stdafx.h"
#include "WindowInputController.h"


WindowInputController::WindowInputController(SystemWindowManager *windowManager)
	:m_WindowManager(windowManager),
	m_MoveWindow(true)
{
}


WindowInputController::~WindowInputController(void)
{
}


LRESULT WindowInputController::Handle(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//note can use with OIS but need to pass more than just the mouse down message

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
		default:
			return 0;
	}


	return 0;
}
