#include "EngineMinimal.h"
#include "LogSystem.h"

FLogSystem* FLogSystem::LogSys = nullptr;

FLogSystem* FLogSystem::GetLog()
{
	if (LogSys == nullptr)
	{
		LogSys = new FLogSystem();
	}

	return LogSys;
}

void FLogSystem::Destroy()
{
	if (LogSys != nullptr)
	{
		delete LogSys;
		LogSys = nullptr;
	}
}

void FLogSystem::Clear()
{
	LogBuf.clear();
	LineOffsets.clear();
	LineOffsets.push_back(0);
}

void FLogSystem::AddLog(const char* fmt, ...)
{
	//��ȡ��һ���������
	int TextSize = LogBuf.size();

	//�ɱ����
	va_list Args;
	va_start(Args, fmt);
	LogBuf.appendfv(fmt, Args);
	va_end(Args);

	HandleBackstageLog(SIMPLE_C_LOG, TextSize);
}

void FLogSystem::AddError(const char* fmt, ...)
{
	int TextSize = LogBuf.size();

	va_list Args;
	va_start(Args, fmt);
	LogBuf.appendfv(fmt, Args);
	va_end(Args);

	HandleBackstageLog(SIMPLE_C_ERROR, TextSize);
}

void FLogSystem::AddWarning(const char* fmt, ...)
{
	int TextSize = LogBuf.size();

	va_list Args;
	va_start(Args, fmt);
	LogBuf.appendfv(fmt, Args);
	va_end(Args);

	HandleBackstageLog(SIMPLE_C_WARNING, TextSize);
}

void FLogSystem::AddSuccess(const char* fmt, ...)
{
	int TextSize = LogBuf.size();

	va_list Args;
	va_start(Args, fmt);
	LogBuf.appendfv(fmt, Args);
	va_end(Args);

	HandleBackstageLog(SIMPLE_C_SUCCESS, TextSize);
}

void FLogSystem::Draw()
{
	if (PreLineColor.Size != LineOffsets.Size)
	{
		return;
	}

	if (!ImGui::Begin("Log"))
	{
		ImGui::End();
		return;
	}

	//��ť����
	{
		if (ImGui::BeginPopup("LogOptions"))
		{
			ImGui::Checkbox("Auto Scroll", &AutoScroll);
			ImGui::EndPopup();
		}

		ImGui::SameLine();

		//��Ҫһ����ť������־
		if (ImGui::Button("LogOptions"))
		{
			ImGui::OpenPopup("LogOptions");
		}

		ImGui::SameLine();

		if (ImGui::Button("ClearAllLog"))
		{
			Clear();
		}

		ImGui::SameLine();

		if (ImGui::Button("CopyAllLog"))
		{
			ImGui::LogToClipboard();
		}

		ImGui::SameLine();

		LogFilter.Draw("LogFilter", -100.0f);
	}

	const char* TextBuffStart = LogBuf.begin();
	const char* TextBuffEnd = LogBuf.end();

	ImGui::Separator();

	ImGui::BeginChild("LogScrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

	//��־ɸѡ��
	if (LogFilter.IsActive())
	{
		for (int i = 0; i < LineOffsets.Size; i++)
		{
			const char* LineStart = TextBuffStart + LineOffsets[i];
			const char* LineEnd = (i + 1 < LineOffsets.Size) ? (TextBuffStart + LineOffsets[i + 1] - 1) : TextBuffEnd;

			if (LogFilter.PassFilter(LineStart, LineEnd))
			{
				ImGui::PushStyleColor(ImGuiCol_Text, GetColor(PreLineColor[i]));
				ImGui::TextUnformatted(LineStart, LineEnd);
				ImGui::PopStyleColor();
			}
		}
	}
	else
	{
		ImGuiListClipper Clipper;
		Clipper.Begin(LineOffsets.Size);
		while (Clipper.Step())
		{
			for (int i = Clipper.DisplayStart; i < Clipper.DisplayEnd; i++)
			{
				const char* LineStart = TextBuffStart + LineOffsets[i];
				const char* LineEnd = (i + 1 < LineOffsets.Size) ? (TextBuffStart + LineOffsets[i + 1] - 1) : TextBuffEnd;;

				ImGui::PushStyleColor(ImGuiCol_Text, GetColor(PreLineColor[i]));
				ImGui::TextUnformatted(LineStart, LineEnd);
				ImGui::PopStyleColor();
			}
		}

		Clipper.End();
	}

	//�ܿ������µ���־
	if (AutoScroll &&
		ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
	{
		ImGui::SetScrollHereY(1.0f);
	}

	ImGui::EndChild();
	ImGui::End();
}

void FLogSystem::AddLineColor(e_error InColor)
{
	PreLineColor.push_back(InColor);
}

void FLogSystem::ResetLineOffsets(e_error InColor, int InOldSize)
{
	LogBuf.append("\n");

	for (int i = LogBuf.size(); InOldSize < i; InOldSize++)
	{
		if (LogBuf[InOldSize] == '\n')
		{
			LineOffsets.push_back(InOldSize + 1);

			//��Ӷ��������ַ�
			AddLineColor(InColor);
		}
	}
}

ImVec4 FLogSystem::GetColor(e_error InColorID)
{
	switch (InColorID)
	{
	case SIMPLE_C_LOG:
		return ImVec4(248.f/255.f, 248.f/255.f, 248.f/255.f, 1.f);
	case SIMPLE_C_ERROR:
		return ImVec4(244.f/255.f, 67.f/255.f, 54.f/255.f, 1.f);
	case SIMPLE_C_WARNING:
		return ImVec4(103.f/255.f, 58.f/255.f, 183.f/255.f, 1.f);
	case SIMPLE_C_SUCCESS:
		return ImVec4(76.f/255.f, 175.f/255.f, 80.f/255.f, 1.f);
	}

	return ImVec4(0.4f, 0.4f, 0.4f, 1.f);
}

void FLogSystem::HandleBackstageLog(e_error InColorID, int InOldSize)
{
	//��ȡ��־����
	char error_str[64] = { 0 };
	get_error_str(InColorID, error_str);

	//�ϲ��ַ���
	// LogBuf.append(error_str);

	//�����ַ���
	ResetLineOffsets(InColorID, InOldSize);
}

FLogSystem::FLogSystem()
{
}

FLogSystem::~FLogSystem()
{
}
