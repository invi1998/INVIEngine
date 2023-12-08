#pragma once

struct FLight
{
    FLight();

    XMFLOAT3 LightIntensity;  // 光照强度
    float StartAttenuation; // 开始衰减

    XMFLOAT3 LightDirection;  // 光照方向
    float EndAttenuation;   // 结束衰减

    XMFLOAT3 LightPosition;    // 灯光位置
    int LightType;          // 灯光类型

    float Kc;               // 非线性衰减常数项
    float Kl;               // 非线性衰减一次项
    float Kq;               // 非线性衰减二次项

    float SpotInnerCornerPhi;          // 指定了聚光半径的切光角。落在这个角度之外的物体都不会被这个聚光所照亮。
    float SpotOuterCornerTheta;        // LightDir向量和SpotDir向量之间的夹角。在聚光内部的话θ值应该比ϕ值小

    float xxs1;
    float xxs2;
    float xxs3;

	XMFLOAT4X4 ShadowTransform;	// 灯光视角阴影变换矩阵
};

// 灯光常量缓冲区
struct FLightConstantBuffer
{
	FLightConstantBuffer();

	FLight SceneLights[16];
};

