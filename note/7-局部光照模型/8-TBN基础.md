# TBN基础

![img](.\format,f_auto)

首先我们回顾一下球坐标系转直角坐标系，有如下公式
$$
x = r \sin{\theta} \cdot \cos{\phi}
\\ 
y = r \sin{\theta} \cdot \sin{\phi}
\\
z = r \cdot \cos{\theta}
$$

- r 是球半径
- θ 是向量和z轴正方向夹角
- φ 是向量在xy平面上的投影和x轴的夹角



3D 图形应用程序使用两种类型的笛卡尔坐标系：左手坐标系和右手坐标系。 在这两个坐标系统中，正 x 轴指向右侧，正 y 轴指向上方。 通过将你的左手或右手手指指向正 x 方向，然后弯曲手指，将其指向正 y 方向，你可以记住正 z 轴指向的方向。 你的拇指指向的方向（无论指向你还是相反方向）就是正 z 轴为坐标系统所指的方向。 下面的图例显示了这两个坐标系统。



![左手和右手笛卡尔坐标系统图例](.\leftrght.png)

**Direct3D 使用左手坐标系统。** 如果要移植基于右手坐标系的应用程序，则必须对传递到 Direct3D 的数据进行两次更改。



如上公式是基本数学中的求坐标公式，在DX12中，因为坐标系朝向的差异，Dx里的y和z是互换的，也就是在模型顶点中（这里拿球体的北极顶点做示例），Y是法线

![image-20230910231739411](.\image-20230910231739411.png)

即，在DX中，球坐标系应该是如下
$$
x = r \sin{\theta} \cdot \cos{\phi}
\\
y = r \cdot \cos{\theta}
\\
z = r \sin{\theta} \cdot \sin{\phi}
$$


在如上的一个顶点中，在切线空间中，y轴数值是为0的，而我们的x 就是我们顶点坐标的z坐标，然后我们的-z，就是等于我们顶点当前的x。
$$
切线空间的顶点切线计算,x,z是顶点坐标
\\
UTangent = (ut_x, ut_y, ut_z)
\\
ut_x = -z
\\
ut_y = 0
\\
ut_z = x
$$


```c++
			int TopIndex = MeshData.VertexData.size() - 1;
			FVertex& InVertex = MeshData.VertexData[TopIndex];
			// 求球面上该点的法线
			XMVECTOR Pos = XMLoadFloat3(&InVertex.Position);
			XMStoreFloat3(&InVertex.Normal, XMVector3Normalize(Pos));

			// U方向的切线
			InVertex.UTangent.x = -InRadius * sinf(BetaValue) * sinf(ThetaValue);
			InVertex.UTangent.y = 0.f;
			InVertex.UTangent.z = InRadius * sinf(BetaValue) * cosf(ThetaValue);

			// 计算出切线后，对切线值进行归一化存储
			XMVECTOR Tangent = XMLoadFloat3(&InVertex.UTangent);
			XMStoreFloat3(&InVertex.UTangent, XMVector3Normalize(Tangent));
```



