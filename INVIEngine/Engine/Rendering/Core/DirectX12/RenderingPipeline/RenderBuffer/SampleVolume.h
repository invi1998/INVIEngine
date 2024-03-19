#pragma once

// 采样体积
class FSampleVolume
{
public:
	FSampleVolume();
	void BuildVolume();

	void* GetVolumeData();

	void UpdateVolumeData(XMFLOAT4* Data);

protected:
	void RandomizeVolumeData();	// 随机化体积数据

protected:
	std::vector<XMFLOAT4> VolumeData{};	// 体积数据
};

