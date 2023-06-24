// Copyright (C) RenZhai.2022.All Rights Reserved.
#pragma once
#include "assert.h" 
#include "../../../simple_library_macro.h"

struct SIMPLE_LIBRARY_API fvector_3id
{
	int x;
	int y;
	int z;

public:
	fvector_3id();
	fvector_3id(int in_value);
	fvector_3id(int in_a, int in_b, int in_c);
};