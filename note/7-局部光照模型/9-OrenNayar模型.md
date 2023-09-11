# OrenNayar模型

Oren-Nayar模型是一种用于表达粗糙表面反射的经典微平面模型，它可以描述细微表面结构对光线的散射现象。该模型考虑了光线在投射到不同方向时受到表面微观凹凸影响的半球分布情况，从而计算出了不同角度下的反射率。

该模型的反射率计算公式如下：

$$
f_{ON}(\theta_i,\phi_i,\theta_r,\phi_r) = \frac{A}{\pi}(\cos\alpha + (B\sin\alpha)\max(0,\cos\phi_i - \cos\phi_r)\sin\theta_i\sin\theta_r)
$$

其中，$\theta_i$和$\phi_i$表示入射光线的极角和方位角，$\theta_r$和$\phi_r$表示反射光线的极角和方位角，$\alpha$表示两个法向量之间的夹角，$A$和$B$为材质常数，可通过以下公式计算得出：

$$
A = 1-\frac{0.5*\sigma^2}{0.5*\sigma^2+0.33}
$$

$$
B = 0.45*\sigma^2/(0.45*\sigma^2+0.09)
$$

其中，$\sigma$表示表面粗糙度，取值范围为0到$+\infty$之间。

Oren-Nayar模型的实现比较简单，只需要根据上述公式计算反射率即可。但需要注意的是，在实际应用中，通常还需要考虑其他因素，如阴影、环境光等。因此，需要结合其他光照模型来进行综合计算。

同时，需要注意的是，该模型并不能完全准确地描述真实世界中的光学行为，它只是一种简化的模型，能够提供良好的视觉效果。在实际应用中，可能需要使用更为精细的光学模型来进行计算。



![image-20230911145837257](.\image-20230911145837257.png)

- $\sigma$ 表示粗糙度
- A 和 B 表示基于粗糙度的一个常量系数，当粗糙度为0的时候，其实最后得到的公式就是我们的兰伯特模型



余弦函数的数学图像如下

![image-20230911172045231](.\image-20230911172045231.png)

反余弦函数的数学图像如下, 其定义域为[-1, 1]，值域为 [π, 0]

![image-20230911172126252](C:\Users\admin\AppData\Roaming\Typora\typora-user-images\image-20230911172126252.png)



shader

```c++
	    // OrenNayar GDC粗糙表面
        
        float3 ViewDirection = normalize(CameraPosition.xyz - mvOut.WorldPosition.xyz);
        float3 NormalLight = saturate(dot(ModelNormal, NormalizeLightDirection));
        float3 NormalView = saturate(dot(ModelNormal, ViewDirection));
        
        float PhiR = 
            length(ViewDirection - ModelNormal * NormalView) *      // 视角到法线的距离
            length(NormalizeLightDirection - ModelNormal * NormalLight);    // 灯光到法线的距离
        
        // 这里可以看到，我们的法线乘以了一个 NormalView 值，ModelNormal * NormalView 的结果就是实现了对法线的一个缩放
        
        float ACosNormalView = acos(NormalView); // [0, 1]
        float ACosNormalLight = acos(NormalLight);
        
        float Alpha = max(ACosNormalView, ACosNormalLight);
        float Beta = min(ACosNormalView, ACosNormalLight);
        
        float Roughness = pow(MaterialRoughness, 2);        // 粗糙度
        
        float A = 1 - 0.5f * (Roughness / (Roughness + 0.33f));
        float B = 0.45f * (Roughness / (Roughness + 0.09f));
        
        
        DotDiffValue = NormalLight * (A + B * max(0, PhiR) * sin(Alpha) * tan(Beta));
```

