#pragma once

#include "EngineMinimal.h"

enum EPressState
{
	Press,		// °´ÏÂ
	Release,	// ËÉ¿ª
};


struct FInputKey
{
	FInputKey();

	std::string KeyName;
	EPressState PressState;
};

