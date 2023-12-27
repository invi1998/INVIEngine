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
	ImGuiTextBuffer LogBuf;		   // 日志缓冲区
	ImGuiTextFilter LogFilter;		   // 日志过滤器
	ImVector<int> LineOffsets;        // Index to lines offset
	bool AutoScroll = true;				// 是否自动滚动到底部

	void Draw();

protected:
	static FLogSystem* LogSys;

private:
	FLogSystem();
	~FLogSystem();

};

