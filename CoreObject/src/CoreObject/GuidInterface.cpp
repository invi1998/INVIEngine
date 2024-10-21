#include "CoreObjectPch.h"

#include "GuidInterface.h"

IGuidInterface::IGuidInterface()
{
	create_guid(&Guid);
}

IGuidInterface::~IGuidInterface()
{
}
