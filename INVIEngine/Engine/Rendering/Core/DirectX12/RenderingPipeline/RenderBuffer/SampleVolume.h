#pragma once

// �������
class FSampleVolume
{
public:
	FSampleVolume();
	void BuildVolume();

	void* GetVolumeData();

protected:
	void RandomizeVolumeData();	// ������������

protected:
	std::vector<XMFLOAT4> VolumeData{};	// �������
};

