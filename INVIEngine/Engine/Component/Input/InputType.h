#pragma once

enum EPressState
{
	Press,		// ����
	Release,	// �ɿ�
};


struct FInputKey
{
	FInputKey();

	std::string KeyName;
	EPressState PressState;
};

