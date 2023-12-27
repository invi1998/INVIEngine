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
	void Draw();

	//���ÿһ�е���ɫ״̬
	void AddLineColor(e_error InColor);

	//������ƫ��
	void ResetLineOffsets(e_error InColor, int InOldSize);

	//��ȡe_error��Ӧ����ɫ����
	ImVec4 GetColor(e_error InColorID);

	void HandleBackstageLog(e_error InColorID, int InOldSize);

protected:
	ImGuiTextBuffer LogBuf;		   // ��־������
	ImGuiTextFilter LogFilter;		   // ��־������
	ImVector<int> LineOffsets;        // Index to lines offset
	ImVector<e_error> PreLineColor;	  // ÿһ�е���ɫ
	bool AutoScroll = true;				// �Ƿ��Զ��������ײ�

protected:
	static FLogSystem* LogSys;

private:
	FLogSystem();
	~FLogSystem();

};

