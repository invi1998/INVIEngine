#include "EngineMinimal.h"
#include "LogEditor.h"

#include "imgui.h"

FLogEditor::FLogEditor()
{
}

FLogEditor::~FLogEditor()
{
}

void FLogEditor::BuildEditor()
{
}

void FLogEditor::DrawEditor()
{
	ImGui::SetNextWindowPos(ImVec2(500, 400), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::Begin("Log");

	ImGui::End();

}

void FLogEditor::ExitEditor()
{
}

