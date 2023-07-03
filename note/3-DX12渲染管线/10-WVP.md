# wvp

在计算图形渲染时，WVP和MVP是两个重要的矩阵变换。它们都是将3D模型从模型空间转换为裁剪空间的变换矩阵，但在实际应用中有所不同。

WVP代表了世界-视图-投影矩阵（World-View-Projection Matrix），而MVP代表了模型-视图-投影矩阵（Model-View-Projection Matrix）。下面分别详细解释一下：

1. WVP（世界-视图-投影矩阵）

WVP矩阵是将一个3D模型从世界空间（World Space）变换到裁剪空间（Clip Space）的过程中使用的矩阵。它由以下三个部分组成：

- 世界矩阵（World Matrix）：将模型从模型空间（Model Space）变换到世界空间（World Space）。
- 视图矩阵（View Matrix）：将模型从世界空间变换到相机空间（Camera Space）或视图空间（View Space）。
- 投影矩阵（Projection Matrix）：将模型从相机空间变换到裁剪空间。

因此，WVP矩阵可以表示为：

```
WVP = WorldMatrix * ViewMatrix * ProjectionMatrix
```

其中，* 表示矩阵乘法运算。

2. MVP（模型-视图-投影矩阵）

MVP矩阵是将一个3D模型从模型空间变换到裁剪空间的过程中使用的矩阵。它由以下三个部分组成：

- 模型矩阵（Model Matrix）：将模型从模型空间变换到世界空间。
- 视图矩阵（View Matrix）：将模型从世界空间变换到相机空间或视图空间。
- 投影矩阵（Projection Matrix）：将模型从相机空间变换到裁剪空间。

因此，MVP矩阵可以表示为：

```
MVP = ModelMatrix * ViewMatrix * ProjectionMatrix
```

其中，* 表示矩阵乘法运算。

区别：

WVP矩阵与MVP矩阵的区别在于，WVP矩阵中的世界矩阵与MVP矩阵中的模型矩阵具有不同的含义。世界矩阵负责将模型从模型空间变换到世界空间，而模型矩阵则只负责将模型从模型空间变换到世界空间。因此，在WVP矩阵中，需要先将模型从模型空间变换到世界空间，然后再进行后续的视图和投影变换；而在MVP矩阵中，则直接将模型从模型空间变换到裁剪空间。

另外，WVP矩阵常用于实现多物体渲染，即同时渲染多个3D模型。由于每个模型可能位于不同的位置并朝向不同的方向，因此需要使用不同的世界矩阵来描述它们。而MVP矩阵则常用于单物体渲染，即只渲染一个3D模型。在这种情况下，通常只需要一个模型矩阵来描述模型的位置、旋转和缩放等属性。



## XMMatrixPerspectiveFovLH 基于视野构建的的左手透视投影矩阵

`XMMatrixPerspectiveFovLH`是DirectX Math库中的一个函数，用于创建左手坐标系下的透视投影矩阵。它根据给定的垂直视角、宽高比、近裁剪面和远裁剪面参数创建一个透视投影矩阵。

该函数的原型如下：

```cpp
XMMATRIX XMMatrixPerspectiveFovLH(float FovAngleY, float AspectRatio, float NearZ, float FarZ);
```

其中，参数含义如下：

- `FovAngleY`：视野的垂直角度，单位是弧度。
- `AspectRatio`：宽高比，即视口宽度除以视口高度。
- `NearZ`：近裁剪面距离。
- `FarZ`：远裁剪面距离。

该函数返回类型为`XMMATRIX`，是一个4x4的浮点矩阵。

以下是使用`XMMatrixPerspectiveFovLH`函数创建透视投影矩阵的示例代码：

```cpp
// 创建透视投影矩阵
XMMATRIX projMatrix = XMMatrixPerspectiveFovLH(XM_PIDIV4, aspectRatio, 1.0f, 1000.0f);

// 将矩阵上传至常量缓冲区或着色器
...
```

在上述示例中，我们创建了一个垂直视角为45度、宽高比为aspectRatio、近裁剪面距离为1.0f、远裁剪面距离为1000.0f的透视投影矩阵，并将其上传至常量缓冲区或着色器供后续使用。

需要注意的是，由于`XMMatrixPerspectiveFovLH`函数创建的是左手坐标系下的透视投影矩阵，因此在使用时需要保证渲染管线也处于左手坐标系。如果您的应用程序使用的是右手坐标系，则应使用`XMMatrixPerspectiveFovRH`函数来创建右手坐标系下的透视投影矩阵。