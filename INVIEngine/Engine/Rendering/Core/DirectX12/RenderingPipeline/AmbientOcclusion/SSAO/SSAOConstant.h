#pragma once

struct FSSAOConstant
{
	FSSAOConstant();

	XMFLOAT4X4 InversiveProjectionMatrix{};	// ������ͶӰ����
	XMFLOAT4X4 ProjectionMatrix{};			// �������ͶӰ����
	XMFLOAT4X4 TextureProjectionMatrix{};		// ����ռ��ͶӰ����

	XMFLOAT4 OffsetVectors[14]{};	// 14���������
};

