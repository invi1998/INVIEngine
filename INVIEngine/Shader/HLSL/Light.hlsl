
struct Light
{
    float3 LightIntensity;  // 光照强度
    float StartAttenuation; // 开始衰减
    float3 LightDirection;  // 光照方向
    float EndAttenuation;   // 结束衰减
    float3 LightPosition;    // 灯光位置
    int LightType;          // 灯光类型
};

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
            float AttenuationRange = L.EndAttenuation - L.StartAttenuation;
        	LightStrenth = LightStrenth * (LightDistence / AttenuationRange);
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

