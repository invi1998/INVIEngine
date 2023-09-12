// pbr ��Ⱦ����

// D �� ���߷ֲ�����
float4 GetDistributionGGX(float3 N, float H, float Roughness)
{
    float PI = 3.1415926f;
    
    float a2 = pow(Roughness, 4);
    
    float NoH = saturate(dot(N, H));
    
    float d = (NoH * NoH * (a2 - 1) + 1);
    
    return a2 / max((d * d * PI), 0.0001f);
    
}