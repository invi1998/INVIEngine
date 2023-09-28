#pragma once

struct FObjectTransformation
{
	FObjectTransformation();

	XMFLOAT4X4 World;

	XMFLOAT4X4 TextureTransformation;
};

