#include "EngineMinimal.h"

#include "WindowsMessageProcessing.h"

#include <unordered_set>

#include "Component/Input/Input.h"
#include <WindowsX.h>

#include "imgui.h"

// ȫ�ֱ�������¼���ڰ��µļ�
unordered_set<int> g_pressedKeys;
unordered_set<int> g_releasedKeys;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT EngineWidowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// ��imgui�����Ϣ����
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
	{
		return true;
	}

	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN:
		// ��¼���µļ�
		g_pressedKeys.insert(wParam);
		g_releasedKeys.erase(wParam);
		return 0;
	case WM_KEYUP:
		// ɾ��̧��İ���
		g_pressedKeys.erase(wParam);
		g_releasedKeys.insert(wParam);
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

bool FInput::IsKeyReleased(uint16_t keycode)
{
	bool state = g_releasedKeys.count(keycode) > 0;
	g_releasedKeys.erase(keycode);
	return state;
}

