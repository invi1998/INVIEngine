#pragma once

struct FObjectTransformation
{
	FObjectTransformation();

	XMFLOAT4X4 World;

	XMFLOAT4X4 TextureTransformation;

	XMFLOAT4X4 NormalTransformation;	// ���߱任����

	UINT MaterialID;		// ����id
	UINT rr1;
	UINT rr2;
	UINT rr3;
};

