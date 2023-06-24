#include "WindowsMessageProcessing.h"

LRESULT EngineWidowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	}

	// 交给windows自己处理
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
