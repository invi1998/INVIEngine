#include "EngineMinimal.h"
#include "Toolbar.h"

#include "imgui.h"

FToolbar::FToolbar()
{
}

FToolbar::~FToolbar()
{
}

void FToolbar::BuildEditor()
{
}

void FToolbar::DrawEditor()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Level"))
			{
				NewLevel();
			}

			if (ImGui::MenuItem("Open Level"))
			{

			}

			if (ImGui::MenuItem("Open Asset"))
			{

			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "ctrl+z"))
			{

			}

			ImGui::EndMenu();
		}

		ImGui::Separator();

		if (ImGui::BeginMenu("Windows"))
		{
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void FToolbar::ExitEditor()
{
}

void FToolbar::NewLevel()
{
}
