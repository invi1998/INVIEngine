#include "EngineMinimal.h"

#include "WindowsMessageProcessing.h"

#include <unordered_set>

#include "Component/Input/Input.h"
#include <WindowsX.h>

#include "imgui.h"

// 全局变量：记录正在按下的键
unordered_set<int> g_pressedKeys;
unordered_set<int> g_releasedKeys;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT EngineWidowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// 给imgui添加消息处理
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
		// 记录按下的键
		g_pressedKeys.insert(wParam);
		g_releasedKeys.erase(wParam);
		return 0;
	case WM_KEYUP:
		// 删除抬起的按键
		g_pressedKeys.erase(wParam);
		g_releasedKeys.insert(wParam);
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

bool FInput::IsKeyReleased(uint16_t keycode)
{
	bool state = g_releasedKeys.count(keycode) > 0;
	g_releasedKeys.erase(keycode);
	return state;
}

