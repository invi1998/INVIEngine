#include "WindowsMessageProcessing.h"

LRESULT EngineWidowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	}

	// ����windows�Լ�����
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
