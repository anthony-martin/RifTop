#include "stdafx.h"
#include "MessagePump.h"

using namespace MessagePump;

std::vector<MessageHandler*> m_Handlers;

LRESULT CALLBACK MessagePump::_WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	for (std::vector<MessageHandler*>::iterator it = m_Handlers.begin(); it != m_Handlers.end(); ++it)
	{
		LRESULT result = (*it)->Handle(hwnd, msg, wParam, lParam);
		if(result != 0)
		{
			return result;
		}
	}
	return DefWindowProc( hwnd, msg, wParam, lParam );
}

void MessagePump::Subscribe(MessageHandler *handler)
{
	m_Handlers.push_back(handler);
}

void MessagePump::UnSubscribe(MessageHandler *handler)
{
	m_Handlers.erase(std::remove(m_Handlers.begin(), m_Handlers.end(), handler), m_Handlers.end());
}
