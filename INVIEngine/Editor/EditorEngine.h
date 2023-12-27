#pragma once
#include "Core/EditorBase.h"
#include "Core/Engine.h"

class CEditorEngine : public CEngine, public FEditorBase
{
public:
	CEditorEngine();
	~CEditorEngine() override;
	
	void Draw();
	void BeginInit() override;
	int PreInit(FWinMainCommandParameters InParameters) override;
	int Init(FWinMainCommandParameters InParameters) override;
	int PostInit() override;
	void Tick(float DeltaTime) override;
	int PreExit() override;
	int PostExit() override;

	void BuildEditor() override;
	void DrawEditor() override;
	void ExitEditor() override;

protected:
	class FToolbar* Toolbar;
	class FLogEditor* LogEditor;
	
};

