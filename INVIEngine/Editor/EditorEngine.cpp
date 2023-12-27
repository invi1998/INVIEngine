#include "EngineMinimal.h"
#include "EditorEngine.h"

CEditorEngine::CEditorEngine()
{
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
}

int CEditorEngine::Init(FWinMainCommandParameters InParameters)
{
}

int CEditorEngine::PostInit()
{
}

void CEditorEngine::Tick(float DeltaTime)
{
	CEngine::Tick(DeltaTime);
}

int CEditorEngine::PreExit()
{
}

int CEditorEngine::PostExit()
{
}

void CEditorEngine::BuildEditor()
{
}

void CEditorEngine::DrawEditor()
{
}

void CEditorEngine::ExitEditor()
{
}
