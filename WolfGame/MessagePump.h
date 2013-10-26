#pragma once
namespace MessagePump
{
	class MessageHandler
	{
		public:
			virtual LRESULT Handle(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
			{return 0;};
	};

	void Subscribe(MessageHandler *handler);
	void UnSubscribe(MessageHandler *handler);

	LRESULT CALLBACK _WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

