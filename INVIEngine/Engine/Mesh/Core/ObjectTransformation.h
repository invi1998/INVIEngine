#pragma once

struct FObjectTransformation
{
	FObjectTransformation();

	XMFLOAT4X4 World;

	XMFLOAT4X4 TextureTransformation;

	UINT MaterialID;		// ²ÄÖÊid
	UINT rr1;
	UINT rr2;
	UINT rr3;
};

