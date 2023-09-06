
#include "Light.hlsl"
#include "Material.hlsl"

cbuffer MeshConstBuffer : register(b0)
{
	// ��������������(������Ҫ��������ĳ��������������ݼĴ浽�Ĵ�����Ĵ�����15��b0-b14��Ȼ��ӼĴ������ȡ����ʹ��)
	float4x4 WorldMatrix;
}


cbuffer ViewportConstBuffer : register(b1)
{
	// ��������������(������Ҫ��������ĳ��������������ݼĴ浽�Ĵ�����Ĵ�����15��b0-b14��Ȼ��ӼĴ������ȡ����ʹ��)
    float4 CameraPosition;
	// �ӿ�ͶӰ����
	float4x4 ViewportProjectionMatrix;
    
}

cbuffer MaterialConstBuffer : register(b2)
{
	// ��������������(������Ҫ��������ĳ��������������ݼĴ浽�Ĵ�����Ĵ�����15��b0-b14��Ȼ��ӼĴ������ȡ����ʹ��)
    int MaterialType;       // ��������
    
    float4 BaseColor;       // ���ʻ�����ɫ
    float MaterialRoughness;    // ���ʴֲڶ�
	float4x4 MaterialProjectionMatrix;
}

cbuffer LightConstBuffer : register(b3)
{
	// ��������������(������Ҫ��������ĳ��������������ݼĴ浽�Ĵ�����Ĵ�����15��b0-b14��Ȼ��ӼĴ������ȡ����ʹ��)
	float3 LightIntensity;		// ����ǿ��
	float3 LightDirection;		// ���շ���
}

struct MeshVertexIn
{
	float3 Position : POSITION;		// λ��
	float4 Color : COLOR;			// ��ɫ
	float3 Normal : NORMAL;			// ����
};

struct MeshVertexOut
{
    float4 WorldPosition : POSITION;
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

	// ��ģ��ת����βü��ռ�
    outV.WorldPosition = mul(float4(mv.Position, 1.0f), WorldMatrix);
    outV.Position = mul(outV.WorldPosition, ViewportProjectionMatrix);

    outV.Normal = mul(mv.Normal, (float3x3) WorldMatrix);

    outV.Color = mv.Color;

	return outV;
}

//float3 render(float2 uv, in float2 px, in float2 py)
//{
//	// ƽ��
//    float3 rdx = normalize(ViewportProjectionMatrix * float3(px, fl));
//    float3 rdy = normalize(ViewportProjectionMatrix * float3(py, fl));

//    float3 ddx = ro.y * (rd / rd.y - rdx / rdx.y);
//    float3 ddy = ro.y * (rd / rd.y - rdy / rdy.y);

//    c = float3(0.) + float3(0.9) * checkersGrid(p.xz, ddx.xz, ddy.xz);
//}

float4 PSMain(MeshVertexOut mvOut) : SV_TARGET
{
    // ������
    float4 AmbientLight = { 0.15f, 0.15f, 0.22f, 1.0f };

    float3 ModelNormal = normalize(mvOut.Normal);
    float3 NormalizeLightDirection = normalize(-LightDirection);
    
    FMaterial material;
    material.BaseColor = BaseColor;
    
    float DotDiffValue = max(dot(ModelNormal, NormalizeLightDirection), 0.0f);

    // DotDiffValue = DotDiffValue * 0.5f + 0.5f;
    
    float4 Specular = { 0.f, 0.f, 0.f, 1.f };
    
    if (MaterialType == 0)
    {
        // �����ز���
        DotDiffValue = max(dot(ModelNormal, NormalizeLightDirection), 0.f);
    }
    else if (MaterialType == 1)
    {
        // �������ز���
        float DiffueseReflection = dot(ModelNormal, NormalizeLightDirection);
        DotDiffValue = max(0.0f, (DotDiffValue * 0.5f + 0.5f));     // [-1, 1]->[0.1]
    }
    else if (MaterialType == 2)
    {
        // Phong
        float3 ReflectDirection = normalize(-reflect(ModelNormal, NormalizeLightDirection));
        float3 ViewDirection = normalize(CameraPosition.xyz - mvOut.WorldPosition.xyz);
        
        DotDiffValue = max(dot(ModelNormal, NormalizeLightDirection), 0.0f);
        
        if (DotDiffValue > 0.f)
        {
            float MaterialShiniess = 1.f - saturate(MaterialRoughness);
            float M = MaterialShiniess * 100.f;
            
            Specular = pow(max(dot(ViewDirection, ReflectDirection), 0.f), M);
        }
    }
    else if (MaterialType == 3)
    {
        // Blinn-Phong
        
        // ��ȡ����������ص������
        float3 ViewDirection = normalize(CameraPosition.xyz - mvOut.WorldPosition.xyz);
        // ��ȡ���ߺ�������ӽǵİ������
        float3 HalfDirection = normalize(-LightDirection + ViewDirection);
    
        // �����Blinn-phongֵ
        DotDiffValue = max(0.0f, dot(mvOut.Normal, HalfDirection));
        
        if (DotDiffValue > 0.f)
        {
            float MaterialShiniess = 1.f - saturate(MaterialRoughness);
            float M = MaterialShiniess * 100.f;
            
            Specular = pow(max(dot(ViewDirection, ViewDirection), 0.f), M);
        }

    }
    
 
    // ������ɫ����
    mvOut.Color = material.BaseColor * DotDiffValue // ������
    + AmbientLight * material.BaseColor  // ��ӹ⣨�����⣩
    + Specular * material.BaseColor;    // �߹�
    
	// ٤��У��
    mvOut.Color = sqrt(mvOut.Color);
    
    return mvOut.Color;

}
