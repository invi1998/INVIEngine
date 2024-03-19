#include "EngineMinimal.h"
#include "SampleVolume.h"

FSampleVolume::FSampleVolume()
{
	BuildVolume();
	RandomizeVolumeData();
}

void FSampleVolume::BuildVolume()
{
	VolumeData.clear();

	// 先构建一个立方体
	VolumeData.push_back({+1.0f, +1.0f, +1.0f, 0.0f});
	VolumeData.push_back({ -1.0f, -1.0f, -1.0f, 0.0f });

	VolumeData.push_back({ -1.0f, +1.0f, +1.0f, 0.0f });
	VolumeData.push_back({ +1.0f, -1.0f, -1.0f, 0.0f });

	VolumeData.push_back({ +1.0f, +1.0f, -1.0f, 0.0f });
	VolumeData.push_back({ -1.0f, -1.0f, +1.0f, 0.0f });

	VolumeData.push_back({ -1.0f, +1.0f, -1.0f, 0.0f });
	VolumeData.push_back({ +1.0f, -1.0f, +1.0f, 0.0f });

	// 构建6个面的中心点
	VolumeData.push_back({ -1.0f, 0.0f, 0.0f, 0.0f });
	VolumeData.push_back({ +1.0f, 0.0f, 0.0f, 0.0f });

	VolumeData.push_back({ 0.0f, -1.0f, 0.0f, 0.0f });
	VolumeData.push_back({ 0.0f, +1.0f, 0.0f, 0.0f });

	VolumeData.push_back({ 0.0f, 0.0f, -1.0f, 0.0f });
	VolumeData.push_back({ 0.0f, 0.0f, +1.0f, 0.0f });
}

void FSampleVolume::RandomizeVolumeData()
{
	if (VolumeData.size() == 0)
	{
		BuildVolume();
	}

	for (size_t i = 0; i < VolumeData.size(); i++)
	{
		const float randomLen = EngineMath::GetRandomNumberGenerator<float>(0.24f, 1.f);
		VolumeData[i].x *= randomLen;
		VolumeData[i].y *= randomLen;
		VolumeData[i].z *= randomLen;
		VolumeData[i].w *= randomLen;

		// 对结果进行归一化
		XMVECTOR temp = XMLoadFloat4(&VolumeData[i]);
		XMStoreFloat4(&VolumeData[i], XMVector4Normalize(temp));
	}
}

void* FSampleVolume::GetVolumeData()
{
	return VolumeData.data();
}

void FSampleVolume::UpdateVolumeData(XMFLOAT4* Data)
{
	memcpy(Data, VolumeData.data(), sizeof(XMFLOAT4) * VolumeData.size());
}
