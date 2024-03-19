#pragma once

// �������
class FSampleVolume
{
public:
	FSampleVolume();
	void BuildVolume();

	void* GetVolumeData();

	void UpdateVolumeData(XMFLOAT4* Data);

protected:
	void RandomizeVolumeData();	// ������������

protected:
	std::vector<XMFLOAT4> VolumeData{};	// �������
};

