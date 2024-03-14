#include "EngineMinimal.h"
#include "DefaultDirectXRootSignature.h"

FDefaultDirectXRootSignature::FDefaultDirectXRootSignature()
{
}

void FDefaultDirectXRootSignature::BuildRootSignature(UINT textureNum)
{
	FDirectXRootSignature::BuildRootSignature(textureNum);
}

void FDefaultDirectXRootSignature::PreDraw(float DeltaTime)
{
	FDirectXRootSignature::PreDraw(DeltaTime);
}

void FDefaultDirectXRootSignature::Draw(float DeltaTime)
{
	FDirectXRootSignature::Draw(DeltaTime);
}

void FDefaultDirectXRootSignature::PostDraw(float DeltaTime)
{
	FDirectXRootSignature::PostDraw(DeltaTime);
}
