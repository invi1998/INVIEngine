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
}

void FLogSystem::AddLog(const char* fmt, ...)
{
	int TextSize = LogBuf.size();

	va_list Args;
	va_start(Args, fmt);
	LogBuf.appendfv(fmt, Args);
	va_end(Args);

	LogBuf.append("\n");

	for (int i = LogBuf.size(); TextSize < i; TextSize++)
	{
		if (LogBuf[TextSize] == '\n')
		{
			LineOffsets.push_back(TextSize + 1);
		}
	}
}

void FLogSystem::Draw()
{
	if (!ImGui::Begin("Log"))
	{
		ImGui::End();
		return;
	}

	//按钮布局
	{
		if (ImGui::BeginPopup("LogOptions"))
		{
			ImGui::Checkbox("Auto Scroll", &AutoScroll);
			ImGui::EndPopup();
		}

		ImGui::SameLine();

		//需要一个按钮操作日志
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

	//日志筛选器
	if (LogFilter.IsActive())
	{
		for (int i = 0; i < LineOffsets.Size; i++)
		{
			const char* LineStart = TextBuffStart + LineOffsets[i];
			const char* LineEnd = (i + 1 < LineOffsets.Size) ? (TextBuffStart + LineOffsets[i + 1] - 1) : TextBuffEnd;

			if (LogFilter.PassFilter(LineStart, LineEnd))
			{
				ImGui::TextUnformatted(LineStart, LineEnd);
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

				ImGui::TextUnformatted(LineStart, LineEnd);
			}
		}

		Clipper.End();
	}

	//能看到最新的日志
	if (AutoScroll &&
		ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
	{
		ImGui::SetScrollHereY(1.0f);
	}

	ImGui::EndChild();
	ImGui::End();
}

FLogSystem::FLogSystem()
{
}

FLogSystem::~FLogSystem()
{
}
