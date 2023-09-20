
struct Light
{
    float3 LightIntensity;  // ����ǿ��
    float StartAttenuation; // ��ʼ˥��
    float3 LightDirection;  // ���շ���
    float EndAttenuation;   // ����˥��
    float3 LightPosition;    // �ƹ�λ��
    int LightType;          // �ƹ�����
    float Kc;               // ������˥��������
    float Kl;               // ������˥��һ����
    float Kq;               // ������˥��������
    float xxs;
};

// ����˥�����㷽ʽ1 (����˥����
float4 AttenuationPointLight(Light L, float LightDistence, float4 LightStrenth)
{
    float AttenuationRange = L.EndAttenuation - L.StartAttenuation;
    LightStrenth = LightStrenth * (LightDistence / AttenuationRange);
    return LightStrenth;
}

// ����˥�����㷽ʽ2 (������ƽ��˥����
// ������Kc��һ����Kl��������Kq
float4 AttenuationPointLightCLQ(Light L, float LightDistence, float4 LightStrenth)
{
    return (1.f / (L.Kc + L.Kl * LightDistence + L.Kq * pow(LightDistence, 2.f))) * LightStrenth;
}

float4 CaculateLightStrength(Light L, float3 PointNormal, float3 WorldLocation, float3 LightDirection)
{
    float4 LightStrenth = float4(L.LightIntensity, 1.f);
	if (L.LightType == 0)
	{
		// ƽ�й�
	}
    else if(L.LightType == 1)
    {
	    // ���

        float3 LightVector = L.LightPosition - WorldLocation;
        
        float LightDistence = length(LightVector);

        if (LightDistence < L.EndAttenuation)
        {
            // LightStrenth = AttenuationPointLight(L, LightDistence, LightStrenth);        // ����˥��
            LightStrenth = AttenuationPointLightCLQ(L, LightDistence, LightStrenth);        // ������˥��
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
		// ƽ�й�
        Direction = -L.LightDirection;

    }
    else if (L.LightType == 1)
    {
	    // ���
        Direction = L.LightPosition - WorldLocation;
       
    }

    return Direction;
}


