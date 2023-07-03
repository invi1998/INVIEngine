
// 声明常量缓冲区(我们需要将程序里的常量缓冲区的数据寄存到寄存器里，寄存器有15个b0-b14，然后从寄存器里读取出来使用)
cbuffer ConstBuffer : register(b0)
{
	float4x4 World;
}

struct MeshVertexIn
{
	float3 Position : POSITION;
	float4 Color : COLOR;
};

struct MeshVertexOut
{
	float3 Position : VS_POSITION;
	float4 Color : COLOR;
};

MeshVertexOut VS_Main(MeshVertexIn mv)
{
	MeshVertexOut outV;

	// 将模型转到其次裁剪空间
	outV.Position = mul(float4(mv.Position, 1.0f), World);
	outV.Color = mv.Color;

	return outV;
}

float4 PS_Main(MeshVertexOut vsOut) : SV_TARGET
{
	return vsOut.Color;
}
