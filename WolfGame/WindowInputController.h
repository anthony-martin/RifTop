#pragma once
#include "MessagePump.h"
#include "SystemWindowManager.h"

class WindowInputController : public MessagePump::MessageHandler
{
private:
	SystemWindowManager *m_WindowManager;
	bool m_MoveWindow;

public:
	WindowInputController(SystemWindowManager *windowManager);
	~WindowInputController(void);

	virtual LRESULT Handle(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

