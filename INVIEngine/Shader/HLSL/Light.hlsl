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
    float SpotInnerCornerPhi; // ָ���˾۹�뾶���й�ǡ���������Ƕ�֮������嶼���ᱻ����۹���������

    float SpotOuterCornerTheta; // LightDir������SpotDir����֮��ļнǡ��ھ۹��ڲ��Ļ�thetaֵӦ�ñ�phiֵС
    float xxs1;
    float xxs2;
    float xxs3;
	
};

float3 GetLightDirection(Light L, float3 WorldLocation)
{
    if (L.LightType == 0)
    {
		// ƽ�й�
        return L.LightDirection;
    }
    else if (L.LightType == 1)
    {
	    // ���
        return L.LightPosition - WorldLocation;
    }
    else if (L.LightType == 2)
    {
	    // �۹�
        return L.LightPosition - WorldLocation;
    }
    return L.LightDirection;
}


// ����˥�����㷽ʽ1 (����˥����
float4 AttenuationPointLight(Light L, float LightDistence, float4 LightStrenth)
{
    float AttenuationRange = L.EndAttenuation - L.StartAttenuation;
    LightStrenth = LightStrenth * (1.f - LightDistence / AttenuationRange);
    return LightStrenth;
}

// ����˥�����㷽ʽ2 (������ƽ��˥����
// ������Kc��һ����Kl��������Kq
float4 AttenuationPointLightCLQ(Light L, float LightDistence, float4 LightStrenth)
{
    return (1.f / (L.Kc + L.Kl * LightDistence + L.Kq * pow(LightDistence, 2.f))) * LightStrenth;
}

float4 CaculateLightStrength(Light L, float3 PointNormal, float3 WorldLocation, float3 NormalizeLightDirection)
{
    float4 LightStrenth = float4(1.f, 1.f, 1.f, 1.f) * float4(L.LightIntensity, 1.f);
	if (L.LightType == 0)
	{
		// ƽ�й�
        return LightStrenth;
    }
    else if(L.LightType == 1)
    {
	    // ���

        float3 LightVector = L.LightPosition - WorldLocation;
        
        float LightDistence = length(LightVector);

        if (LightDistence < L.EndAttenuation)
        {
            return AttenuationPointLight(L, LightDistence, LightStrenth);        // ����˥��
            // return AttenuationPointLightCLQ(L, LightDistence, LightStrenth); // ������˥��
        }
    	return float4(0.f, 0.f, 0.f, 1.f);
    }
    else if (L.LightType == 2)
    {
	    // �۹��
        float3 LightVector = L.LightPosition - WorldLocation;
        float LightDistence = length(LightVector);

        if (LightDistence < L.EndAttenuation)
        {
            float DotValue = max(dot(NormalizeLightDirection, L.LightDirection), 0.f);

            float Theta = acos(DotValue);   // ����Ƕ�
            if (Theta <= L.SpotInnerCornerPhi)
            {
                return LightStrenth;
            }
            if (Theta <= L.SpotOuterCornerTheta && Theta > L.SpotInnerCornerPhi)
            {
                float OuterInnerDistance = L.SpotOuterCornerTheta - L.SpotInnerCornerPhi;
                float CurrentDistance = OuterInnerDistance - (Theta - L.SpotInnerCornerPhi);

                return (CurrentDistance / OuterInnerDistance) * LightStrenth;
				// LightStrenth = AttenuationPointLightCLQ(L, CurrentDistance, LightStrenth); // ������˥��
            }
        }
    }
    return float4(0.f, 0.f, 0.f, 1.f);
}
