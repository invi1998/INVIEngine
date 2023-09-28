#pragma once

#if defined(_WIN32)

class FWinMainCommandParameters
{
public:
	FWinMainCommandParameters(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCMD);

	HINSTANCE HInstance;
	HINSTANCE PrevInstance;
	PSTR CMDLine;
	int ShowCMD;
};

#endif

