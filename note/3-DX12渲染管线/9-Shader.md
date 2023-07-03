# Shader

## HLSL

HLSL（High-Level Shader Language）是 Microsoft 开发的一种高级着色器语言，用于编写 GPU 着色器代码。HLSL 通常用于 Direct3D 应用程序开发中，可以用来描述顶点、像素、几何、计算等不同类型的着色器。与其他着色器语言相比，HLSL 具有以下优势：

- 独立于硬件：HLSL 可以在不同的图形卡上运行，而无需进行大量的调整和优化。
- 易于使用：HLSL 采用类 C 的语法，易于编写和理解。
- 功能丰富：HLSL 提供了许多内置函数和数据类型，可以执行矩阵变换、纹理采样、光照计算等常见任务。
- 调试支持：HLSL 支持利用 Visual Studio 进行调试，方便用户快速定位问题。

HLSL 程序通常由两个部分组成：输入和输出。输入部分定义了输入结构体，其中包含着色器需要的输入参数，如顶点坐标、颜色、纹理坐标等。输出部分则定义了输出结构体，其中包含着色器需要输出的结果，如像素颜色、深度值等。

例如，下面是一个简单的 HLSL 顶点着色器示例：

```hlsl
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