#pragma once

#include "Core/Construction/ActorMeshConstruction.h"

#define CREATE_RENDER_DATA(T, ...)\
ActorMeshConstruction::CreateMeshRenderData<T>(GetMeshManage(), this, __VA_ARGS__);

