#include "EngineMinimal.h"

#include "WinMainCommandParameters.h"

FWinMainCommandParameters::FWinMainCommandParameters(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine,
	int showCMD) : HInstance(hInstance), PrevInstance(prevInstance), CMDLine(cmdLine), ShowCMD(showCMD)
{
}

