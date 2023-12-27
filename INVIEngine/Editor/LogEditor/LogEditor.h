#pragma once
#include "Core/EditorBase.h"

class FLogEditor : public FEditorBase
{
public:
	FLogEditor();
	~FLogEditor() override;

	void BuildEditor() override;
	void DrawEditor() override;
	void ExitEditor() override;
	
};

