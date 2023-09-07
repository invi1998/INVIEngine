// �������
struct FMaterial
{
    // ͨ�ò���
    float4 BaseColor;
};


// �����
float3 FresnelSchlick(float3 F0, float3 PointNormal, float3 Direction)
{
    return F0 + (1.f - saturate(dot(PointNormal, Direction)));
}
