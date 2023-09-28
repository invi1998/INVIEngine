#pragma once

struct FViewportTransformation
{
	FViewportTransformation();

	XMFLOAT4 CameraPosition;
	XMFLOAT4X4 ViewProjectionMatrix;
};

