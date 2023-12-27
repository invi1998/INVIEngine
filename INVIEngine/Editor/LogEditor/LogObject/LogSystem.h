#pragma once

#include "imgui.h"

// ����ģʽ ��־ϵͳ
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
	ImGuiTextBuffer LogBuf;		   // ��־������
	ImGuiTextFilter LogFilter;		   // ��־������
	ImVector<int> LineOffsets;        // Index to lines offset
	bool AutoScroll = true;				// �Ƿ��Զ��������ײ�

	void Draw();

protected:
	static FLogSystem* LogSys;

private:
	FLogSystem();
	~FLogSystem();

};

