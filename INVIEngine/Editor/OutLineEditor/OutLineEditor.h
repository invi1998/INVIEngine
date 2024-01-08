#pragma once
#include "Core/EditorBase.h"
#include "Interface/DirectXDeviceInterface.h"

class FOutLineEditor : public FEditorBase, public IDirectXDeviceInterface
{
public:
	void BuildEditor() override;
	void DrawEditor() override;
	void ExitEditor() override;

	void HighlightObject(class GActorObject* Object);
};

