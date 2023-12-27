#include "EngineMinimal.h"
#include "EditorEngine.h"

#include "LogEditor/LogEditor.h"
#include "ToolbarEditor/Toolbar.h"

#include "imgui.h"

CEditorEngine::CEditorEngine()
{
	Toolbar = new FToolbar();
	LogEditor = new FLogEditor();
}

CEditorEngine::~CEditorEngine()
{
}

void CEditorEngine::Draw()
{
}

void CEditorEngine::BeginInit()
{
	CEngine::BeginInit();
}

int CEditorEngine::PreInit(FWinMainCommandParameters InParameters)
{
	return 0;
}

int CEditorEngine::Init(FWinMainCommandParameters InParameters)
{
	return 0;
}

int CEditorEngine::PostInit()
{
	return 0;
}

void CEditorEngine::Tick(float DeltaTime)
{
	CEngine::Tick(DeltaTime);
}

int CEditorEngine::PreExit()
{
	return 0;
}

int CEditorEngine::PostExit()
{
	return 0;
}

int CEditorEngine::Exit()
{
	return 0;
}

void CEditorEngine::BuildEditor()
{
	ImGuiIO& IO = ImGui::GetIO();
	IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	Toolbar->BuildEditor();
	LogEditor->BuildEditor();
}

void CEditorEngine::DrawEditor()
{
	DrawLayer();

	Toolbar->DrawEditor();
	LogEditor->DrawEditor();
}

void CEditorEngine::ExitEditor()
{
	Toolbar->ExitEditor();
	LogEditor->ExitEditor();
}

void CEditorEngine::DrawLayer()
{
	ImGuiDockNodeFlags DockspaceFlags = ImGuiDockNodeFlags_None;

	ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	const ImGuiViewport* Viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(Viewport->WorkPos);
	ImGui::SetNextWindowSize(Viewport->WorkSize);
	ImGui::SetNextWindowViewport(Viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	WindowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	WindowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	WindowFlags |= ImGuiWindowFlags_NoBackground;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	//背景改为透明
	DockspaceFlags |= ImGuiDockNodeFlags_PassthruCentralNode;

	bool bOpenWindows = true;
	ImGui::Begin("Hell", &bOpenWindows, WindowFlags);
	ImGui::PopStyleVar();
	ImGui::PopStyleVar(2);

	ImGuiIO& IO = ImGui::GetIO();
	//是不是开启吸附
	if (IO.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID DockspaceID = ImGui::GetID("EditorEngineLayer");
		ImGui::DockSpace(DockspaceID, ImVec2(0.0f, 0.0f), DockspaceFlags);
	}

	ImGui::End();

}
