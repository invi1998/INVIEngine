
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
    float xxs;
};

// 光线衰减计算方式1 (线性衰减）
float4 AttenuationPointLight(Light L, float LightDistence, float4 LightStrenth)
{
    float AttenuationRange = L.EndAttenuation - L.StartAttenuation;
    LightStrenth = LightStrenth * (LightDistence / AttenuationRange);
    return LightStrenth;
}

// 光线衰减计算方式2 (非线性平滑衰减）
// 常数项Kc、一次项Kl、二次项Kq
float4 AttenuationPointLightCLQ(Light L, float LightDistence, float4 LightStrenth)
{
    return (1.f / (L.Kc + L.Kl * LightDistence + L.Kq * pow(LightDistence, 2.f))) * LightStrenth;
}

float4 CaculateLightStrength(Light L, float3 PointNormal, float3 WorldLocation, float3 LightDirection)
{
    float4 LightStrenth = float4(L.LightIntensity, 1.f);
	if (L.LightType == 0)
	{
		// 平行光
	}
    else if(L.LightType == 1)
    {
	    // 点光

        float3 LightVector = L.LightPosition - WorldLocation;
        
        float LightDistence = length(LightVector);

        if (LightDistence < L.EndAttenuation)
        {
            // LightStrenth = AttenuationPointLight(L, LightDistence, LightStrenth);        // 线性衰减
            LightStrenth = AttenuationPointLightCLQ(L, LightDistence, LightStrenth);        // 非线性衰减
        }
        else
        {
            LightStrenth = float4(0.f, 0.f, 0.f, 1.f);

        }
    }

    return LightStrenth;

}


float3 GetLightDirection(Light L, float3 WorldLocation)
{
    float3 Direction;
    if (L.LightType == 0)
    {
		// 平行光
        Direction = -L.LightDirection;

    }
    else if (L.LightType == 1)
    {
	    // 点光
        Direction = L.LightPosition - WorldLocation;
       
    }

    return Direction;
}


