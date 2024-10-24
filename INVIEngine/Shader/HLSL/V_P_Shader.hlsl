#include "Material.hlsl"
#include "PBR.hlsl"
#include "SkyFunction.hlsl"
#include "ShadowFunction.hlsl"

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
	float4 WorldPosition : POSITION0;	// 世界坐标
	float4 TexPositionHome : POSITION1;		// 纹理坐标
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
    outV.WorldPosition = mul(float4(mv.Position, 1.0f), WorldMatrix);
    
    // 将模型转到其次裁剪空间
    outV.Position = mul(outV.WorldPosition, ViewportProjectionMatrix);
	
	// 传入世界坐标，将其转换到纹理坐标（屏幕空间）
	outV.TexPositionHome = mul(outV.WorldPosition, TexViewProjectionMatrix);
    
	if (MatConstbuffer.MaterialType == 13)
    {
        // 局部法线
        outV.Normal = mv.Normal;
    }
    else
    {
		// 世界法线 (从切线空间转到世界空间）
		outV.Normal = mul(mv.Normal, (float3x3) NormalMatrix);
	}

    // 切线 (从切线空间转到世界空间）
	outV.UTangent = mul(mv.UTangent, (float3x3) NormalMatrix);

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
	
	// 归一化到屏幕空间（近景层面）
	mvOut.TexPositionHome /= mvOut.TexPositionHome.w;
	
	// 环境光可及率 （这里采样不能用纹理坐标，因为纹理坐标是世界坐标，这里需要屏幕空间坐标）
	float Accessibility = SimpleSSAOMap.Sample(TextureSampler, mvOut.TexPositionHome.xy).r;

	if (MatConstbuffer.MaterialType == 101)
	{
		// 渲染阴影贴图
		// return float4(SimpleShadowMap.Sample(TextureSampler, mvOut.Texcoord).rrr, 1.0f);
		
		// 渲染AO贴图
		// return float4(SimpleSSAOMap.Sample(TextureSampler, mvOut.Texcoord).rgb, 1.0f);
		
		return float4(Accessibility, Accessibility, Accessibility, 1.f);

	}
	
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
	
	// 获取法线，如果设置了法线贴图，则绘制法线贴图
	ModelNormal = GetMaterialNormal(MatConstbuffer, mvOut.Texcoord, ModelNormal, mvOut.UTangent);
	
	// return float4(mvOut.UTangent, 1.0f);
	// return MatConstbuffer.BaseColor;
	
	// return MatConstbuffer.BaseColor * (SceneLights[1].LightIntensity, 1.0f);
	
	float4 FinalColor = { 0.f, 0.f, 0.f, 1.f };
    
    for (int i = 0; i < 16; i++)
    {
        if (length(SceneLights[i].LightIntensity.xyz) > 0.f)
        {
			
            float3 NormalizeLightDirection = normalize(GetLightDirection(SceneLights[i], mvOut.WorldPosition.xyz));
            
			float4 LightStrength = CaculateLightStrength(SceneLights[i], ModelNormal, mvOut.WorldPosition.xyz, NormalizeLightDirection);

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
				float3 HalfDirection = normalize(ViewDirection + NormalizeLightDirection);
    
                // 计算出Blinn-phong值
				// 先半兰博特化 再减去0.2f曝光的，再平方，变得更柔和
				DotDiffValue = pow(max(0.0f, dot(ModelNormal, NormalizeLightDirection) * 0.5f + 0.5f) - 0.2f, 2.f);
        
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
        
                float Roughness = MatConstbuffer.MaterialRoughness; // 粗糙度
                float3 Matallic = MatConstbuffer.Metallicity; // 金属度
        
                // D 项 D_GGX
                float4 D = GetDistributionGGX(N, H, Roughness);
        
                float3 F0 = 0.04f;
				F0 = lerp(F0, MatConstbuffer.BaseColor.rgb, Matallic);
        
                // 菲尼尔项 F项
                float4 F = float4(FresnelSchlick(F0, N, V, 5), 1.0f);
        
                // G 项 几何函数项
                float4 G = GSmith(N, V, L, Roughness);
				
				float LoH = saturate(dot(L, H));
				float NoV = saturate(dot(N, V));
				float NoL = saturate(dot(N, L));
				
				float3 FIndirect = GetIndirectLight(LoH, F0, Roughness);
				float3 IndirKS = GetDirectLight(NoV, F0, Roughness);
        
                // 获取兰伯特项
				float4 Kd = 1 - float4(FIndirect, 1.f); // 就是菲尼尔取反
				
				Kd *= (1 - float4(IndirKS, 1.f)) * (1 - float4(Matallic, 1.f));
        
				float3 Diffuse = Kd.xyz * GetDiffuseLambert(MatConstbuffer.BaseColor.xyz);
        
                float4 Value = (D * F * G) / (4 * (NoV * NoL));
        
				Specular = float4(Value.rgb, 1.f);
        
                float3 Radiance = LightStrength.xyz;
                // 漫反射 * 高光 * NOL(朗博余弦）* 辐射度（这里暂时用灯光强度代替）
				float3 PBRColor = (Diffuse + Specular.xyz) * NoL * Radiance;
        
                return float4(PBRColor, 1.f);

            }
			else if (MatConstbuffer.MaterialType == 100)
            {
                //// 菲尼尔
                //float3 ViewDirection = normalize(CameraPosition.xyz - mvOut.WorldPosition.xyz);
        
                //float3 f0 = { 0.02f, 0.02f, 0.02f };
                //Specular.xyz = FresnelSchlick(f0, ModelNormal, ViewDirection, 2);
				
				//另一种菲尼尔方法
				float3 ViewDirection = normalize(CameraPosition.xyz - mvOut.WorldPosition.xyz);
				DotDiffValue = pow(1.f - max(dot(ModelNormal, ViewDirection), 0.0), 2.f);
			}
			
			float4 Diffuse = material.BaseColor;

			// 将数值限制为【0-1】
			Specular = saturate(Specular);
			
			//int IndexR = GetSampleCubeMapIndexR(mvOut.WorldPosition.xyz);
			//return DebugCubeVieport(IndexR);
			
			float ShadowFactor = 1.f;
			
			if (SceneLights[i].LightType == 1)
			{
				// 点光源
				// ShadowFactor = ProcessingOmnidirectinalSampleCubeMapShadow(mvOut.WorldPosition.xyz, SceneLights[i].LightPosition);
				ShadowFactor = ProcessingOmnidirectinalSampleCmpLevelZeroShadow(mvOut.WorldPosition.xyz, SceneLights[i].LightPosition);

			}
			else
			{
				// 平行光 聚光
				
				// 计算阴影
				// ShadowFactor = GetShadowFactor(mvOut.WorldPosition, SceneLights[i].ShadowTransform);
				// ShadowFactor = GetShadowFactor_PCF_Sample4(mvOut.WorldPosition, SceneLights[i].ShadowTransform);
				ShadowFactor = GetShadowFactor_PCF_Sample9(mvOut.WorldPosition, SceneLights[i].ShadowTransform);
			}
			
			
			
			
			FinalColor += ShadowFactor * (saturate((Diffuse + Specular) * LightStrength * DotDiffValue));
			
		}
    }
	
	float4 Ambient = (AmbientLight + 0.1f) * material.BaseColor * Accessibility;

    // 最终颜色贡献
	// material.BaseColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    mvOut.Color = FinalColor + Ambient; // 间接光（环境光）
	
	// 计算cube map反射
	switch (MatConstbuffer.MaterialType)
	{
		case 2:
		case 3:
		case 9:
		{
			// 计算反射
			float3 ReflectColor = GetReflectionColor(MatConstbuffer, ModelNormal, mvOut.WorldPosition.xyz);
	
			mvOut.Color = mvOut.Color + float4(ReflectColor, 1.f);
			
			break;
		}
		case 15: // 15
		{
			// 先计算折射
			float3 RefractColor = GetRefract(ModelNormal, mvOut.WorldPosition.xyz, MatConstbuffer.RefractiveValue);
			float3 SampleRefractColor = GetReflectionSampleColor(ModelNormal, RefractColor);
			
			// 计算反射
			float ReflectValue = GetReflect(ModelNormal, mvOut.WorldPosition.xyz);
			float3 SampleReflectionColor = GetReflectionSampleColor(ModelNormal, ReflectValue);
			
			
			// 计算Alpha混合通道
			float3 v = normalize(CameraPosition.xyz - mvOut.WorldPosition.xyz);
			float Shininess = GetShininess(MatConstbuffer);
			float3 FresnelFactor = FresnelSchlickFactor(MatConstbuffer, ModelNormal, v);
			
			// 通过alpha通道决定混合比例，越接近1，越倾向于反射，越接近0，越倾向于折射
			float3 Color = lerp(SampleRefractColor, SampleReflectionColor, pow(Shininess * FresnelFactor, 2));
			
			
			mvOut.Color.rgb += Color;
			
			break;
		}
	}
	
	
	if (MatConstbuffer.MaterialType == 15)
	{
		//透明的
		mvOut.Color.a = MatConstbuffer.Transparency;
	}
	else
	{
		mvOut.Color.a = material.BaseColor.a;
	}
	
	// 计算雾
	mvOut.Color = GetFogValue(mvOut.Color, mvOut.WorldPosition.xyz);

    // 伽马校正
    // mvOut.Color = sqrt(mvOut.Color);
    
    return mvOut.Color;
    
    // return CameraPosition;

    // return BaseColor;
	
	// return float4(MatConstbuffer.Param0, MatConstbuffer.Param1, MatConstbuffer.Param2, 1.f);	// debug

}
