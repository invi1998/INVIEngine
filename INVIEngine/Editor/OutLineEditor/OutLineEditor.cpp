#include "EngineMinimal.h"
#include "OutLineEditor.h"

#include "Actor/Core/ActorObject.h"
#include "Core/World.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/Geometry/RenderingData.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderLayer/RenderLayerManage.h"

void FOutLineEditor::BuildEditor()
{
}

extern int ActorSelected;	// 被选中的Actor的ID
void FOutLineEditor::DrawEditor()
{
	ImGui::Begin("Outline Editor");
	if (CWorld* world = GetWorld())
	{
		ImGui::BeginChild("ActorList", ImVec2(200, 0), true);

		const std::vector<GActorObject*>& actors = world->GetActors();

		for (int i = 0; i < actors.size(); ++i)
		{
			char objectName[256] = { 0 };
			sprintf_s(objectName, "%s##%d", actors[i]->GetName().c_str(), i);

			if (ImGui::Selectable(objectName, ActorSelected == i))
			{
				HighlightObject(actors[i]);
			}
		}
		ImGui::EndChild();
	}
	ImGui::End();
}

void FOutLineEditor::ExitEditor()
{
}

void FOutLineEditor::HighlightObject(GActorObject* Object)
{
	if (FRenderLayerManage* renderLayerManage = GetRenderLayerManager())
	{
		renderLayerManage->HighlightObject(Object);
	}
}
