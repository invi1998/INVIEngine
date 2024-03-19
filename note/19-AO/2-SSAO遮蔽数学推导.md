# SSAO遮蔽

![image-20240319153757818](.\image-20240319153757818.png)

我们在shader中现在可以拿到N法线信息，D深度信息，我们的深度信息是需要通过深度图采样提取出来，此时得到的深度信息是位于NDC空间的值（范围为[-1, 1]），我们现在需要做的就是首先得把这个深度值转到视口空间【0-1】。

![image-20240319154357540](.\image-20240319154357540.png)

如图，a点是我们场景里需要着色的一个点，他的法线为n，b是我们以a为中心的采样体积内的一个随机采样点，然后我们将这点和我们的视口进行连线，得到与前方的一个遮挡物的交点c，所以这里c是我们的一个未知数。然后下面这个水平的红色值是我们基于视口的深度信息。c点的深度表示为c2，b为b2， a为a2。

然后我们通过射线可以知道
$$
b = ct\\
t = \frac{b}{c}\\
同理对于我们的深度信息，也是一样的\\
b_2 = c_2 t\\
t = \frac{b_2}{c_2}\\
所以\\
\frac{b}{c} = \frac{b_2}{c_2}\\
c = \frac{c_2}{b_2}b\\
$$
c2，b, b2这些信息我们都是知道的，所以这样就可以求出点c的位置。

然后，接下来我们求a点。

![image-20240319155825103](.\image-20240319155825103.png)

如图，在我们的shader中，此时我们有一个 ViewPosition 这个点是我们近景层面的点，也就是上图中的v，

```glsl
MeshVertexOut VSMain(uint VertexID : SV_VertexID)
{
	
	MeshVertexOut outV;
	
	outV.Texcoord = TextureCoordinates[ VertexID];
	
	// 我们需要将 0-1 映射到NDC空间，所以我们需要将UV坐标乘以2，然后减去1，然后因为Y轴是反的，所以我们需要将Y轴的UV坐标乘以-1
	outV.Position = float4(outV.Texcoord.x * 2.f - 1.f, (outV.Texcoord.y * 2.f - 1.f) * -1.f, 0.f, 1.f);
	
	// 将点转换到视口空间下
	float4 PositionH = mul(outV.Position, InversiveProjectionMatrix);
	
	// 转到近景层面
	outV.ViewPosition.xyz = PositionH.xyz / PositionH.w;

	return outV;
}
```

然后，我们假设经过t时间，我们的射线从v达到了a，那么a就可以表示为
$$
a = tv\\
带入深度信息可以得到\\
a_2 = v_2 t\\
因为v是我们的近景层面，所以他的深度信息就是我们Near值，这个值我们在创建视口的时候就已经确定的\\
所以v_2我们是知道的，带入就可以得到a的值为\\
a = \frac{a_2}{v_2}v
$$
所以，我们只要有深度信息，就可以很容易得到我们想要的点信息。但是我们之前也说了，我们通过深度贴图采样得到的深度值是位于NDC空间的，我们需要将其转为视口空间下，才能使用。

![image-20240319160929348](.\image-20240319160929348.png)

![image-20240319161008220](.\image-20240319161008220.png)

上述矩阵就是我们将点从NDC空间转到视口空间的矩阵，注意，我们GPUshader在使用的时候，需要将该矩阵进行转置使用

我们的深度信息z，
$$
Z_{NDC} = A + \frac{B}{Z}
$$
我们这里要求的就是这个z，A,B,以及$z_{ndc}$值是已知的，就是上面我们矩阵里的$A = m_{33}, B = m_{34}$,
$$
Z = \frac{B}{Z_{NDC}-A}\\
$$
