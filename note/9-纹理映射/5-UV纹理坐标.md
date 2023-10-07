# 球体模型展UV

![image-20230927141405796](.\image-20230927141405796.png)

球体模型展UV算法是一种将球体表面的纹理坐标映射到平面上的算法，用于在球体表面上正确呈现一个贴图。这个过程也被称为“球形映射”（spherical mapping）或“球形贴图”（spherical texturing），它可以通过以下步骤来实现：

1. 计算每个顶点的球面坐标

对于球体模型上的每个顶点，我们需要计算出其对应的球面坐标。一种常见的方法是使用球面坐标系（spherical coordinate system），其中一个点由三个参数组成：半径、极角和方位角。根据球体的半径和顶点在球面上的位置，我们可以计算出每个顶点的球面坐标。

2. 将球面坐标转换为UV坐标

   首先，我们知道，球坐标系和直角坐标系的转换关系如下

   1).球坐标系(r,θ,φ)与直角坐标系(x,y,z)的转换关系: 

   x=rsinθcosφ.

   y=rsinθsinφ.

   z=rcosθ.

   2).反之，直角坐标系(x,y,z)与球坐标系(r,θ,φ)的转换关系为:
   $$
   r = \sqrt{x^2 + y^2 + z^2}
   \\ \\
   \theta = \arccos{\frac{z}{r}}
   \\ \\
   \phi = \arctan{\frac{y}{x}}
   $$
   而θ是原点到点 P 的连线与正 z-轴之间的天顶角，它的范围0-π就能映射出一个球体，而对于φ，他是原点到点 P 的连线，在 xy-平面的[投影](https://baike.baidu.com/item/投影/7565?fromModule=lemma_inlink)线，与正 x-轴之间的方位角，它的范围是0-2π，就能映射出一个球体范围。

   所以球体的UV在我们知道θ和φ之后，可以这样表示：
   $$
   U = \phi / (2 * \pi)
   \\ \\
   V = \theta / \pi
   $$
   

接下来，我们需要将每个顶点的球面坐标映射到一个二维平面上，以便从贴图中获取相应的颜色值，使用公式：

```c++
u = (atan2(y, x) + PI) / (2 * PI)
v = acos(z / r) / PI
```

其中，(x,y,z)是该顶点的球面坐标，r是球的半径，PI表示圆周率。这个公式可以将球面坐标转换为一个范围为[0,1]的二维坐标，其中u和v分别代表水平和垂直方向的坐标。

3. 处理UV坐标的旋转和偏移

由于球体的表面通常不是完美的平滑曲面，而是由许多小多边形拼接而成的，因此在实际应用中，我们需要对UV坐标进行进一步处理，以保证贴图的正确呈现。一种常见的方法是在生成UV坐标时添加一定的旋转和偏移量，以便更好地匹配球体的表面。例如，在Unity游戏引擎中，使用球形映射时，可以通过调整材质属性来控制UV坐标的旋转和偏移量。



## InitAsShaderResourceView

`InitAsShaderResourceView` 是 DirectX 12 中的一个函数，用于将描述符堆中的描述符初始化为着色器资源视图。具体而言，它将传入的参数封装成一个 `D3D12_SHADER_RESOURCE_VIEW_DESC` 结构体，并使用这个结构体初始化指定索引处的描述符。

以下是 `InitAsShaderResourceView` 函数的声明：

```cpp
void CD3DX12_CPU_DESCRIPTOR_HANDLE::InitAsShaderResourceView(
  const D3D12_SHADER_RESOURCE_VIEW_DESC &SRVDesc
);
```

该函数的参数 `SRVDesc` 是一个 `D3D12_SHADER_RESOURCE_VIEW_DESC` 结构体，它描述了着色器资源视图的属性，例如格式、纹理类型、采样方式等。在调用该函数之前，需要先创建一个描述符堆，并获取其 CPU 句柄，然后再调用 `InitAsShaderResourceView` 初始化相应的描述符。

以下是使用 `InitAsShaderResourceView` 函数初始化描述符堆中的着色器资源视图的代码示例：

```cpp
// 创建描述符堆和相应的资源
ComPtr<ID3D12DescriptorHeap> srvHeap;
D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
srvHeapDesc.NumDescriptors = 1;
srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
ThrowIfFailed(md3dDevice->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap)));

ComPtr<ID3D12Resource> texture;
// 省略创建资源的代码

// 获取 CPU 描述符句柄
CD3DX12_CPU_DESCRIPTOR_HANDLE srvHandle(srvHeap->GetCPUDescriptorHandleForHeapStart());

// 初始化 SRV 描述符
D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
srvDesc.Format = texture->GetDesc().Format;
srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
srvDesc.Texture2D.MostDetailedMip = 0;
srvDesc.Texture2D.MipLevels = -1;
srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
srvHandle.InitAsShaderResourceView(srvDesc);
```

在上述代码中，首先创建了一个包含一个描述符的 CBV/SRV/UAV 类型的描述符堆，然后获取了该描述符堆的 CPU 描述符句柄。接下来，根据纹理资源的属性设置了 SRV 描述符的各个字段，并调用 `InitAsShaderResourceView` 函数将 SRV 描述符初始化到描述符堆的第一个位置。



## SetGraphicsRootShaderResourceView

`SetGraphicsRootShaderResourceView` 是 DirectX 12 中的一个命令列表函数，用于将着色器资源视图绑定到指定的根签名槽。它用于在渲染管线的各个阶段中绑定着色器所需的纹理、缓冲等资源。

以下是 `SetGraphicsRootShaderResourceView` 函数的声明：

```cpp
void SetGraphicsRootShaderResourceView(
  UINT RootParameterIndex,
  D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor
);
```

该函数的参数包括：

- `RootParameterIndex`：要绑定的根签名槽的索引。
- `BaseDescriptor`：着色器资源视图的 GPU 描述符句柄。

在调用 `SetGraphicsRootShaderResourceView` 函数之前，需要确保已经创建了与根签名相对应的根签名描述符表，并将其设置为流水线状态对象的一部分。此外，还需要创建描述符堆并将着色器资源视图的 CPU 描述符句柄绑定到其中，以便可以从中获取 GPU 描述符句柄。

以下是使用 `SetGraphicsRootShaderResourceView` 函数将着色器资源视图绑定到根签名槽的代码示例：

```cpp
// 创建描述符堆和相应的资源
ComPtr<ID3D12DescriptorHeap> srvHeap;
D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
srvHeapDesc.NumDescriptors = 1;
srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
ThrowIfFailed(md3dDevice->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap)));

ComPtr<ID3D12Resource> texture;
// 省略创建资源的代码

// 获取 CPU/GPU 描述符句柄
CD3DX12_CPU_DESCRIPTOR_HANDLE srvHandle(srvHeap->GetCPUDescriptorHandleForHeapStart());
CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle(srvHeap->GetGPUDescriptorHandleForHeapStart());

// 初始化 SRV 描述符
D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
srvDesc.Format = texture->GetDesc().Format;
srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
srvDesc.Texture2D.MostDetailedMip = 0;
srvDesc.Texture2D.MipLevels = -1;
srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
md3dDevice->CreateShaderResourceView(texture.Get(), &srvDesc, srvHandle);

// 将 SRV 描述符绑定到根签名槽
mCommandList->SetGraphicsRootShaderResourceView(0, gpuHandle);
```

在上述代码中，首先创建了一个包含一个描述符的 CBV/SRV/UAV 类型的描述符堆，获取了该描述符堆的 CPU 和 GPU 描述符句柄。接下来，根据纹理资源的属性设置了 SRV 描述符的各个字段，并调用 `CreateShaderResourceView` 函数将 SRV 描述符初始化到描述符堆中。最后，调用 `SetGraphicsRootShaderResourceView` 函数将 SRV 描述符绑定到根签名槽。
