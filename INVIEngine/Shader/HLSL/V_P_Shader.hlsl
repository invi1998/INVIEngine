
cbuffer MeshConstBuffer : register(b0)
{
	// 声明常量缓冲区(我们需要将程序里的常量缓冲区的数据寄存到寄存器里，寄存器有15个b0-b14，然后从寄存器里读取出来使用)
	float4x4 WorldMatrix;
}


cbuffer ViewportConstBuffer : register(b1)
{
	// 声明常量缓冲区(我们需要将程序里的常量缓冲区的数据寄存到寄存器里，寄存器有15个b0-b14，然后从寄存器里读取出来使用)
	// 视口投影矩阵
	float4x4 ViewportProjectionMatrix;
}

cbuffer MaterialConstBuffer : register(b2)
{
	// 声明常量缓冲区(我们需要将程序里的常量缓冲区的数据寄存到寄存器里，寄存器有15个b0-b14，然后从寄存器里读取出来使用)
	float4x4 MaterialProjectionMatrix;
}

cbuffer LightConstBuffer : register(b2)
{
	// 声明常量缓冲区(我们需要将程序里的常量缓冲区的数据寄存到寄存器里，寄存器有15个b0-b14，然后从寄存器里读取出来使用)
	float3 LightIntensity;		// 光照强度
	float3 LightDirection;		// 光照方向
}

struct MeshVertexIn
{
	float3 Position : POSITION;		// 位置
	float4 Color : COLOR;			// 颜色
	float3 Normal : NORMAL;			// 法线
};

struct MeshVertexOut
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR;
	float3 Normal : NORMAL;
};

MeshVertexOut VSMain(MeshVertexIn mv)
{
	MeshVertexOut outV;

	// 将模型转到其次裁剪空间
	float4 Position = mul(float4(mv.Position, 1.0f), WorldMatrix);
	outV.Position = mul(Position, ViewportProjectionMatrix);

    outV.Normal = normalize(mul(mv.Normal, (float3x3) WorldMatrix));

	// 光照方向得取反
    float diff = max(0.f, dot(outV.Normal, normalize(-LightDirection)));

	outV.Color = mv.Color * diff;

	return outV;
}

float4 PSMain(MeshVertexOut vsOut) : SV_TARGET
{
    // float4 color;
    return vsOut.Color;
}
