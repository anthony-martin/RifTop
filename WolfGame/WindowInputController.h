#pragma once
#include "MessagePump.h"
#include "SystemWindowManager.h"

class WindowInputController : public MessagePump::MessageHandler,
							  public OIS::MouseListener
{
private:
	SystemWindowManager *m_WindowManager;
	bool m_MoveWindow;
	bool m_InputMode;
	MouseCursor *m_MosueCursor;

	LRESULT HandleInputMode(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
	WindowInputController(SystemWindowManager *windowManager, MouseCursor *mosueCursor);
	~WindowInputController(void);

	virtual LRESULT Handle(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// OIS::MouseListener
    virtual bool mouseMoved( const OIS::MouseEvent &arg );
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
};

