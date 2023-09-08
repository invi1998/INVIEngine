// 定义材质
struct FMaterial
{
    // 通用材质
    float4 BaseColor;
};


// 菲尼尔
float3 FresnelSchlick(float3 F0, float3 PointNormal, float3 Direction, int M)
{
    return F0 + (1.f-F0) * pow(1.f - saturate(dot(PointNormal, Direction)), M);
}
