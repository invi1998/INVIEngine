struct Light
{
    float3 LightIntensity;  // 光照强度
    float StartAttenuation; // 开始衰减

    float3 LightDirection;  // 光照方向
    float EndAttenuation;   // 结束衰减

    float3 LightPosition;    // 灯光位置
    int LightType;          // 灯光类型

    float Kc;               // 非线性衰减常数项
    float Kl;               // 非线性衰减一次项
    float Kq;               // 非线性衰减二次项
    float SpotInnerCornerPhi; // 指定了聚光半径的切光角。落在这个角度之外的物体都不会被这个聚光所照亮。

    float SpotOuterCornerTheta; // LightDir向量和SpotDir向量之间的夹角。在聚光内部的话theta值应该比phi值小
    float xxs1;
    float xxs2;
    float xxs3;
	
};

float3 GetLightDirection(Light L, float3 WorldLocation)
{
    if (L.LightType == 0)
    {
		// 平行光
        return L.LightDirection;
    }
    else if (L.LightType == 1)
    {
	    // 点光
        return L.LightPosition - WorldLocation;
    }
    else if (L.LightType == 2)
    {
	    // 聚光
        return L.LightPosition - WorldLocation;
    }
    return L.LightDirection;
}


// 光线衰减计算方式1 (线性衰减）
float4 AttenuationPointLight(Light L, float LightDistence, float4 LightStrenth)
{
    float AttenuationRange = L.EndAttenuation - L.StartAttenuation;
    LightStrenth = LightStrenth * (1.f - LightDistence / AttenuationRange);
    return LightStrenth;
}

// 光线衰减计算方式2 (非线性平滑衰减）
// 常数项Kc、一次项Kl、二次项Kq
float4 AttenuationPointLightCLQ(Light L, float LightDistence, float4 LightStrenth)
{
    return (1.f / (L.Kc + L.Kl * LightDistence + L.Kq * pow(LightDistence, 2.f))) * LightStrenth;
}

float4 CaculateLightStrength(Light L, float3 PointNormal, float3 WorldLocation, float3 NormalizeLightDirection)
{
    float4 LightStrenth = float4(1.f, 1.f, 1.f, 1.f) * float4(L.LightIntensity, 1.f);
	if (L.LightType == 0)
	{
		// 平行光
        return LightStrenth;
    }
    else if(L.LightType == 1)
    {
	    // 点光

        float3 LightVector = L.LightPosition - WorldLocation;
        
        float LightDistence = length(LightVector);

        if (LightDistence < L.EndAttenuation)
        {
            return AttenuationPointLight(L, LightDistence, LightStrenth);        // 线性衰减
            // return AttenuationPointLightCLQ(L, LightDistence, LightStrenth); // 非线性衰减
        }
    	return float4(0.f, 0.f, 0.f, 1.f);
    }
    else if (L.LightType == 2)
    {
	    // 聚光灯
        float3 LightVector = L.LightPosition - WorldLocation;
        float LightDistence = length(LightVector);

        if (LightDistence < L.EndAttenuation)
        {
            float DotValue = max(dot(NormalizeLightDirection, L.LightDirection), 0.f);

            float Theta = acos(DotValue);   // 算出角度
            if (Theta <= L.SpotInnerCornerPhi)
            {
                return LightStrenth;
            }
            if (Theta <= L.SpotOuterCornerTheta && Theta > L.SpotInnerCornerPhi)
            {
                float OuterInnerDistance = L.SpotOuterCornerTheta - L.SpotInnerCornerPhi;
                float CurrentDistance = OuterInnerDistance - (Theta - L.SpotInnerCornerPhi);

                return (CurrentDistance / OuterInnerDistance) * LightStrenth;
				// LightStrenth = AttenuationPointLightCLQ(L, CurrentDistance, LightStrenth); // 非线性衰减
            }
        }
    }
    return float4(0.f, 0.f, 0.f, 1.f);
}
