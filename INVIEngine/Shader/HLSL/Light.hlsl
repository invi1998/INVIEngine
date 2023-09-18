
struct Light
{
    float3 LightIntensity;  // 光照强度
    float StartAttenuation; // 开始衰减
    float3 LightDirection;  // 光照方向
    float EndAttenuation;   // 结束衰减
    float3 LightPosition;    // 灯光位置
    int LightType;          // 灯光类型
};
