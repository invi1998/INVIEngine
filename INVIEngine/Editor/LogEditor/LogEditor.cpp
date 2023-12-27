#include "EngineMinimal.h"
#include "LogEditor.h"

#include "imgui.h"
#include "LogObject/LogSystem.h"

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

	FLogSystem::GetLog()->Draw();

	ImGui::End();

}

void FLogEditor::ExitEditor()
{
	FLogSystem::Destroy();
}

