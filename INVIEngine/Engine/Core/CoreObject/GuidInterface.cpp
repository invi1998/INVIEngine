#include "EngineMinimal.h"

#include "GuidInterface.h"

IGuidInterface::IGuidInterface()
{
	create_guid(&Guid);
}

IGuidInterface::~IGuidInterface()
{
}
