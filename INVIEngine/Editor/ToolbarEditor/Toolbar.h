#pragma once
#include "Core/EditorBase.h"

class FToolbar : public FEditorBase
{
public:
	FToolbar();
	~FToolbar() override;
	void BuildEditor() override;
	void DrawEditor() override;
	void ExitEditor() override;

	void NewLevel();
};

