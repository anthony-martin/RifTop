#include "stdafx.h"
#include "MessagePump.h"


LRESULT CALLBACK MessagePump::_WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	/*DWORD dwPID;
	bool posted = false;
	DWORD hThread ;
	hThread = GetWindowThreadProcessId(mExternalWindow, &dwPID);  
	if (dwPID != NULL && hThread!= NULL ) 
	{
		SendMessage(mExternalWindow, msg, wParam, lParam);
	}*/
	return DefWindowProc( hwnd, msg, wParam, lParam );
	return 0;
}
