# Shader

## HLSL

HLSL（High-Level Shader Language）是 Microsoft 开发的一种高级着色器语言，用于编写 GPU 着色器代码。HLSL 通常用于 Direct3D 应用程序开发中，可以用来描述顶点、像素、几何、计算等不同类型的着色器。与其他着色器语言相比，HLSL 具有以下优势：

- 独立于硬件：HLSL 可以在不同的图形卡上运行，而无需进行大量的调整和优化。
- 易于使用：HLSL 采用类 C 的语法，易于编写和理解。
- 功能丰富：HLSL 提供了许多内置函数和数据类型，可以执行矩阵变换、纹理采样、光照计算等常见任务。
- 调试支持：HLSL 支持利用 Visual Studio 进行调试，方便用户快速定位问题。

HLSL 程序通常由两个部分组成：输入和输出。输入部分定义了输入结构体，其中包含着色器需要的输入参数，如顶点坐标、颜色、纹理坐标等。输出部分则定义了输出结构体，其中包含着色器需要输出的结果，如像素颜色、深度值等。

例如，下面是一个简单的 HLSL 顶点着色器示例：

```c++
struct VertexInput
{
    float3 position : POSITION;
    float4 color : COLOR;
    float2 texcoord : TEXCOORD;
};

struct VertexOutput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 texcoord : TEXCOORD;
};

VertexOutput main(VertexInput input)
{
    VertexOutput output;
    output.position = mul(float4(input.position, 1.0f), WorldViewProj);
    output.color = input.color;
    output.texcoord = input.texcoord;
    return output;
}
```

在上面的代码中，首先定义了一个输入结构体 VertexInput，其中包含了顶点坐标、颜色和纹理坐标等信息。然后，定义了一个输出结构体 VertexOutput，其中包含了转换后的顶点位置、颜色和纹理坐标等信息。最后，定义了一个 main 函数作为入口点，通过将输入结构体转换为输出结构体，实现了从顶点到像素的转换过程。

总之，HLSL 是一种用于编写 GPU 着色器代码的高级着色器语言，具有独立于硬件、易于使用、功能丰富和调试支持等优势。在 Direct3D 应用程序开发中，HLSL 能够帮助开发者快速实现各种类型的着色器，并获得更好的性能和效果。

## SV_POSITION 和 POSITION

SV_ _POSITION: SV_ 前缀的变量代表system value,在DX10以后的语义绑定中被使用代表特殊的意义，和POSITION用法并无不同。唯一区别是SV_ POSTION-旦被作为vertex shader的输出语义，那么这个最终的顶点位置就被固定了(不能tensellate,不能再被后续改变它的空间位置? ), 已经成为了转换裁剪世界的坐标，可以直接用来进入光栅化处理的坐标，如果作为fragment shader的输入语义那么和POSITION是一样的，代表着每个像素点在屏幕上的位置(这 个说法其实并不准确，事实是fragment 在view space空间中的位置,但直观的感受是如括号之前所述一般)

其次：在DX10版本之前没有引入SV_的预定义语义，POSITION被用作vertex shader的输入，输出, fragment shader的输入参数。但DX10之后就推荐使用SV_ POSITION作为vertex shader的输出和fragment shader的输入了,注意vertex shader的输入还是使用POSITION!切记。但是DX10以后的代码依旧兼容POSITION作为全程表达

总结

两个的用法大致相同,都是存储坐标信息,但是一个是转换前的， -个是转换后的。
POSITION:用来存储，模型在本地坐标下，模型空间中(objcet space)的顶点坐标,转换为剪裁空间坐标前的坐标，unity告诉我们的模型顶点坐标，没经过转换的。可作定色器(vertex shader)的输入、输出;片段着色器(frag) 的输入。

SV_ POSITION:来存储，模型在剪裁空间，投影空间中的位置信息,即把模型空间的定点坐标，转化为剪裁空间的坐标，可作顶点着色器(vertex shader)的输出;片段着色器(frag) 的输入。.

## D3D12_INPUT_ELEMENT_DESC

D3D12_INPUT_ELEMENT_DESC 是 Direct3D 12 应用程序开发中的一个结构体，用于描述输入布局（Input Layout）中的单个元素。在 Direct3D 12 中，输入布局是一个包含多个输入元素的数据结构，表示顶点缓冲区中存储的顶点数据的格式和排列方式。

D3D12_INPUT_ELEMENT_DESC 结构体的定义如下：

```c++
typedef struct D3D12_INPUT_ELEMENT_DESC {
    LPCSTR SemanticName;
    UINT SemanticIndex;
    DXGI_FORMAT Format;
    UINT InputSlot;
    UINT AlignedByteOffset;
    D3D12_INPUT_CLASSIFICATION InputSlotClass;
    UINT InstanceDataStepRate;
} D3D12_INPUT_ELEMENT_DESC;
```

各成员变量的含义如下：

- SemanticName：表示输入元素语义名称的字符串指针，通常与着色器中的输入语义名称相对应。
- SemanticIndex：表示输入元素语义索引，通常用于区分具有相同语义名称的不同元素。
- Format：表示输入元素数据格式，例如浮点数、整数、向量等。
- InputSlot：表示输入元素所在的顶点缓冲区槽位（Vertex Buffer Slot）。
- AlignedByteOffset：表示输入元素在顶点缓冲区中的偏移量，以字节为单位。
- InputSlotClass：表示输入元素数据类型的类别，可以是 Per Vertex Data 或者 Per Instance Data。
- InstanceDataStepRate：表示每次实例化时，该输入元素的步幅大小，以字节为单位。

例如，下面的代码展示了如何创建一个包含两个输入元素的输入布局：

```c++
D3D12_INPUT_ELEMENT_DESC inputElementDescs[2] = {};
inputElementDescs[0].SemanticName = "POSITION";
inputElementDescs[0].SemanticIndex = 0;
inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
inputElementDescs[0].InputSlot = 0;
inputElementDescs[0].AlignedByteOffset = 0;
inputElementDescs[0].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
inputElementDescs[0].InstanceDataStepRate = 0;

inputElementDescs[1].SemanticName = "TEXCOORD";
inputElementDescs[1].SemanticIndex = 0;
inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
inputElementDescs[1].InputSlot = 0;
inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
inputElementDescs[1].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
inputElementDescs[1].InstanceDataStepRate = 0;
```

在上面的代码中，首先定义了一个包含两个输入元素的数组 inputElementDescs，其中第一个元素代表顶点坐标，第二个元素代表纹理坐标。然后，对每个元素依次进行初始化，并指定它们在顶点缓冲区中的位置和排列方式。注意，第二个元素的 AlignedByteOffset 属性使用了 D3D12_APPEND_ALIGNED_ELEMENT 宏，表示它的偏移量应该在前一个元素之后自动对齐。

总之，D3D12_INPUT_ELEMENT_DESC 结构体是一个用于描述输入布局中单个元素的数据结构，通过它可以指定输入元素的语义名称、索引、数据格式和排列方式等信息。在 Direct3D 12 应用程序开发中，需要根据具体的需求创建合适的输入布局对象，从而让 GPU 正确地读取和处理顶点数据。



## D3DCompileFromFile

D3DCompileFromFile 是 Direct3D 应用程序开发中的一个函数，用于编译 HLSL（High-Level Shader Language）代码并生成字节码数据。在 Direct3D 中，着色器代码通常需要经过编译才能在 GPU 上运行，而 D3DCompileFromFile 函数就是用于实现这一功能的 API。

D3DCompileFromFile 函数的原型如下：

```c++
HRESULT D3DCompileFromFile(
  LPCWSTR pFileName,
  const D3D_SHADER_MACRO* pDefines,
  ID3DInclude* pInclude,
  LPCSTR pEntrypoint,
  LPCSTR pTarget,
  UINT Flags1,
  UINT Flags2,
  ID3DBlob** ppCode,
  ID3DBlob** ppErrorMsgs
);
```

各参数的含义如下：

- pFileName：表示包含 HLSL 代码的文件名。
- pDefines：表示宏定义数组的指针，用于定义 HLSL 代码中使用的预处理宏。
- pInclude：表示自定义的 include 文件接口，用于加载 HLSL 代码中引用的外部文件。
- pEntrypoint：表示 HLSL 代码中的入口点函数名称，例如顶点着色器中的 main 函数。
- pTarget：表示目标着色器模型的名称和版本号，例如 "vs_5_0" 表示使用 Direct3D 11 的顶点着色器模型。
- Flags1：表示编译选项的掩码，可以使用常量 D3DCOMPILE_* 中的多个值进行按位或运算。
- Flags2：暂未使用，必须设置为 0。
- ppCode：表示返回的指向包含编译后代码数据的 ID3DBlob 接口指针的指针。
- ppErrorMsgs：表示返回的指向包含编译错误信息的 ID3DBlob 接口指针的指针。

例如，下面的代码展示了如何使用 D3DCompileFromFile 函数来编译一个简单的 HLSL 顶点着色器：

```c++
ID3DBlob* pShaderBlob = nullptr;
HRESULT hr = D3DCompileFromFile(
    L"MyShader.hlsl",   // 文件名
    nullptr,            // 宏定义数组
    nullptr,            // 自定义 include 文件接口
    "VSMain",           // 入口点函数名称
    "vs_5_0",           // 目标着色器模型
    D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // 编译选项
    0,                  // 保留参数，设为 0
    &pShaderBlob,       // 返回编译后代码数据
    nullptr             // 返回编译错误信息
);
if (FAILED(hr))
{
    // 编译失败，处理错误信息
    // ...
}
else
{
    // 编译成功，使用编译后的代码数据创建着色器对象
    hr = device->CreateVertexShader(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), nullptr, &m_VertexShader);
    if (FAILED(hr))
    {
        // 着色器创建失败，处理错误信息
        // ...
    }
}
```

在上面的代码中，首先调用 D3DCompileFromFile 函数对 MyShader.hlsl 文件中的 VSMain 函数进行编译，并将编译后的代码数据保存在 pShaderBlob 变量中。如果编译失败，则需要处理编译错误信息；否则，通过使用 CreateVertexShader 函数创建顶点着色器对象，并将编译后的代码数据作为初始化参数传入。

总之，D3DCompileFromFile 函数是一个用于编译 HLSL 代码的 API，通过它可以将 HLSL 代码转换为字节码数据，并提供多种编译选项以满足不同的需求。在 Direct3D 应用程序开发中，通常需要使用 D3DCompileFromFile 函数对着色器代码进行编译，以便让 GPU 正确地读取和处理着色器数据。

### 编译选项

当使用 D3DCompileFromFile 函数进行 HLSL 代码编译时，可以通过 Flags1 参数指定多个编译选项，以满足不同的需求。这些编译选项是一个按位或的掩码，可以使用常量 D3DCOMPILE_* 中的多个值进行组合。在下面的示例中，我们使用了两个编译选项：D3DCOMPILE_DEBUG 和 D3DCOMPILE_SKIP_OPTIMIZATION。

- D3DCOMPILE_DEBUG：表示开启调试模式，会生成包含调试信息的字节码数据，方便调试工具进行错误跟踪和性能分析等操作。
- D3DCOMPILE_SKIP_OPTIMIZATION：表示跳过优化阶段，即不对编译后的代码进行优化处理，以便于快速调试和验证着色器逻辑。

这两个编译选项的作用如下：

- D3DCOMPILE_DEBUG ：该标志将生成包含调试信息的字节码，使调试器能够更容易地跟踪 HLSL 代码的执行。通常此标志用于开发期间，以帮助调试 HLSL 代码中的错误。

- D3DCOMPILE_SKIP_OPTIMIZATION：此选项告诉编译器不要优化编译后的代码。这意味着您将获得一个 编译速度更快 的字节码文件，但也可能意味着您的程序运行速度较慢。此标志通常用于开发初期，以便在测试代码之前更轻松地检查它们。

需要注意的是，编译选项可以根据实际需求进行自由组合，例如可以使用 D3DCOMPILE_ENABLE_STRICTNESS 标记来开启严格模式，强制编译器检查 HLSL 代码中的所有错误，从而提高代码质量；又例如可以使用 D3DCOMPILE_PACK_MATRIX_ROW_MAJOR 或者 D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR 标记来指定矩阵数据排列方式等。

总之，在使用 D3DCompileFromFile 函数进行 HLSL 代码编译时，合理选择编译选项可以提高编译效率、优化代码性能和保证代码质量等多个方面的要求，因此需要仔细考虑和评估。



## HLSL-mul

HLSL-mul 是 HLSL（High-Level Shader Language）中的一个数学函数，用于实现矩阵和向量的乘法运算。在 HLSL 中，矩阵和向量是常用的数据类型之一，它们经常需要进行乘法运算来变换顶点坐标或者计算光照效果等。

HLSL-mul 函数的语法如下：

```c++
float4 mul(float4x4 A, float4 V);
```

其中，A 表示一个 4x4 的矩阵，V 表示一个 4D 向量。mul 函数将矩阵 A 和向量 V 相乘，并返回一个新的 4D 向量作为结果。

例如，下面的代码展示了如何使用 mul 函数对矩阵和向量进行乘法运算：

```c++
float4x4 WorldMatrix;   // 定义世界矩阵
float4 Position;        // 定义顶点位置

// 将世界矩阵和顶点位置相乘
Position = mul(WorldMatrix, Position);
```

在上面的代码中，首先定义了一个 4x4 的世界矩阵 WorldMatrix 和一个 4D 向量 Position，然后调用 mul 函数对它们进行乘法运算，将结果保存到 Position 变量中。这个过程实际上是对顶点的位置进行了变换，使其适应不同的场景和视角需求。

除了与向量的乘法运算外，HLSL-mul 函数还支持矩阵与矩阵之间的乘法运算，以及向量与矩阵之间的乘法运算。例如，可以使用 mul 函数将两个矩阵相乘，以实现复杂的变换操作；也可以使用 mul 函数将一个 3D 向量与一个 4x4 矩阵相乘，以实现三维空间中的投影变换。

总之，HLSL-mul 函数是一个用于实现矩阵和向量乘法运算的数学函数，在 HLSL 编程中广泛应用于顶点着色器、像素着色器和几何着色器等多个方面。掌握好 mul 函数的使用方法，可以帮助我们更加高效地进行着色器编程，并实现各种有趣的渲染效果。



# PSO绑定到渲染流水线

## PSO（流水线状态对象）

PSO（Pipeline State Object）是指在 DirectX 12 和 Vulkan 等新一代图形 API 中，用于描述图形渲染流水线状态的一种对象。它包含了多个状态参数，如顶点着色器、像素着色器、深度测试等，用于描述渲染管线中各种阶段的状态信息。

在传统的 Direct3D 11 中，渲染流水线状态是通过设置多个独立的对象来控制的，而在 DirectX 12 和 Vulkan 中，这些状态都被组织到单个 PSO 对象中，以提高渲染效率和可扩展性。通过使用 PSO，开发者可以预先定义多个渲染状态，然后在运行时选择其中一个状态进行渲染，从而实现更高效的图形渲染。

PSO 能够描述的渲染状态参数包括但不限于以下几类：

- 顶点着色器；
- 像素着色器；
- 几何着色器；
- 输入布局；
- 渲染目标格式；
- 深度/模板缓存状态；
- 光栅化状态；
- 多重采样状态。

我们可以通过创建 ID3D12PipelineState 或 VkPipeline 对象来创建 PSO。在创建 PSO 时，需要指定与某一特定渲染管线所关联的所有状态参数，并在必要时设置对应的值。创建完成后，就可以将 PSO 作为参数传递给绘制命令或者 Compute 命令来启动相应的渲染操作或计算操作。

例如，在 DirectX 12 中，我们可以使用以下代码来创建一个 PSO：

```c++
D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};    // 创建 PSO 描述符
psoDesc.InputLayout = { inputElementDescs.data(), (UINT)inputElementDescs.size() };
psoDesc.pRootSignature = m_RootSignature.Get();
psoDesc.VS.BytecodeLength = vertexShader->GetBufferSize();
psoDesc.VS.pShaderBytecode = vertexShader->GetBufferPointer();
psoDesc.PS.BytecodeLength = pixelShader->GetBufferSize();
psoDesc.PS.pShaderBytecode = pixelShader->GetBufferPointer();
psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
psoDesc.SampleMask = UINT_MAX;
psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
psoDesc.NumRenderTargets = 1;
psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
psoDesc.SampleDesc.Count = 1;

HRESULT hr = device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_PipelineState));
```

在上面的代码中，我们首先创建了一个 D3D12_GRAPHICS_PIPELINE_STATE_DESC 结构体，用于描述 PSO 的各个状态参数。然后，我们按需设置各个状态参数，例如输入布局、根签名、顶点着色器和像素着色器等，并调用 CreateGraphicsPipelineState 函数来创建一个 Graphics Pipeline State Object 对象，并将其保存到 m_PipelineState 变量中。

总之，PSO 是一种用于描述渲染管线状态的对象，在 DirectX 12 和 Vulkan 等新一代图形 API 中得到广泛应用。通过 PSO，我们可以预定义多个渲染状态，并在运行时选择其中一个状态进行渲染，从而提高图形渲染效率和可扩展性。



## PSO绑定到渲染流水线

将 PSO 绑定到渲染流水线是一种启动图形渲染的方法。在 DirectX 12 和 Vulkan 中，我们可以通过将 PSO 绑定到渲染管线中来启动相应的渲染操作。

在 DirectX 12 中，我们可以使用 ID3D12GraphicsCommandList::SetPipelineState 方法来将 PSO 绑定到渲染流水线中。例如，在以下代码片段中，我们创建了一个 Graphics Command List 对象，并将其用于绘制一个三角形：

```c++
// 创建 Graphics Command List
ComPtr<ID3D12GraphicsCommandList> commandList;
device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_CommandAllocator.Get(), nullptr, IID_PPV_ARGS(&commandList));

// 将 PSO 绑定到渲染管线
commandList->SetPipelineState(m_PipelineState.Get());

// 设置渲染目标和深度缓存视图
commandList->OMSetRenderTargets(1, &m_RTVHandle, FALSE, &m_DSVHandle);
commandList->ClearRenderTargetView(m_RTVHandle, Colors::CornflowerBlue, 0, nullptr);
commandList->ClearDepthStencilView(m_DSVHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

// 设置顶点缓存和索引缓存等数据
commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
commandList->IASetVertexBuffers(0, 1, &m_VertexBufferView);
commandList->IASetIndexBuffer(&m_IndexBufferView);

// 执行绘制命令
commandList->DrawIndexedInstanced(3, 1, 0, 0, 0);

// 完成渲染并关闭命令列表
commandList->Close();
```

在上面的代码中，我们首先创建了一个 Graphics Command List 对象，并将其用于绘制一个三角形。然后，我们将 m_PipelineState 对象作为参数传递给 SetPipelineState 方法，以将 PSO 绑定到渲染管线中。接下来，我们设置了渲染目标和深度缓存视图，并设置了顶点缓存和索引缓存等数据。最后，我们调用 DrawIndexedInstanced 方法执行绘制命令，并通过 Close 方法完成渲染并关闭命令列表。

在 Vulkan 中，我们可以使用 vkCmdBindPipeline 命令来将 PSO 绑定到渲染流水线中。例如，在以下代码片段中，我们创建了一个 Command Buffer 对象，并将其用于在屏幕上绘制一个矩形：

```c++
// 开始录制命令缓冲区
VkCommandBufferBeginInfo beginInfo = {};
beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
vkBeginCommandBuffer(commandBuffer, &beginInfo);

// 将 PSO 绑定到渲染管线
vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

// 开始渲染通道
VkRenderPassBeginInfo renderPassInfo = {};
renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
renderPassInfo.renderPass = renderPass;
renderPassInfo.framebuffer = framebuffer;
renderPassInfo.renderArea.offset = { 0, 0 };
renderPassInfo.renderArea.extent = swapChainExtent;

// 清除颜色和深度缓存
VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
VkClearValue clearDepthStencil = { 1.0f, 0 };
std::array<VkClearValue, 2> clearValues = { clearColor, clearDepthStencil };
renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
renderPassInfo.pClearValues = clearValues.data();

vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

// 绘制三角形
vkCmdDraw(commandBuffer, 3, 1, 0, 0);

// 结束渲染通道
vkCmdEndRenderPass(commandBuffer);

// 完
```



### D3D12_GRAPHICS_PIPELINE_STATE_DESC 渲染管线状态描述

D3D12_GRAPHICS_PIPELINE_STATE_DESC 是用于描述 DirectX 12 中图形管线状态的结构体类型，它包含了一系列成员变量，用于描述顶点着色器、像素着色器、光栅化器状态、深度/模板测试等渲染流水线中的各个阶段的状态信息。

下面是 D3D12_GRAPHICS_PIPELINE_STATE_DESC 结构体的定义：

```c++
typedef struct D3D12_GRAPHICS_PIPELINE_STATE_DESC {
    ID3D12RootSignature *pRootSignature;        // 根签名
    D3D12_SHADER_BYTECODE VS;                   // 顶点着色器
    D3D12_SHADER_BYTECODE PS;                   // 像素着色器
    D3D12_SHADER_BYTECODE DS;                   // 域着色器
    D3D12_SHADER_BYTECODE HS;                   // 曲面着色器
    D3D12_SHADER_BYTECODE GS;                   // 几何着色器
    D3D12_STREAM_OUTPUT_DESC StreamOutput;      // 流输出描述符
    D3D12_BLEND_DESC BlendState;                // 混合状态
    UINT SampleMask;                            // 多重采样遮罩
    D3D12_RASTERIZER_DESC RasterizerState;      // 光栅化状态
    D3D12_DEPTH_STENCIL_DESC DepthStencilState; // 深度/模板测试状态
    D3D12_INPUT_LAYOUT_DESC InputLayout;        // 输入布局描述符
    D3D12_INDEX_BUFFER_STRIP_CUT_VALUE IBStripCutValue; // 索引缓存条带裁剪值
    D3D12_PRIMITIVE_TOPOLOGY_TYPE PrimitiveTopologyType; // 图元拓扑类型
    UINT NumRenderTargets;                      // 渲染目标数量
    DXGI_FORMAT RTVFormats[8];                  // 渲染目标格式数组
    DXGI_FORMAT DSVFormat;                      // 深度/模板视图格式
    DXGI_SAMPLE_DESC SampleDesc;                // 多重采样描述符
    UINT NodeMask;                              // 节点掩码
    D3D12_CACHED_PIPELINE_STATE CachedPSO;      // 缓存的 PSO 对象
    D3D12_PIPELINE_STATE_FLAGS Flags;           // PSO 标志
} D3D12_GRAPHICS_PIPELINE_STATE_DESC;
```

在上面的定义中，D3D12_GRAPHICS_PIPELINE_STATE_DESC 结构体的成员变量非常丰富。其中，pRootSignature 表示该 PSO 所使用的根签名；VS、PS、DS、HS 和 GS 分别表示顶点着色器、像素着色器、域着色器、曲面着色器和几何着色器等着色器代码；StreamOutput 表示流输出描述符；BlendState、RasterizerState 和 DepthStencilState 分别表示混合状态、光栅化器状态和深度/模板测试状态；InputLayout 表示输入布局描述符；IBStripCutValue 表示索引缓存条带裁剪值；PrimitiveTopologyType 表示图元拓扑类型；NumRenderTargets 表示渲染目标数量；RTVFormats 表示渲染目标格式数组；DSVFormat 表示深度/模板视图格式；SampleDesc 表示多重采样描述符；NodeMask 表示节点掩码；CachedPSO 表示缓存的 PSO 对象；Flags 表示 PSO 的标志位。

总之，D3D12_GRAPHICS_PIPELINE_STATE_DESC 结构体是用于描述 DirectX 12 中图形管线状态的结构体类型。通过设置该结构体的成员变量，我们可以实现对 DirectX 12 渲染流水线各个阶段状态的自定义控制，并创建出符合需求的 Graphics Pipeline State Object（PSO）。



### CD3DX12_RASTERIZER_DESC 设置渲染的光栅化器状态

`CD3DX12_RASTERIZER_DESC`是DirectX 12中的一个辅助类，用于设置渲染的光栅化器状态。这个类封装了`D3D12_RASTERIZER_DESC`结构体并提供了一种方便的初始化方法。

光栅化器负责将场景的几何形状转换为屏幕上的像素，并确定如何着色这些像素。`CD3DX12_RASTERIZER_DESC`类允许您指定各种参数，这些参数影响光栅化器执行此任务的方式，例如填充模式（例如实心或线框）、剔除模式（例如正面或背面）和深度裁剪。

以下是使用`CD3DX12_RASTERIZER_DESC`创建具有默认设置的光栅化器的示例：

```cpp
CD3DX12_RASTERIZER_DESC rasterizerDesc(D3D12_DEFAULT);
```

这将创建具有默认设置的光栅化器对象。然后，您可以使用该类提供的各种setter方法修改各个属性。例如，要启用线框渲染，您需要调用：

```cpp
rasterizerDesc.FillMode = D3D12_FILL_MODE_WIREFRAME;
```

一旦您创建并配置了光栅化器，就可以使用命令列表的`SetPipelineState()`方法将其传递给图形管道。

当使用`CD3DX12_RASTERIZER_DESC`类创建光栅化器对象时，可以通过传递一个指向`D3D12_RASTERIZER_DESC`结构体的指针来设置默认值。在这种情况下，如果您没有显式地设置任何属性，则将使用以下默认值：

- FillMode：`D3D12_FILL_MODE_SOLID`（实心填充模式）
- CullMode：`D3D12_CULL_MODE_BACK`（背面剔除模式）
- FrontCounterClockwise：`FALSE`（卷曲顺序为顺时针）
- DepthBias：`D3D12_DEFAULT_DEPTH_BIAS`（深度偏差）
- DepthBiasClamp：`D3D12_DEFAULT_DEPTH_BIAS_CLAMP`（深度偏差夹紧）
- SlopeScaledDepthBias：`D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS`（斜率缩放深度偏差）
- DepthClipEnable：`TRUE`（启用深度裁剪）
- MultisampleEnable：`FALSE`（禁用多重采样）
- AntialiasedLineEnable：`FALSE`（禁用抗锯齿线段）

因此，在不修改它的任何属性的情况下，使用默认设置创建的`CD3DX12_RASTERIZER_DESC`对象将使用上述默认值。



### CD3DX12_BLEND_DESC 设置混合器状态

`CD3DX12_BLEND_DESC`是DirectX 12中的一个辅助类，用于设置混合器状态。混合器是一个可选的渲染管线组件，它控制如何将新的像素颜色与先前的像素颜色相结合。`CD3DX12_BLEND_DESC`类封装了D3D12_BLEND_DESC结构体并提供了一种方便的初始化方法。

在其默认设置下，混合器会禁用混合操作。以下是使用`CD3DX12_BLEND_DESC`类创建具有默认设置的混合器对象的示例：

```cpp
CD3DX12_BLEND_DESC blendDesc(D3D12_DEFAULT);
```

这将创建具有默认设置的混合器对象。您可以使用该类提供的各种setter方法修改各个属性，例如混合因子和混合操作类型。

以下是修改`CD3DX12_BLEND_DESC`对象的示例代码：

```cpp
CD3DX12_BLEND_DESC blendDesc(D3D12_DEFAULT);
blendDesc.RenderTarget[0].BlendEnable = true;
blendDesc.RenderTarget[0].LogicOpEnable = false;
blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
blendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
```

以上代码创建了一个启用alpha混合的混合器对象。在这个示例中，使用颜色源和反向颜色源逐通道地混合，并且alpha通道使用1作为源因子和0作为目标因子进行混合，这意味着源像素颜色完全替换目标像素颜色。另外注意到在这个示例中仅针对第一个渲染目标设置了混合器状态，如果有多个渲染目标，需要对每个渲染目标设置混合器状态。

当使用`CD3DX12_BLEND_DESC`类创建混合器对象时，可以通过传递一个指向`D3D12_BLEND_DESC`结构体的指针来设置默认值。在这种情况下，如果您没有显式地设置任何属性，则将使用以下默认值：

```c++
RenderTarget[0].BlendEnable = FALSE;
RenderTarget[0].LogicOpEnable = FALSE;
RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
```

在上述默认设置中，混合操作已经被禁用，这意味着新的像素颜色将不会与先前的像素颜色相结合，而是直接覆盖它们。这是因为所有的混合因子和混合操作类型都被设置成了对应于1和0的常量因子和加法操作。

以上默认设置只适用于渲染目标数组中的第一个渲染目标，并且只允许写入全部颜色通道。如果有多个渲染目标或需要更细粒度控制颜色通道，您需要显式地为每个渲染目标设置混合状态。

在`CD3DX12_BLEND_DESC`中，`D3D12_BLEND_DESC`结构体的各个属性含义如下：

- `BlendEnable`：一个布尔值，指示是否启用混合操作。如果设置为TRUE，则启用混合操作；否则将禁用混合操作。
- `LogicOpEnable`：一个布尔值，指示是否启用逻辑运算。如果设置为TRUE，则启用逻辑运算；否则将禁用逻辑运算。
- `SrcBlend`：源混合因子。该属性指定了新像素颜色和先前像素颜色之间的混合因子。
- `DestBlend`：目标混合因子。该属性指定了新像素颜色和先前像素颜色之间的混合因子。
- `BlendOp`：混合操作类型。该属性指定了新像素颜色和先前像素颜色之间的混合操作类型，例如加法、减法等等。
- `SrcBlendAlpha`：源alpha混合因子。与SrcBlend类似，但是应用于像素的alpha通道。
- `DestBlendAlpha`：目标alpha混合因子。与DestBlend类似，但是应用于像素的alpha通道。
- `BlendOpAlpha`：alpha混合操作类型。与BlendOp类似，但是应用于像素的alpha通道。
- `LogicOp`：逻辑运算类型。该属性指定逻辑运算的类型，例如AND、OR等等。
- `RenderTargetWriteMask`：用于向渲染目标写入像素颜色的掩码。该属性允许您选择要写入哪些颜色通道，例如红色、绿色、蓝色或alpha通道。

需要注意的是，以上属性只适用于渲染目标数组中的第一个渲染目标，如果有多个渲染目标，需要为每个渲染目标显式地设置混合状态。



### CD3DX12_DEPTH_STENCIL_DESC 设置深度和模板测试状态

`CD3DX12_DEPTH_STENCIL_DESC`是DirectX 12中的一个辅助类，用于设置深度和模板测试状态。深度和模板测试是可选的渲染管线组件，它们允许您控制哪些像素将被写入到帧缓冲区中。

`CD3DX12_DEPTH_STENCIL_DESC`类封装了D3D12_DEPTH_STENCIL_DESC结构体并提供了一种方便的初始化方法。以下是使用CD3DX12_DEPTH_STENCIL_DESC类创建具有默认设置的深度模板状态对象的示例：

```cpp
CD3DX12_DEPTH_STENCIL_DESC depthStencilDesc(D3D12_DEFAULT);
```

这将创建具有默认设置的深度模板状态对象。您可以使用该类提供的各种setter方法修改各个属性，例如启用或禁用深度测试、深度比较函数等等。

以下是修改`CD3DX12_DEPTH_STENCIL_DESC`对象的示例代码：

```cpp
CD3DX12_DEPTH_STENCIL_DESC depthStencilDesc(D3D12_DEFAULT);
depthStencilDesc.DepthEnable = TRUE;
depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
depthStencilDesc.StencilEnable = FALSE;
depthStencilDesc.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
depthStencilDesc.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
depthStencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
depthStencilDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
depthStencilDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
depthStencilDesc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
depthStencilDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
depthStencilDesc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
depthStencilDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
depthStencilDesc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
```

以上代码创建了一个启用了深度测试的深度模板状态对象。在这个示例中，深度测试启用，深度写入掩码设置为全部，深度比较函数设置为小于，模板测试禁用。

需要注意的是，在此示例中，模板测试被禁用了，因为`StencilEnable`属性被设置为FALSE。如果要启用模板测试，需要将`StencilEnable`设置为TRUE，并显式地指定各个模板测试函数和操作。