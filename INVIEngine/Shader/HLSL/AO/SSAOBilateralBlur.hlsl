#include "AOFunc.hlsl"

struct MeshVertexOutBlur
{
	float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD;
};

MeshVertexOutBlur VSMain(uint VertexID : SV_VertexID)
{
	MeshVertexOutBlur Out = (MeshVertexOutBlur) 0.f;

	Out.TexCoord = TextureCoordinates[VertexID];

	//0-1  NDC¿Õ¼ä 
	Out.Position = float4(2.f * Out.TexCoord.x - 1.f, 1.f - 2.f * Out.TexCoord.y, 0.f, 1.f);

	return Out;
}

float4 PSMain(MeshVertexOutBlur mvOut) : SV_TARGET
{
	float3 tempColor = SampleAcceptMap.SampleLevel(TextureSampler, mvOut.TexCoord, 0).rgb;
	return float4(0.f, 0.f, 0.f, 0.f);
}
