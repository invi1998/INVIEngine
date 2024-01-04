// 射线选中物体的shader

#include "Material.hlsl"

struct MeshVertexIn
{
	float3 Position : POSITION; // 位置
	float2 Texcoord : TEXCOORD; // UV
};

struct MeshVertexOut
{
	float4 PositionH : SV_POSITION;
	float2 Texcoord : TEXCOORD;
};

MeshVertexOut VSMain(MeshVertexIn mv)
{
	MaterialConstBuffer MatConstBuffer = Materials[MaterialID];
	
	MeshVertexOut Out = (MeshVertexOut) 0;
	
	// 将模型的顶点坐标转为世界坐标
	float4 PositionWorld = mul(float4(mv.Position, 1.f), WorldMatrix);
	
	// 获取指向摄像机的向量
	float3 ViewDir = normalize(CameraPosition.xyz - PositionWorld.xyz);
	
	// 将模型的顶点坐标沿着摄像机指向的方向移动一点，这样就可以让射线选中的物体在摄像机前面从而被渲染出来
	PositionWorld.xyz += ViewDir * 0.01f;
	
	// 将世界坐标转为视口裁剪空间坐标
	Out.PositionH = mul(PositionWorld, ViewportProjectionMatrix);
	
	// UV
    // 先将传入的uv坐标和模型的纹理变换相乘，得到纹理变换后的UV
	float4 TextureTexcoord = mul(float4(mv.Texcoord, 0.f, 1.f), TextureTransformationMatrix);
    // 然后再与材质的变换矩阵相乘，得到材质变换后的UV，就是我们最终的顶点UV了
	Out.Texcoord = mul(TextureTexcoord, MatConstBuffer.MaterialProjectionMatrix).xy;
	
	return Out;

}


float4 PSMain(MeshVertexOut mvOut) : SV_TARGET
{
	return float4(0.9f, 0.23f, 0.11f, 1.0f);
}


