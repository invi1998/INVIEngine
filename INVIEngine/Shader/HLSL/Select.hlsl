// 射线选中物体的shader

#include "Material.hlsl"

struct MeshVertexIn
{
	float3 Position : POSITION; // 位置
	float4 Color : COLOR; // 颜色
	float3 Normal : NORMAL; // 法线
	float3 UTangent : TANGENT; // 切线（U方向）
	float2 Texcoord : TEXCOORD; // UV
};

struct MeshVertexOut
{
	float4 PositionH : SV_POSITION;
	float3 Normal : NORMAL;
	float2 Texcoord : TEXCOORD;
	float4 WorldPosition : POSITION;
};

MeshVertexOut VSMain(MeshVertexIn mv)
{
	MaterialConstBuffer MatConstBuffer = Materials[MaterialID];
	
	MeshVertexOut Out = (MeshVertexOut) 0;
	
	// 将模型的顶点坐标转为世界坐标
	Out.WorldPosition = mul(float4(mv.Position, 1.f), WorldMatrix);
	
	// 获取指向摄像机的向量
	float3 ViewDir = normalize(CameraPosition.xyz - Out.WorldPosition.xyz);
	
	// 将模型的顶点坐标沿着摄像机指向的方向移动一点，这样就可以让射线选中的物体在摄像机前面从而被渲染出来
	Out.WorldPosition.xyz += ViewDir * 0.01f;
	
	// 将世界坐标转为视口裁剪空间坐标
	Out.PositionH = mul(Out.WorldPosition, ViewportProjectionMatrix);
	
	// UV
    // 先将传入的uv坐标和模型的纹理变换相乘，得到纹理变换后的UV
	float4 TextureTexcoord = mul(float4(mv.Texcoord, 0.f, 1.f), TextureTransformationMatrix);
    // 然后再与材质的变换矩阵相乘，得到材质变换后的UV，就是我们最终的顶点UV了
	Out.Texcoord = mul(TextureTexcoord, MatConstBuffer.MaterialProjectionMatrix).xy;
	
	// 法线
	Out.Normal = mul(mv.Normal, (float3x3) WorldMatrix);
	
	return Out;

}


float4 PSMain(MeshVertexOut mvOut) : SV_TARGET
{
	float3 ModelNormal = normalize(mvOut.Normal);
	float3 ViewDirection = normalize(CameraPosition.xyz - mvOut.WorldPosition.xyz);
	float DotDiffValue = pow(1.f - max(dot(ModelNormal, ViewDirection), 0.0), 4.f);
	
	return float4(0.98f, 0.12f, 0.22f, 1.0f) * DotDiffValue;
}


