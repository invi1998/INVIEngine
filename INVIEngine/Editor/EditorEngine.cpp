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
	Toolbar->DrawEditor();
	LogEditor->DrawEditor();
}

void CEditorEngine::ExitEditor()
{
	Toolbar->ExitEditor();
	LogEditor->ExitEditor();
}
