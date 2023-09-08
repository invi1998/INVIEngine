
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

//float2 tri(in float2 x)
//{
//    float2 h = frac(x * 0.5f) - 0.5f;
//    return 1.f - 2.f * abs(h);
//}

//float checkersGrid(float2 uv, float2 ddx, float2 ddy)
//{
//    float2 w = max(abs(ddx), abs(ddy)) + 0.01f;
//    float2 i = (tri(uv + 0.5 * w) - tri(uv - 0.5 * w)) / w;
//    return 0.5 - 0.5 * i.x * i.y;
//}


MeshVertexOut VSMain(MeshVertexIn mv)
{
	MeshVertexOut outV;

	// ��ȡ�������������
    outV.WorldPosition = mul(float4(mv.Position, 1.0f), WorldMatrix);
    
    // ��ģ��ת����βü��ռ�
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
    // material.BaseColor = float4(0.82f, 0.82f, 0.82f, 1.0f);
    
    float DotDiffValue = 0.f;
    
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
        // reflect hlsl����������ߵķ����ĺ����������������㷴������
        float3 ReflectDirection = normalize(-reflect(NormalizeLightDirection, ModelNormal));
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
        float3 HalfDirection = normalize(NormalizeLightDirection + ViewDirection);
    
        // �����Blinn-phongֵ
        DotDiffValue = max(0.0f, dot(ModelNormal, HalfDirection));
        
        if (DotDiffValue > 0.f)
        {
            float MaterialShiniess = 1.f - saturate(MaterialRoughness);
            float M = MaterialShiniess * 100.f;
            
            Specular = pow(max(dot(HalfDirection, ModelNormal), 0.f), M);
        }

    }
    else if (MaterialType == 4)
    {
        // WrapLightģ�� ����Ƥ��ģ��
        
        // float WrapValue = 1.f;  // ��ֵΪ1��ʱ���ǰ������ز���
        float WrapValue = 2.5f;    // ��ֵԽ�ߣ�Ƥ��Ч��Խͨ͸
        float DiffueseReflection = dot(ModelNormal, NormalizeLightDirection);
        DotDiffValue = max(0.0f, (DiffueseReflection + WrapValue) / (1.f + WrapValue)); // [-1, 1]->[0.1]
        
    }
    else if (MaterialType == 5)
    {
        // Minnaertģ�ͣ��¹�ģ�ͣ�
        
        // ��һ�� dot(l, n) * dot(n, v)
        
        float3 ViewDirection = normalize(CameraPosition.xyz - mvOut.WorldPosition.xyz);
        
        float DotLight = max(dot(ModelNormal, NormalizeLightDirection), 0.f);
        float DotView = max(dot(ModelNormal, ViewDirection), 0.f);
        
        // DotDiffValue = DotLight * DotView;
        
        // �ڶ��� dot(l, n) * pow(dot(l, n) * dot(n, v), r)
        
        float MaterialShiniess = 1.f - saturate(MaterialRoughness);
        float M = MaterialShiniess * 10.f;
        
        DotDiffValue = saturate(DotLight * pow(DotLight * DotView, M));
        
        
    }
    else if (MaterialType == 6)
    {
        // Banded ������ͨ
        
        float DiffueseReflection = (dot(ModelNormal, NormalizeLightDirection) + 1.f) * 0.5f;
        
        // �ֲ�����
        float Layered = 4.f;
        
        DotDiffValue = floor(DiffueseReflection * Layered) / Layered;
        
    }
    else if (MaterialType == 7)
    {
        // GradualBanded  ������Ŀ�ͨЧ��
        
        // ������ɫ
        float4 GradualColor = { 0.87f, 0.12f, 0.6f, 1.f };
        
        float3 ViewDirection = normalize(CameraPosition.xyz - mvOut.WorldPosition.xyz);
        
        float LightDotValue = dot(ModelNormal, NormalizeLightDirection);
        
        float DiffueseReflection = (LightDotValue + 1.f) * 0.5f;
        
        // �ֲ�����
        float Layered = 8.f;
        
        DotDiffValue = floor(DiffueseReflection * Layered) / Layered;
        
        material.BaseColor = lerp(material.BaseColor, GradualColor, LightDotValue);
        
    }
    else if (MaterialType == 8)
    {
        // CustomBanded  �Զ��忨ͨЧ��
        
        float3 ViewDirection = normalize(CameraPosition.xyz - mvOut.WorldPosition.xyz);
        
        float DiffueseReflection = (dot(ModelNormal, NormalizeLightDirection) + 1.f) * 0.5f;
        
        // �ֲ�����
        float Layered = 4.f;
        
        DotDiffValue = floor(DiffueseReflection * Layered) / Layered;
        
        // ��������Ч��
        float3 f0 = { 0.02f, 0.02f, 0.02f };
        Specular.xyz = FresnelSchlick(f0, ModelNormal, ViewDirection, 2);
        
        // ��ȡ���ߺ�������ӽǵİ������
        float3 HalfDirection = normalize(NormalizeLightDirection + ViewDirection);
        
        // ����߹�
        if (DotDiffValue > 0.f)
        {
            float MaterialShiniess = 1.f - saturate(MaterialRoughness);
            float M = MaterialShiniess * 70.f;
            
            Specular += pow(max(dot(HalfDirection, ModelNormal), 0.f), M) / 0.032f;
        }
        
    }
    else if (MaterialType == 100)
    {
        // �����
        float3 ViewDirection = normalize(CameraPosition.xyz - mvOut.WorldPosition.xyz);
        
        float3 f0 = { 0.02f, 0.02f, 0.02f };
        Specular.xyz = FresnelSchlick(f0, ModelNormal, ViewDirection, 2);
    }
 
    // ������ɫ����
    // material.BaseColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    mvOut.Color = material.BaseColor * DotDiffValue // ������
    + material.BaseColor * AmbientLight // ��ӹ⣨�����⣩
    + material.BaseColor * Specular; // �߹�
    
	// ٤��У��
    // mvOut.Color = sqrt(mvOut.Color);
    
    return mvOut.Color;
    
    // return CameraPosition;

    // return BaseColor;

}
