#include "EngineMinimal.h"
#include "FogConstantBuffer.h"

FFogConstantBuffer::FFogConstantBuffer(): FogColor(XMFLOAT4(Colors::White)), FogStart(5.f), FogRange(100.f),
                                          FogHeight(100.f), FogTransparentCoefficient(0.f)
{
}
