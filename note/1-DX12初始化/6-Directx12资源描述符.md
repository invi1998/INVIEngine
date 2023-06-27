# Directx12资源描述符

DirectX 12资源描述符（Resource Descriptor）是一种用于描述DirectX 12库中各种图形资源的数据结构，包括缓冲区、纹理、常量缓冲区等。通过使用资源描述符，开发者可以方便地创建、管理和使用各种类型的图形资源。

在DirectX 12中，资源描述符通常由D3D12_RESOURCE_DESC结构体表示，其主要成员如下：

1. Dimension：指定资源的维度，如D3D12_RESOURCE_DIMENSION_BUFFER（缓冲区）、D3D12_RESOURCE_DIMENSION_TEXTURE1D（一维纹理）等。

2. Alignment：指定资源的对齐方式，通常为0或D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT。

3. Width/Height/Depth：指定资源的宽度、高度和深度，根据不同的维度而有所不同。

4. Format：指定资源的格式，如DXGI_FORMAT_R8G8B8A8_UNORM（32位无符号归一化RGBA格式）等。

5. MipLevels：指定资源的mipmap级别数量，通常为1或0（不使用mipmap）。

6. SampleDesc.Count/Quality：指定资源的多重采样数量和质量级别，用于消除锯齿和提高渲染质量。

7. Layout：指定资源的布局方式，如D3D12_TEXTURE_LAYOUT_UNKNOWN（未知布局）、D3D12_TEXTURE_LAYOUT_ROW_MAJOR（行主序布局）等。

通过使用上述成员，开发者可以灵活地创建各种类型的图形资源，并按照需求进行调整和优化。同时，在创建图形资源时需要注意硬件和平台的限制和兼容性，以确保代码的正确性和稳定性。

总之，DirectX 12资源描述符是DirectX 12库中非常重要的数据结构之一，用于描述和管理各种类型的图形资源。开发者可以通过设置不同的成员实现不同的渲染效果和优化策略，从而提高应用程序的性能和质量。



## 创建资源描述符RTV,DSV

```c++
/// 资源描述符

	// RTV
	D3D12_DESCRIPTOR_HEAP_DESC RTVDescriptorHeapDesc; // 定义 描述描述符堆（Descriptor Heap）的属性和配置信息 RTV
	// 配置描述符属性
	RTVDescriptorHeapDesc.NumDescriptors = FEngineRenderConfig::GetRenderConfig()->SwapChainCount;						// 描述符数量，这里设置为2，是因为我们使用双缓冲（前台缓冲区，后台缓冲区，所以有两个渲染视图，所以需要两个RTV， 故这里配置2）
	RTVDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;	// 指定描述符类型-渲染目标视图
	RTVDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;	// 指定描述符默认用法
	RTVDescriptorHeapDesc.NodeMask = 0;								// 指定描述符堆的节点掩码，用于多个GPU节点之间的通信与同步 (0表示不设置，使用默认GPU节点）
	// 配置好属性就可以创建了
	ANALYSIS_RESULT(D3dDevice->CreateDescriptorHeap(&RTVDescriptorHeapDesc, IID_PPV_ARGS(RTVHeap.GetAddressOf())));		// 使用设备驱动创建RTV描述符堆


	// DSV
	D3D12_DESCRIPTOR_HEAP_DESC DSVDescriptorHeapDesc; // 描述描述符堆（Descriptor Heap）的属性和配置信息 DSV
	// 配置描述符属性
	DSVDescriptorHeapDesc.NumDescriptors = 1;						// 描述符数量，深度缓冲区只需要一个就够了，深度缓冲信息是可以复用的
	DSVDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;	// 指定描述符类型-深度模板视图
	DSVDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;	// 指定描述符默认用法
	DSVDescriptorHeapDesc.NodeMask = 0;								// 指定描述符堆的节点掩码，用于多个GPU节点之间的通信与同步 (0表示不设置，使用默认GPU节点）
	// 配置好属性就可以创建了
	ANALYSIS_RESULT(D3dDevice->CreateDescriptorHeap(&DSVDescriptorHeapDesc, IID_PPV_ARGS(DSVHeap.GetAddressOf())));		// 使用设备驱动创建DSV描述符堆
```





## RTV 渲染目标视图

RTV是DirectX 12中的概念，全称为Render Target View，翻译为“渲染目标视图”。RTV是一种用于描述渲染目标缓冲区（Render Target Buffer）的数据结构，它将渲染目标缓冲区与GPU资源绑定，用于实现渲染和输出操作。

在DirectX 12中，每个渲染目标缓冲区都需要分配一个对应的RTV。RTV通常由D3D12_RENDER_TARGET_VIEW_DESC结构体表示，其中包含了以下成员：

1. Format：指定渲染目标缓冲区的格式，如DXGI_FORMAT_R8G8B8A8_UNORM（32位无符号归一化RGBA格式）等。

2. ViewDimension：指定RTV的维度，通常为D3D12_RTV_DIMENSION_TEXTURE2D（二维纹理）或D3D12_RTV_DIMENSION_TEXTURE3D（三维纹理）等。

3. Texture2D/MipSlice：用于指定二维纹理的层数或三维纹理的mipmap级别。

4. Texture2DArray/FirstArraySlice/ArraySize/MipSlice：用于指定二维纹理数组的起始和结束层数、mipmap级别等信息。

通过使用上述成员，开发者可以创建各种类型的RTV，并将其与渲染目标缓冲区进行绑定，实现渲染和输出功能。同时，在创建RTV时需要注意硬件和平台的限制和兼容性，以确保代码的正确性和稳定性。

总之，RTV是DirectX 12中非常重要的概念之一，用于描述和绑定渲染目标缓冲区，实现渲染和输出功能。开发者可以通过设置不同的成员实现不同的绑定方式和优化策略，从而提高应用程序的性能和质量。



## DSV 深度模板视图

DSV是DirectX 12中的一个概念，全称为Depth Stencil View，翻译为“深度模板视图”。在DirectX 12中，DSV用于描述深度和模板缓冲区，并将其与GPU资源绑定，用于实现深度测试和模板测试等操作。

DSV通常由D3D12_DEPTH_STENCIL_VIEW_DESC结构体表示，其中包含以下成员：

1. Format：指定深度模板缓冲区的格式，如DXGI_FORMAT_D24_UNORM_S8_UINT（24位无符号归一化深度+8位无符号整型模板格式）等。

2. ViewDimension：指定DSV的维度，通常为D3D12_DSV_DIMENSION_TEXTURE2D（二维纹理）或D3D12_DSV_DIMENSION_TEXTURE2DMS（多重采样二维纹理）等。

3. Texture2D/MipSlice：用于指定二维纹理的层数或mipmap级别。

4. Texture2DArray/FirstArraySlice/ArraySize/MipSlice：用于指定二维纹理数组的起始和结束层数、mipmap级别等信息。

通过使用上述成员，开发者可以创建不同类型的DSV，并将其与深度模板缓冲区进行绑定，实现深度测试和模板测试等操作。同时，在创建DSV时也需要注意硬件和平台的限制和兼容性，以确保代码的正确性和稳定性。

总之，DSV是DirectX 12中非常重要的概念之一，用于描述和绑定深度和模板缓冲区，实现深度测试和模板测试等操作。开发者可以通过设置不同的成员实现不同的绑定方式和优化策略，从而提高应用程序的性能和质量。



## D3D12_DESCRIPTOR_HEAP_DESC

D3D12_DESCRIPTOR_HEAP_DESC是DirectX 12库中的一个结构体，用于描述描述符堆（Descriptor Heap）的属性和配置信息。在DirectX 12中，描述符堆用于存储各种类型的描述符，如CBV、SRV、UAV、RTV、DSV等，用于实现GPU资源的绑定和管理。

D3D12_DESCRIPTOR_HEAP_DESC包含以下成员：

1. Type：指定描述符堆的类型，通常为D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV（常量缓冲区视图CBV/着色器资源视图SRV/无序访问视图UAV）或D3D12_DESCRIPTOR_HEAP_TYPE_RTV（渲染目标视图）等。
2. NumDescriptors：指定描述符堆中的描述符数量，根据不同的类型而有所不同。
3. Flags：指定描述符堆的标志，通常为D3D12_DESCRIPTOR_HEAP_FLAG_NONE或D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE等。
4. NodeMask：指定描述符堆的节点掩码，用于多个GPU节点之间的通信与同步。

通过使用上述成员，开发者可以创建不同类型和大小的描述符堆，并将其用于存储和管理不同类型的描述符。同时，在创建描述符堆时也需要注意硬件和平台的限制和兼容性，以确保代码的正确性和稳定性。

总之，D3D12_DESCRIPTOR_HEAP_DESC是DirectX 12中非常重要的结构体之一，用于描述描述符堆的属性和配置信息。开发者可以通过设置不同的成员实现不同类型和大小的描述符堆，并将其用于管理和绑定GPU资源，从而提高应用程序的性能和质量。

CreateDescriptorHeap是DirectX 12库中的一个方法，用于创建描述符堆（Descriptor Heap）。描述符堆是DirectX 12中非常重要的概念之一，用于存储和管理各种类型的描述符，如CBV、SRV、UAV、RTV、DSV等。

### Type

D3D12_DESCRIPTOR_HEAP_DESC的Type成员是一个枚举值，用于指定描述符堆（Descriptor Heap）的类型。在DirectX 12中，常用的描述符堆类型包括以下几种：

1. D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV：用于存储常量缓冲区、着色器资源视图和无序访问视图等。

2. D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER：用于存储采样器（Sampler）对象。

3. D3D12_DESCRIPTOR_HEAP_TYPE_RTV：用于存储渲染目标视图（Render Target View）。

4. D3D12_DESCRIPTOR_HEAP_TYPE_DSV：用于存储深度模板视图（Depth Stencil View）。

开发者可以根据实际需求选择合适的描述符堆类型，并设置相应的属性和配置信息。同时，在创建描述符堆时也需要注意硬件和平台的限制和兼容性，以保证代码的正确性和稳定性。

总之，D3D12_DESCRIPTOR_HEAP_DESC的Type成员包含了几种常见的描述符堆类型，开发者可以根据需求使用相应的枚举值，创建不同类型和大小的描述符堆，用于存储和管理GPU资源。

### Flags

D3D12_DESCRIPTOR_HEAP_DESC的Flags成员是一个枚举值，用于指定描述符堆（Descriptor Heap）的标志。在DirectX 12中，常见的描述符堆标志包括以下几种：

1. D3D12_DESCRIPTOR_HEAP_FLAG_NONE：表示不使用任何标志。

2. D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE：表示描述符堆可以在着色器阶段可见，允许GPU直接读取和写入其中的描述符。

3. D3D12_DESCRIPTOR_HEAP_FLAG_ALLOW_DEPTH_STENCIL：表示描述符堆可以存储深度模板视图（DSV），仅适用于类型为D3D12_DESCRIPTOR_HEAP_TYPE_DSV的描述符堆。

4. D3D12_DESCRIPTOR_HEAP_FLAG_DENY_VERTEX_SHADER_ROOT_ACCESS：表示描述符堆不能在顶点着色器根签名中使用，禁止GPU直接访问其中的描述符。

5. D3D12_DESCRIPTOR_HEAP_FLAG_DENY_HULL_SHADER_ROOT_ACCESS：表示描述符堆不能在Tessellation Hull着色器根签名中使用，禁止GPU直接访问其中的描述符。

6. D3D12_DESCRIPTOR_HEAP_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS：表示描述符堆不能在Tessellation Domain着色器根签名中使用，禁止GPU直接访问其中的描述符。

7. D3D12_DESCRIPTOR_HEAP_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS：表示描述符堆不能在几何着色器根签名中使用，禁止GPU直接访问其中的描述符。

8. D3D12_DESCRIPTOR_HEAP_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS：表示描述符堆不能在像素着色器根签名中使用，禁止GPU直接访问其中的描述符。

开发者可以根据实际需求选择相应的标志，并将其设置到D3D12_DESCRIPTOR_HEAP_DESC结构体中，以创建具有不同特性和功能的描述符堆。同时，在使用描述符堆时也需要注意硬件和平台的限制和兼容性，以确保代码的正确性和稳定性。

总之，D3D12_DESCRIPTOR_HEAP_DESC的Flags成员包含了几种常见的描述符堆标志，开发者可以根据需求使用相应的枚举值，创建具有不同特性和功能的描述符堆，用于存储和管理GPU资源。

## CreateDescriptorHeap 创建描述符堆

CreateDescriptorHeap方法通常由ID3D12Device接口调用，其函数原型如下：

```c++
HRESULT CreateDescriptorHeap(
  const D3D12_DESCRIPTOR_HEAP_DESC *pDescriptorHeapDesc,
  REFIID                            riid,
  void                              **ppvHeap
);
```

该方法接受三个参数：

1. pDescriptorHeapDesc：指向D3D12_DESCRIPTOR_HEAP_DESC结构体的指针，用于描述描述符堆的属性和配置信息。

2. riid：标识所创建的描述符堆的接口类型ID，如IID_PPV_ARGS。

3. ppvHeap：输出参数，返回指向描述符堆对象的指针。

通过调用CreateDescriptorHeap方法，开发者可以创建不同类型和大小的描述符堆，并将其用于存储和管理GPU资源。在创建描述符堆时需要注意硬件和平台的限制和兼容性，以确保代码的正确性和稳定性。

总之，CreateDescriptorHeap方法是DirectX 12库中非常重要的方法之一，用于创建描述符堆并返回指向描述符堆对象的指针。开发者应当根据实际需求选择合适的描述符堆类型和大小，并注意硬件和平台的限制和兼容性，以实现高效、稳定和易于维护的应用程序。

**为什么DX12编程里，某些情况获取地址不用取地址符&，而是用GetAddressOf方法**

在DX12编程中，一些获取地址的操作不需要使用取地址符&，而是使用GetAddressOf()方法，这是由于DirectX 12库的设计特点决定的。

在DirectX 12库中，许多对象和接口都是以指针的形式返回的。为了方便开发者使用这些对象和接口，DirectX 12库提供了一些简化的方法和函数，比如GetAddressOf()方法，用于获取指针的地址。

使用GetAddressOf()方法可以避免出现繁琐的取地址符&操作，并且更加直观和易于理解。例如，在创建描述符堆时，可以使用以下代码获取ID3D12DescriptorHeap接口的指针地址：

```c++
Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_descriptorHeap;
HRESULT hr = m_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(m_descriptorHeap.GetAddressOf()));
```

在上述代码中，使用了m_descriptorHeap.GetAddressOf()方法获取ID3D12DescriptorHeap接口的指针地址，使得代码更加简洁和易于理解。

总之，使用GetAddressOf()方法可以简化代码，同时也能提高可读性和可维护性。开发者在DX12编程中应当灵活使用各种方法和函数，以实现高效、稳定和易于维护的应用程序。