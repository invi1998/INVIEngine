
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

cbuffer LightConstBuffer : register(b3)
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

float2 tri(in float2 x)
{
    float2 h = frac(x * 0.5f) - 0.5f;
    return 1.f - 2.f * abs(h);
}

float checkersGrid(float2 uv, float2 ddx, float2 ddy)
{
    float2 w = max(abs(ddx), abs(ddy)) + 0.01f;
    float2 i = (tri(uv + 0.5 * w) - tri(uv - 0.5 * w)) / w;
    return 0.5 - 0.5 * i.x * i.y;
}


MeshVertexOut VSMain(MeshVertexIn mv)
{
	MeshVertexOut outV;

	// 将模型转到其次裁剪空间
    float4 Position = mul(float4(mv.Position, 1.0f), WorldMatrix);
    outV.Position = mul(Position, ViewportProjectionMatrix);

    outV.Normal = mul(mv.Normal, (float3x3) WorldMatrix);

	// 光照方向得取反
    float diff = max(dot(normalize(outV.Normal), normalize(-LightDirection)), 0.0f);

	// 环境光
    float4 ambient = float4(0.19f, 0.18f, 0.17f, 1.0f);

    outV.Color = mv.Color * diff + ambient;

	// 伽马校正
    outV.Color = sqrt(outV.Color);

	return outV;
}

//float3 render(float2 uv, in float2 px, in float2 py)
//{
//	// 平面
//    float3 rdx = normalize(ViewportProjectionMatrix * float3(px, fl));
//    float3 rdy = normalize(ViewportProjectionMatrix * float3(py, fl));

//    float3 ddx = ro.y * (rd / rd.y - rdx / rdx.y);
//    float3 ddy = ro.y * (rd / rd.y - rdy / rdy.y);

//    c = float3(0.) + float3(0.9) * checkersGrid(p.xz, ddx.xz, ddy.xz);
//}

float4 PSMain(MeshVertexOut mvOut) : SV_TARGET
{
    float4 ambientLight = { 0.15f, 0.15f, 0.22f, 1.0f };

    float ModelNormal = normalize(mvOut.Normal);
    float NormalizeLightDirection = normalize(-LightDirection);
    float DotDiffValue = max(dot(ModelNormal, NormalizeLightDirection), 0.0f);

    mvOut.Color = mvOut.Color * DotDiffValue + ambientLight;
	// 伽马校正
    mvOut.Color = sqrt(mvOut.Color);
    return mvOut.Color;

}
