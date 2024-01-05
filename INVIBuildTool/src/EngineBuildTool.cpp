#include "BuildPch.h"

#include "CodeReflection/CollectClassInfoFun/CollectClassInfo.h"

int main()
{
	FClassAnalysis classAnalysis;

	CollectClassInfo::Collection("../test.cpp", classAnalysis);
	return 0;
}
