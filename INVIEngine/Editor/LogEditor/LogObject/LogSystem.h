#pragma once

#include "imgui.h"

// 单例模式 日志系统
class FLogSystem
{
	friend class FLogEditor;
public:
	static FLogSystem* GetLog();
	static void Destroy();

	void Clear();
	void AddLog(const char* fmt, ...);
	void AddError(const char* fmt, ...);
	void AddWarning(const char* fmt, ...);
	void AddSuccess(const char* fmt, ...);

protected:
	void Draw();

	//添加每一行的颜色状态
	void AddLineColor(e_error InColor);

	//重置行偏移
	void ResetLineOffsets(e_error InColor, int InOldSize);

	//获取e_error对应的颜色数据
	ImVec4 GetColor(e_error InColorID);

	void HandleBackstageLog(e_error InColorID, int InOldSize);

protected:
	ImGuiTextBuffer LogBuf;		   // 日志缓冲区
	ImGuiTextFilter LogFilter;		   // 日志过滤器
	ImVector<int> LineOffsets;        // Index to lines offset
	ImVector<e_error> PreLineColor;	  // 每一行的颜色
	bool AutoScroll = true;				// 是否自动滚动到底部

protected:
	static FLogSystem* LogSys;

private:
	FLogSystem();
	~FLogSystem();

};

