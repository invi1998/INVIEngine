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
	//case WM_RBUTTONDOWN:	// �Ҽ�����
	//	MouseDownDelegate.Broadcast(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	//	return 0;
	//case WM_LBUTTONUP:		// �Ҽ�̧��
	//	MouseUpDelegate.Broadcast(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	//	return 0;
	//case WM_MOUSEMOVE:		// ����ƶ�
	//	MouseMoveDelegate.Broadcast(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	//	return 0;
	case WM_MOUSEWHEEL:	 // ������
		MousesWheelsDelegate.Broadcast(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), static_cast<short>(HIWORD(wParam)));
		return 0;
	}

	// ����windows�Լ�����
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
