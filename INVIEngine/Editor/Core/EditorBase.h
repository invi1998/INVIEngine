#pragma once

class FEditorBase
{
public:
	FEditorBase();
	virtual ~FEditorBase();

	virtual void BuildEditor() = 0;
	virtual void DrawEditor() = 0;
	virtual void ExitEditor() = 0;
};

