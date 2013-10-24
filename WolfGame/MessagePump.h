#pragma once

class MessageHandler
{
	LRESULT Handle(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

namespace MessagePump
{
	void Subscribe(MessageHandler *handler);
	void UnSubscribe(MessageHandler *handler);

	LRESULT CALLBACK _WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

