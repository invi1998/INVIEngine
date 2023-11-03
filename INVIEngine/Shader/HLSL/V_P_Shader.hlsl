#include "Material.hlsl"
#include "PBR.hlsl"

struct MeshVertexIn
{
	float3 Position : POSITION; // λ��
	float4 Color : COLOR; // ��ɫ
	float3 Normal : NORMAL; // ����
	float3 UTangent : TANGENT; // ���ߣ�U����
	float2 Texcoord : TEXCOORD; // UV
};

struct MeshVertexOut
{
	float4 WorldPosition : POSITION;
	float4 Position : SV_POSITION;
	float4 Color : COLOR;
	float3 Normal : NORMAL;
	float3 UTangent : TANGENT; // ���ߣ�U����
	float2 Texcoord : TEXCOORD; // UV
};


MeshVertexOut VSMain(MeshVertexIn mv)
{
	MaterialConstBuffer MatConstbuffer = Materials[MaterialID];
	
	MeshVertexOut outV;

	// ��ȡ�������������
    outV.WorldPosition = mul(float4(mv.Position, 1.0f), MaterialTransformationMatrix);
    
    // ��ģ��ת����βü��ռ�
    outV.Position = mul(outV.WorldPosition, ViewportProjectionMatrix);
    
	if (MatConstbuffer.MaterialType == 13)
    {
        // �ֲ�����
        outV.Normal = mv.Normal;
    }
    else
    {
		// ���編�� (�����߿ռ�ת������ռ䣩
        outV.Normal = mul(mv.Normal, (float3x3) MaterialTransformationMatrix);
    }

    // ���� (�����߿ռ�ת������ռ䣩
	outV.UTangent = mul(mv.UTangent, (float3x3) MaterialTransformationMatrix);

    // ��ɫ
    outV.Color = mv.Color;
    
    // UV
    // �Ƚ������uv�����ģ�͵�����任��ˣ��õ�����任���UV
    float4 TextureTexcoord = mul(float4(mv.Texcoord, 0.f, 1.f), TextureTransformationMatrix);
    // Ȼ��������ʵı任������ˣ��õ����ʱ任���UV�������������յĶ���UV��
    outV.Texcoord = mul(TextureTexcoord, MatConstbuffer.MaterialProjectionMatrix).xy;

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
	MaterialConstBuffer MatConstbuffer = Materials[MaterialID];
	
    FMaterial material;
	
	
	material.BaseColor = GetMaterialBaseColor(MatConstbuffer, mvOut.Texcoord);
   
    
    float3 ModelNormal = normalize(mvOut.Normal);
    
    
	if (MatConstbuffer.MaterialType == 12)
    {
        return material.BaseColor;
    }
	if (MatConstbuffer.MaterialType == 13)
    {
        // ��ʾ���㷨��
        return float4(ModelNormal, 1.0f);
    }
	if (MatConstbuffer.MaterialType == 14)
    {
        // ��ʾ���編��
        return float4(ModelNormal, 1.0f);
    }
    
    // ������
    float4 AmbientLight = { 0.15f, 0.15f, 0.22f, 1.0f };
    
    float DotDiffValue = 0.f;
    
	float4 Specular = GetMaterialSpecular(MatConstbuffer, mvOut.Texcoord);
    float4 LightStrength = { 0.f, 0.f, 0.f, 1.f };
	
	// ��ȡ���ߣ���������˷�����ͼ������Ʒ�����ͼ
	ModelNormal = GetMaterialNormal(MatConstbuffer, mvOut.Texcoord, ModelNormal, mvOut.UTangent);
	
	// return float4(mvOut.UTangent, 1.0f);
	// return MatConstbuffer.BaseColor;
	
	// return MatConstbuffer.BaseColor * (SceneLights[1].LightIntensity, 1.0f);
    
    for (int i = 0; i < 16; i++)
    {
        if (length(SceneLights[i].LightIntensity.xyz) > 0.f)
        {
			
            float3 NormalizeLightDirection = normalize(GetLightDirection(SceneLights[i], mvOut.WorldPosition.xyz));
            
            float4 LightStrengthTemp = CaculateLightStrength(SceneLights[i], ModelNormal, mvOut.WorldPosition.xyz, NormalizeLightDirection);

			if (MatConstbuffer.MaterialType == 0)
            {
                // �����ز���
                DotDiffValue = pow(max(dot(ModelNormal, NormalizeLightDirection), 0.f), 2.f);
            }
			else if (MatConstbuffer.MaterialType == 1)
            {
                // �������ز���
                float DiffueseReflection = dot(ModelNormal, NormalizeLightDirection);
                DotDiffValue = max(0.0f, (DiffueseReflection * 0.5f + 0.5f)); // [-1, 1]->[0.1]
            }
			else if (MatConstbuffer.MaterialType == 2)
            {
                // Phong
                // reflect hlsl����������ߵķ����ĺ����������������㷴������
                float3 ReflectDirection = normalize(-reflect(NormalizeLightDirection, ModelNormal));
                float3 ViewDirection = normalize(CameraPosition.xyz - mvOut.WorldPosition.xyz);
        
                DotDiffValue = pow(max(dot(ModelNormal, NormalizeLightDirection), 0.0f), 2.f);
        
				float MaterialShiniess = 1.f - saturate(MatConstbuffer.MaterialRoughness);
                float M = MaterialShiniess * 100.f;
            
				Specular *= saturate(pow(max(dot(ViewDirection, ReflectDirection), 0.f), M)/3.1415926f);
			}
			else if (MatConstbuffer.MaterialType == 3)
            {
                // Blinn-Phong
        
                // ��ȡ����������ص������
                float3 ViewDirection = normalize(CameraPosition.xyz - mvOut.WorldPosition.xyz);
                // ��ȡ���ߺ�������ӽǵİ������
                float3 HalfDirection = normalize(NormalizeLightDirection + ViewDirection);
    
                // �����Blinn-phongֵ
				// �Ȱ������ػ� �ټ�ȥ0.2f�ع�ģ���ƽ������ø����
                DotDiffValue = pow(max(0.0f, dot(ModelNormal, HalfDirection) * 0.5f + 0.5f)-0.2f, 2.f);
        
				float MaterialShiniess = 1.f - saturate(MatConstbuffer.MaterialRoughness);
                float M = MaterialShiniess * 100.f;
                
                // c=(m+2.f/PI) blinnPhong��һ��ϵ��
				Specular *= saturate((M + 2.f) * pow(max(dot(HalfDirection, ModelNormal), 0.f), M) / 3.1415926f);

            }
			else if (MatConstbuffer.MaterialType == 4)
            {
                // WrapLightģ�� ����Ƥ��ģ��
        
                // float WrapValue = 1.f;  // ��ֵΪ1��ʱ���ǰ������ز���
                float WrapValue = 2.6f; // ��ֵԽ�ߣ�Ƥ��Ч��Խͨ͸
                float DiffueseReflection = dot(ModelNormal, NormalizeLightDirection);
                DotDiffValue = max(0.0f, (DiffueseReflection + WrapValue) / (1.f + WrapValue)); // [-1, 1]->[0.1]
        
            }
			else if (MatConstbuffer.MaterialType == 5)
            {
                // Minnaertģ�� ģ������������Ч��
        
                // ��һ�� dot(l, n) * dot(n, v)
        
                float3 ViewDirection = normalize(CameraPosition.xyz - mvOut.WorldPosition.xyz);
        
                float DotLight = pow(max(dot(ModelNormal, NormalizeLightDirection), 0.f), 2.f);
                float DotView = max(dot(ModelNormal, ViewDirection), 0.f);
        
                // DotDiffValue = DotLight * DotView;
        
                // �ڶ��� dot(l, n) * pow(dot(l, n) * dot(n, v), r)
        
				float MaterialShiniess = 1.f - saturate(MatConstbuffer.MaterialRoughness);
                float M = MaterialShiniess * 10.f;
        
                DotDiffValue = saturate(DotLight * pow(DotLight * DotView, M));
        
        
            }
			else if (MatConstbuffer.MaterialType == 6)
            {
                // Banded ������ͨ
                if (i == 0)
                {
                    float DiffueseReflection = (dot(ModelNormal, NormalizeLightDirection) + 1.f) * 0.5f;
        
                    // �ֲ�����
                    float Layered = 4.f;
        
                    DotDiffValue = floor(DiffueseReflection * Layered) / Layered;
                }
            }
			else if (MatConstbuffer.MaterialType == 7)
            {
                // GradualBanded  ������Ŀ�ͨЧ��
                
                if (i == 0)
                {
                    // ������ɫ
                    float4 GradualColor = { 0.87f, 0.12f, 0.6f, 1.f };
        
                    float LightDotValue = dot(ModelNormal, NormalizeLightDirection);
        
                    float DiffueseReflection = (LightDotValue + 1.f) * 0.5f;
        
                    // �ֲ�����
                    float Layered = 8.f;
        
                    DotDiffValue = floor(DiffueseReflection * Layered) / Layered;
        
                    material.BaseColor = lerp(material.BaseColor, GradualColor, LightDotValue);
                }
            }
			else if (MatConstbuffer.MaterialType == 8)
            {
                // CustomBanded  �Զ��忨ͨЧ��
                
                if (i == 0)
                {
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
						float MaterialShiniess = 1.f - saturate(MatConstbuffer.MaterialRoughness);
                        float M = MaterialShiniess * 70.f;
            
						Specular += saturate(pow(max(dot(HalfDirection, ModelNormal), 0.f), M) / 0.032f);
					}
                }
            }
			else if (MatConstbuffer.MaterialType == 9)
            {
                // Back ��ʯ����

                float3 ViewDirection = normalize(CameraPosition.xyz - mvOut.WorldPosition.xyz);

                // ģ��͸�����

                // ��ʯ��������㷨��ʹ��BlinnPhong��ģ�⣩
                // wrap
                float WrapValue = 1.2f; // ��ֵԽ�ߣ�Ƥ��Ч��Խͨ͸
                float DiffueseReflection = dot(ModelNormal, NormalizeLightDirection);
                DotDiffValue = max(0.0f, (DiffueseReflection + WrapValue) / (1.f + WrapValue)); // [-1, 1]->[0.1]

                // ��ȡ���ߺ�������ӽǵİ������
                float3 HalfDirection = normalize(NormalizeLightDirection + ViewDirection);

                // �����Blinn-phongֵ
                if (DotDiffValue > 0.f)
                {
					float MaterialShiniess = 1.f - saturate(MatConstbuffer.MaterialRoughness);
                    float M = MaterialShiniess * 100.f;

					Specular = saturate(pow(max(dot(HalfDirection, ModelNormal), 0.f), M));
				}

                // ��ӷ����Ч��
                /*float3 f0 = { 0.02f, 0.02f, 0.02f };
                Specular.xyz += FresnelSchlick(f0, ModelNormal, ViewDirection, 2);*/


                // ������͸��ģ��
                // ����ȡ��
		        // ����ȡ�� Ȼ�� ����һ��������͸��ֵ(sss)
		        // ��ȡ����ͷ����İ������
		        // ������������������Ҫģ���͸��Ч��
                float SSSValue = 1.55f;
                float TransmissionScale = 1.5f; // ͸�䷶Χ
                float TransmissionIntensity = 2.5f; // ͸��ǿ��

                float3 BackLightNormalValue = -normalize(ModelNormal * SSSValue + NormalizeLightDirection);

                // pow ��£�����ǿ
				DotDiffValue += pow(saturate(dot(BackLightNormalValue, ViewDirection)), TransmissionScale) * TransmissionIntensity;

        
            }
			else if (MatConstbuffer.MaterialType == 10)
            {
	            // �������ԣ�ͷ��ģ��
            }
			else if (MatConstbuffer.MaterialType == 11)
            {
	            // OrenNayar GDC�ֲڱ���
        
                float3 ViewDirection = normalize(CameraPosition.xyz - mvOut.WorldPosition.xyz);
                float3 NormalLight = saturate(pow(max(0.f, dot(ModelNormal, NormalizeLightDirection)), 2.f));
                float3 NormalView = saturate(dot(ModelNormal, ViewDirection));
        
                float PhiR =
                length(ViewDirection - ModelNormal * NormalView) * // �ӽǵ����ߵľ���
                length(NormalizeLightDirection - ModelNormal * NormalLight); // �ƹ⵽���ߵľ���
        
                // ������Կ��������ǵķ��߳�����һ�� NormalView ֵ��ModelNormal * NormalView �Ľ������ʵ���˶Է��ߵ�һ������
        
                float ACosNormalView = acos(NormalView); // [0, 1]
                float ACosNormalLight = acos(NormalLight);
        
                float Alpha = max(ACosNormalView, ACosNormalLight);
                float Beta = min(ACosNormalView, ACosNormalLight);
        
				float Roughness = pow(MatConstbuffer.MaterialRoughness, 2); // �ֲڶ�
        
                float A = 1 - 0.5f * (Roughness / (Roughness + 0.33f));
                float B = 0.45f * (Roughness / (Roughness + 0.09f));
        
        
                DotDiffValue = NormalLight * (A + B * max(0, PhiR) * sin(Alpha) * tan(Beta));
        
        
            }
			else if (MatConstbuffer.MaterialType == 20)
            {
                // PBR ������ʵ����Ĳ�����Ⱦ
                float3 L = NormalizeLightDirection;
                float3 V = normalize(CameraPosition.xyz - mvOut.WorldPosition.xyz);
        
                float3 H = normalize(V + L);
        
                float3 N = ModelNormal;
        
                float PI = 3.1415926f;
        
                float Roughness = 0.02f; // �ֲڶ�
                float Matallic = 0.2f; // ������
        
                // D �� D_GGX
                float4 D = GetDistributionGGX(N, H, Roughness);
        
                float F0 = 0.04f;
                F0 = lerp(F0, material.BaseColor, Matallic);
        
                // ������� F��
                float4 F = float4(FresnelSchlick(F0, N, V, 5), 1.0f);
        
                // G �� ���κ�����
                float4 G = GSmith(N, V, L, Roughness);
        
                // ��ȡ��������
                float4 Kd = 1 - F; // ���Ƿ����ȡ��
                Kd *= 1 - Matallic;
        
                float4 Diffuse = float4(Kd.xyz * GetDiffuseLambert(material.BaseColor.xyz), 1.f);
        
                float NoV = saturate(dot(N, V));
                float NoL = saturate(dot(N, L));
        
                float4 Value = (D * F * G) / (4 * (NoV * NoL));
        
                Specular *= float4(Value.rgb, 1.f);
        
                float3 Radiance = SceneLights[i].LightIntensity.xyz;
                // ������ * �߹� * NOL(�ʲ����ң�* ����ȣ�������ʱ�õƹ�ǿ�ȴ��棩
                float3 PBRColor = (Diffuse.xyz + Specular.xyz) * NoL * Radiance;
        
                return float4(PBRColor, 1.f);

            }
			else if (MatConstbuffer.MaterialType == 100)
            {
                // �����
                float3 ViewDirection = normalize(CameraPosition.xyz - mvOut.WorldPosition.xyz);
        
                float3 f0 = { 0.02f, 0.02f, 0.02f };
                Specular.xyz = FresnelSchlick(f0, ModelNormal, ViewDirection, 2);
            }

            LightStrength += saturate(LightStrengthTemp * DotDiffValue * float4(SceneLights[i].LightIntensity.xyz, 1.f));
            LightStrength.w = 1.f;

			// ����ֵ����Ϊ��0-1��
			LightStrength = saturate(LightStrength);
			Specular = saturate(Specular);
			material.BaseColor = saturate(material.BaseColor);
			
		}
    }

    // ������ɫ����
	// material.BaseColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    mvOut.Color = material.BaseColor * LightStrength // ������
        + material.BaseColor * Specular * LightStrength // �߹�
        + material.BaseColor * AmbientLight;  // ��ӹ⣨�����⣩

    // ٤��У��
    // mvOut.Color = sqrt(mvOut.Color);
    
    return mvOut.Color;
    
    // return CameraPosition;

    // return BaseColor;

}
