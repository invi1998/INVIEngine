#include "Material.hlsl"
#include "PBR.hlsl"

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
	float4 WorldPosition : POSITION;
	float4 Position : SV_POSITION;
	float4 Color : COLOR;
	float3 Normal : NORMAL;
	float3 UTangent : TANGENT; // 切线（U方向）
	float2 Texcoord : TEXCOORD; // UV
};


MeshVertexOut VSMain(MeshVertexIn mv)
{
	MaterialConstBuffer MatConstbuffer = Materials[MaterialID];
	
	MeshVertexOut outV;

	// 获取顶点的世界坐标
    outV.WorldPosition = mul(float4(mv.Position, 1.0f), MaterialTransformationMatrix);
    
    // 将模型转到其次裁剪空间
    outV.Position = mul(outV.WorldPosition, ViewportProjectionMatrix);
    
	if (MatConstbuffer.MaterialType == 13)
    {
        // 局部法线
        outV.Normal = mv.Normal;
    }
    else
    {
		// 世界法线 (从切线空间转到世界空间）
        outV.Normal = mul(mv.Normal, (float3x3) MaterialTransformationMatrix);
    }

    // 切线 (从切线空间转到世界空间）
	outV.UTangent = mul(mv.UTangent, (float3x3) MaterialTransformationMatrix);

    // 颜色
    outV.Color = mv.Color;
    
    // UV
    // 先将传入的uv坐标和模型的纹理变换相乘，得到纹理变换后的UV
    float4 TextureTexcoord = mul(float4(mv.Texcoord, 0.f, 1.f), TextureTransformationMatrix);
    // 然后再与材质的变换矩阵相乘，得到材质变换后的UV，就是我们最终的顶点UV了
    outV.Texcoord = mul(TextureTexcoord, MatConstbuffer.MaterialProjectionMatrix).xy;

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
        // 显示顶点法线
        return float4(ModelNormal, 1.0f);
    }
	if (MatConstbuffer.MaterialType == 14)
    {
        // 显示世界法线
        return float4(ModelNormal, 1.0f);
    }
    
    // 环境光
    float4 AmbientLight = { 0.15f, 0.15f, 0.22f, 1.0f };
    
    float DotDiffValue = 0.f;
    
	float4 Specular = GetMaterialSpecular(MatConstbuffer, mvOut.Texcoord);
    float4 LightStrength = { 0.f, 0.f, 0.f, 1.f };
	
	// 获取法线，如果设置了法线贴图，则绘制法线贴图
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
                // 兰伯特材质
                DotDiffValue = pow(max(dot(ModelNormal, NormalizeLightDirection), 0.f), 2.f);
            }
			else if (MatConstbuffer.MaterialType == 1)
            {
                // 半兰伯特材质
                float DiffueseReflection = dot(ModelNormal, NormalizeLightDirection);
                DotDiffValue = max(0.0f, (DiffueseReflection * 0.5f + 0.5f)); // [-1, 1]->[0.1]
            }
			else if (MatConstbuffer.MaterialType == 2)
            {
                // Phong
                // reflect hlsl中用于求光线的反射光的函数，可以用来计算反射向量
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
        
                // 获取摄像机到像素点的向量
                float3 ViewDirection = normalize(CameraPosition.xyz - mvOut.WorldPosition.xyz);
                // 获取光线和摄像机视角的半程向量
                float3 HalfDirection = normalize(NormalizeLightDirection + ViewDirection);
    
                // 计算出Blinn-phong值
				// 先半兰博特化 再减去0.2f曝光的，再平方，变得更柔和
                DotDiffValue = pow(max(0.0f, dot(ModelNormal, HalfDirection) * 0.5f + 0.5f)-0.2f, 2.f);
        
				float MaterialShiniess = 1.f - saturate(MatConstbuffer.MaterialRoughness);
                float M = MaterialShiniess * 100.f;
                
                // c=(m+2.f/PI) blinnPhong归一化系数
				Specular *= saturate((M + 2.f) * pow(max(dot(HalfDirection, ModelNormal), 0.f), M) / 3.1415926f);

            }
			else if (MatConstbuffer.MaterialType == 4)
            {
                // WrapLight模型 早期皮肤模拟
        
                // float WrapValue = 1.f;  // 该值为1的时候，是半兰伯特材质
                float WrapValue = 2.6f; // 该值越高，皮肤效果越通透
                float DiffueseReflection = dot(ModelNormal, NormalizeLightDirection);
                DotDiffValue = max(0.0f, (DiffueseReflection + WrapValue) / (1.f + WrapValue)); // [-1, 1]->[0.1]
        
            }
			else if (MatConstbuffer.MaterialType == 5)
            {
                // Minnaert模型 模拟月球和天鹅绒效果
        
                // 第一种 dot(l, n) * dot(n, v)
        
                float3 ViewDirection = normalize(CameraPosition.xyz - mvOut.WorldPosition.xyz);
        
                float DotLight = pow(max(dot(ModelNormal, NormalizeLightDirection), 0.f), 2.f);
                float DotView = max(dot(ModelNormal, ViewDirection), 0.f);
        
                // DotDiffValue = DotLight * DotView;
        
                // 第二种 dot(l, n) * pow(dot(l, n) * dot(n, v), r)
        
				float MaterialShiniess = 1.f - saturate(MatConstbuffer.MaterialRoughness);
                float M = MaterialShiniess * 10.f;
        
                DotDiffValue = saturate(DotLight * pow(DotLight * DotView, M));
        
        
            }
			else if (MatConstbuffer.MaterialType == 6)
            {
                // Banded 基础卡通
                if (i == 0)
                {
                    float DiffueseReflection = (dot(ModelNormal, NormalizeLightDirection) + 1.f) * 0.5f;
        
                    // 分层数量
                    float Layered = 4.f;
        
                    DotDiffValue = floor(DiffueseReflection * Layered) / Layered;
                }
            }
			else if (MatConstbuffer.MaterialType == 7)
            {
                // GradualBanded  带渐变的卡通效果
                
                if (i == 0)
                {
                    // 渐变颜色
                    float4 GradualColor = { 0.87f, 0.12f, 0.6f, 1.f };
        
                    float LightDotValue = dot(ModelNormal, NormalizeLightDirection);
        
                    float DiffueseReflection = (LightDotValue + 1.f) * 0.5f;
        
                    // 分层数量
                    float Layered = 8.f;
        
                    DotDiffValue = floor(DiffueseReflection * Layered) / Layered;
        
                    material.BaseColor = lerp(material.BaseColor, GradualColor, LightDotValue);
                }
            }
			else if (MatConstbuffer.MaterialType == 8)
            {
                // CustomBanded  自定义卡通效果
                
                if (i == 0)
                {
                    float3 ViewDirection = normalize(CameraPosition.xyz - mvOut.WorldPosition.xyz);
        
                    float DiffueseReflection = (dot(ModelNormal, NormalizeLightDirection) + 1.f) * 0.5f;
        
                    // 分层数量
                    float Layered = 4.f;
        
                    DotDiffValue = floor(DiffueseReflection * Layered) / Layered;
        
                    // 加入菲尼尔效果
                    float3 f0 = { 0.02f, 0.02f, 0.02f };
                    Specular.xyz = FresnelSchlick(f0, ModelNormal, ViewDirection, 2);
        
                    // 获取光线和摄像机视角的半程向量
                    float3 HalfDirection = normalize(NormalizeLightDirection + ViewDirection);
        
                    // 加入高光
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
                // Back 玉石材质

                float3 ViewDirection = normalize(CameraPosition.xyz - mvOut.WorldPosition.xyz);

                // 模拟透射光线

                // 玉石的面光面算法（使用BlinnPhong来模拟）
                // wrap
                float WrapValue = 1.2f; // 该值越高，皮肤效果越通透
                float DiffueseReflection = dot(ModelNormal, NormalizeLightDirection);
                DotDiffValue = max(0.0f, (DiffueseReflection + WrapValue) / (1.f + WrapValue)); // [-1, 1]->[0.1]

                // 获取光线和摄像机视角的半程向量
                float3 HalfDirection = normalize(NormalizeLightDirection + ViewDirection);

                // 计算出Blinn-phong值
                if (DotDiffValue > 0.f)
                {
					float MaterialShiniess = 1.f - saturate(MatConstbuffer.MaterialRoughness);
                    float M = MaterialShiniess * 100.f;

					Specular = saturate(pow(max(dot(HalfDirection, ModelNormal), 0.f), M));
				}

                // 添加菲尼尔效果
                /*float3 f0 = { 0.02f, 0.02f, 0.02f };
                Specular.xyz += FresnelSchlick(f0, ModelNormal, ViewDirection, 2);*/


                // 背光面透射模拟
                // 光线取反
		        // 法线取反 然后 乘以一个给定的透射值(sss)
		        // 获取反光和反法的半程向量
		        // 这个半程向量就是我们要模拟的透射效果
                float SSSValue = 1.55f;
                float TransmissionScale = 1.5f; // 透射范围
                float TransmissionIntensity = 2.5f; // 透射强度

                float3 BackLightNormalValue = -normalize(ModelNormal * SSSValue + NormalizeLightDirection);

                // pow 收拢折射光强
				DotDiffValue += pow(saturate(dot(BackLightNormalValue, ViewDirection)), TransmissionScale) * TransmissionIntensity;

        
            }
			else if (MatConstbuffer.MaterialType == 10)
            {
	            // 各向异性，头发模拟
            }
			else if (MatConstbuffer.MaterialType == 11)
            {
	            // OrenNayar GDC粗糙表面
        
                float3 ViewDirection = normalize(CameraPosition.xyz - mvOut.WorldPosition.xyz);
                float3 NormalLight = saturate(pow(max(0.f, dot(ModelNormal, NormalizeLightDirection)), 2.f));
                float3 NormalView = saturate(dot(ModelNormal, ViewDirection));
        
                float PhiR =
                length(ViewDirection - ModelNormal * NormalView) * // 视角到法线的距离
                length(NormalizeLightDirection - ModelNormal * NormalLight); // 灯光到法线的距离
        
                // 这里可以看到，我们的法线乘以了一个 NormalView 值，ModelNormal * NormalView 的结果就是实现了对法线的一个缩放
        
                float ACosNormalView = acos(NormalView); // [0, 1]
                float ACosNormalLight = acos(NormalLight);
        
                float Alpha = max(ACosNormalView, ACosNormalLight);
                float Beta = min(ACosNormalView, ACosNormalLight);
        
				float Roughness = pow(MatConstbuffer.MaterialRoughness, 2); // 粗糙度
        
                float A = 1 - 0.5f * (Roughness / (Roughness + 0.33f));
                float B = 0.45f * (Roughness / (Roughness + 0.09f));
        
        
                DotDiffValue = NormalLight * (A + B * max(0, PhiR) * sin(Alpha) * tan(Beta));
        
        
            }
			else if (MatConstbuffer.MaterialType == 20)
            {
                // PBR 基于真实物理的材质渲染
                float3 L = NormalizeLightDirection;
                float3 V = normalize(CameraPosition.xyz - mvOut.WorldPosition.xyz);
        
                float3 H = normalize(V + L);
        
                float3 N = ModelNormal;
        
                float PI = 3.1415926f;
        
                float Roughness = 0.02f; // 粗糙度
                float Matallic = 0.2f; // 金属度
        
                // D 项 D_GGX
                float4 D = GetDistributionGGX(N, H, Roughness);
        
                float F0 = 0.04f;
                F0 = lerp(F0, material.BaseColor, Matallic);
        
                // 菲尼尔项 F项
                float4 F = float4(FresnelSchlick(F0, N, V, 5), 1.0f);
        
                // G 项 几何函数项
                float4 G = GSmith(N, V, L, Roughness);
        
                // 获取兰伯特项
                float4 Kd = 1 - F; // 就是菲尼尔取反
                Kd *= 1 - Matallic;
        
                float4 Diffuse = float4(Kd.xyz * GetDiffuseLambert(material.BaseColor.xyz), 1.f);
        
                float NoV = saturate(dot(N, V));
                float NoL = saturate(dot(N, L));
        
                float4 Value = (D * F * G) / (4 * (NoV * NoL));
        
                Specular *= float4(Value.rgb, 1.f);
        
                float3 Radiance = SceneLights[i].LightIntensity.xyz;
                // 漫反射 * 高光 * NOL(朗博余弦）* 辐射度（这里暂时用灯光强度代替）
                float3 PBRColor = (Diffuse.xyz + Specular.xyz) * NoL * Radiance;
        
                return float4(PBRColor, 1.f);

            }
			else if (MatConstbuffer.MaterialType == 100)
            {
                // 菲尼尔
                float3 ViewDirection = normalize(CameraPosition.xyz - mvOut.WorldPosition.xyz);
        
                float3 f0 = { 0.02f, 0.02f, 0.02f };
                Specular.xyz = FresnelSchlick(f0, ModelNormal, ViewDirection, 2);
            }

            LightStrength += saturate(LightStrengthTemp * DotDiffValue * float4(SceneLights[i].LightIntensity.xyz, 1.f));
            LightStrength.w = 1.f;

			// 将数值限制为【0-1】
			LightStrength = saturate(LightStrength);
			Specular = saturate(Specular);
			material.BaseColor = saturate(material.BaseColor);
			
		}
    }

    // 最终颜色贡献
	// material.BaseColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    mvOut.Color = material.BaseColor * LightStrength // 漫反射
        + material.BaseColor * Specular * LightStrength // 高光
        + material.BaseColor * AmbientLight;  // 间接光（环境光）

    // 伽马校正
    // mvOut.Color = sqrt(mvOut.Color);
    
    return mvOut.Color;
    
    // return CameraPosition;

    // return BaseColor;

}
