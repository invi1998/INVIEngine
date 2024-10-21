#pragma once

class IGuidInterface
{
public:
	IGuidInterface();
	virtual ~IGuidInterface();

	bool operator==(const IGuidInterface& InOther) const
	{
		return guid_equal(&Guid, &InOther.Guid);
	}

	simple_c_guid GetGuid() const { return Guid; }


private:
	simple_c_guid Guid;
};

