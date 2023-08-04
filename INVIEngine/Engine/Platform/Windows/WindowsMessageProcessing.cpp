#include "WindowsMessageProcessing.h"
#include "Component/Input/Input.h"
#include <WindowsX.h>

LRESULT EngineWidowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	//case WM_RBUTTONDOWN:	// 右键按下
	//	MouseDownDelegate.Broadcast(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	//	return 0;
	//case WM_LBUTTONUP:		// 右键抬起
	//	MouseUpDelegate.Broadcast(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	//	return 0;
	//case WM_MOUSEMOVE:		// 鼠标移动
	//	MouseMoveDelegate.Broadcast(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	//	return 0;
	case WM_MOUSEWHEEL:	 // 鼠标滚轮
		MousesWheelsDelegate.Broadcast(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), static_cast<short>(HIWORD(wParam)));
		return 0;
	}

	// 交给windows自己处理
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
