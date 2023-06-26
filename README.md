# INVIEngine



SNC和RPC是两种不同的通信协议，它们之间的主要区别如下：

1. SNC是Symmetric Network Computing的缩写，是一种对等网络计算协议。SNC允许用户在本地或远程计算机上运行应用程序，并且应用程序可以像在本地一样进行操作，而无需考虑程序所在的位置。SNC使用类似于RDP（远程桌面协议）的技术实现远程访问。
2. RPC是Remote Procedure Call的缩写，是一种远程过程调用协议。RPC允许一个进程（即客户端）发起请求，另一个进程（即服务器）响应该请求，并向客户端返回所需数据。RPC可以通过本地网络或Internet连接进行通信。
3. 主要应用场景不同：SNC主要应用于远程桌面、远程文件管理、共享会话等场景，RPC则主要应用于分布式系统、网络应用程序、Web服务等场景。
4. 通信模型不同：SNC使用点对点的通信模型，不需要中央服务器的支持；而RPC使用客户端-服务器模型。
5. 实现方式不同：SNC是由Symantec公司开发的专有协议，通常在Symantec提供的软件中使用；RPC是一种开放的标准协议，可以在各种平台和编程语言中使用。

综合来看，SNC和RPC是两种不同的通信协议，适用于不同的场景和应用。SNC主要面向个人用户，提供方便的远程访问功能；RPC主要面向企业用户，用于构建分布式系统和Web服务。



### 宽字符（wchar_t）和窄字符（char）

在C++中，字符有两种表示方式：窄字符和宽字符。它们的主要区别在于所占字节数和所能表示的字符集范围不同。

1. 窄字符：也称为ANSI字符，是一种占用一个字节（8位）内存空间的字符类型。它只能表示ASCII字符集中的字符，包括英文字母、数字和少数特殊符号。在C++中，通常使用char类型表示窄字符。
2. 宽字符：也称为Unicode字符，是一种占用两个字节（16位）内存空间的字符类型。它可以表示全球几乎所有语言中的字符，包括拉丁字母、汉字、日语假名等等。在C++中，通常使用wchar_t类型表示宽字符。

需要注意的是，由于宽字符类型所占内存空间较大，因此在某些情况下会影响程序性能和内存开销。因此，在处理单个ASCII字符或仅支持少数语言字符集的情况下，使用窄字符可能更加高效。但是，如果需要处理多语言字符或跨平台应用程序时，使用宽字符则更为合适。

在Windows操作系统中，API函数提供了对宽字符的支持，例如，CreateWindowExW函数用于创建宽字符窗口，wprintf函数用于使用宽字符格式化输出等。同时，Windows API还提供了一些宏定义来简化宽字符和窄字符类型之间的转换，如TCHAR、_T()、_TEXT()等。



### C++字符串前缀

在C++中，字符串字面值可以使用不同的前缀来表示不同的类型或特性。以下是常见的字符串前缀及其含义：

1. ""：普通字符串字面值（Narrow String Literal），由单引号括起来的字符序列，每个字符占一个字节。
2. L""：宽字符串字面值（Wide String Literal），由单引号括起来的字符序列，每个字符占两个字节。
3. u8""：UTF-8字符串字面值（UTF-8 String Literal），由单引号括起来的字符序列，使用UTF-8编码，每个字符占1到4个字节。
4. u""：UTF-16字符串字面值（UTF-16 String Literal），由单引号括起来的字符序列，使用UTF-16编码，每个字符占两个字节。
5. U""：UTF-32字符串字面值（UTF-32 String Literal），由单引号括起来的字符序列，使用UTF-32编码，每个字符占四个字节。
6. R"()"：原始字符串字面值（Raw String Literal），由括号内的字符序列组成，不需要使用转义符号。

其中，除了第一种普通字符串字面值以外，其他字符串字面值都是Unicode字符集编码的字符串字面值，用于处理多语言文本、网络传输、操作系统API调用等场景。

需要注意的是，C++11之后，还可以使用{}包围的编码点来表示Unicode字符，例如：

```
char16_t c1 = u'\u4e2d';
char32_t c2 = U'\U0001F60A';
```

该代码定义了两个Unicode字符，分别为中文字符“中”和表情符号“😊”。



## DX12开发需要包含的静态库和头文件

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

### #include <wrl.h>

//< wrl.h > 头文件提供了一些关键类和模板，使得C++开发人员能够更轻松地创建和使用Windows Runtime组件和应用程序。其中一些类和模板包括：
//Microsoft::WRL::ComPtr：一个智能指针模板，用于管理Windows Runtime组件的引用计数。
//Microsoft::WRL::RuntimeClass：一个模板类，用于定义继承自Windows Runtime接口的C++类。
//Microsoft::WRL::Implements：一个模板类，用于实现一个或多个Windows Runtime接口。
//Microsoft::WRL::Make：一个模板函数，用于创建Windows Runtime组件的实例。

### #include <dxgi1_4.h>

//< dxgi1_4.h > 是DirectX Graphics Infrastructure库（DXGI）中的一个头文件，它包含了一组C++接口和结构体，用于管理计算机上的图形适配器、显示器和窗口等硬件资源。
//DXGI库是Microsoft DirectX技术的一部分，它提供了一个通用的、中立的图形API，使得应用程序可以充分利用不同类型的硬件设备，并且可以在多个操作系统和平台上使用。DXGI库主要包括以下功能：
//硬件资源管理：包括图形适配器、显示模式、输出源和窗口等硬件资源的管理和配置。
//交换链：包括创建和管理与硬件显示器的交换链，以及实现双缓冲和三缓冲等高级渲染技术。
//全屏模式：包括进入和退出全屏模式，以及处理多个显示器时的显示逻辑。
//<dxgi1_4.h>头文件是DXGI库中的一个特定版本，它引入了一些新的接口和结构体，用于支持DirectX 12和Windows 10操作系统。其中一些重要的接口和结构体包括：
//IDXGIFactory4：工厂类接口，用于创建并管理DXGI对象。
//DXGI_SWAP_CHAIN_DESC1：交换链描述结构体，用于描述交换链的属性和配置。
//DXGI_PRESENT_PARAMETERS：呈现参数结构体，用于指定呈现操作的属性和选项。

### #include <d3d12.h>

//< d3d12.h > 是Direct3D 12 API的头文件，它提供了一组C++类、结构体和枚举类型，用于实现高性能图形渲染和计算。
//Direct3D 12是Microsoft DirectX技术中的一个子系统，它是一种底层的硬件抽象层（Hardware Abstraction Layer）
//可以在Windows 10操作系统上利用多核CPU和新型GPU等硬件资源来实现更快速和更高效的图形处理功能。
//Direct3D 12相比于Direct3D 11，具有更细粒度的线程控制、更低的CPU开销、更短的渲染延迟等优势，适合于大规模游戏和虚拟现实应用程序等场景。
//<d3d12.h>头文件包含了一些关键的类和结构体，其中一些常用的类和结构体包括：
//ID3D12Device：设备接口，用于创建并管理Direct3D 12对象。
//D3D12_DESCRIPTOR_HEAP_DESC：描述符堆描述结构体，用于描述描述符堆的属性和配置。
//D3D12_RESOURCE_BARRIER：资源屏障结构体，用于描述资源状态转换的操作。
//D3D12_COMMAND_QUEUE_DESC：命令队列描述结构体，用于描述命令队列的属性和配置。

### #include <D3Dcompiler.h>

//< D3Dcompiler.h > 是DirectX Shader Compiler的头文件，它提供了一组函数和结构体，用于编译和反编译HLSL着色器代码。
//DirectX Shader Compiler（DXC）是Microsoft DirectX技术中的一个子系统，它是一个独立的编译器工具，用于将高级别着色器语言（HLSL）代码转换为GPU可执行的低级别指令。DXC可以在Windows操作系统上运行，并且支持多个平台和硬件设备。
//<D3Dcompiler.h>头文件包含了一些关键的函数和结构体，其中一些常用的函数和结构体包括：
//D3DCompile：编译HLSL着色器代码的函数，将HLSL代码转换为GPU可执行的字节码。
//D3DDisassemble：反汇编GPU可执行字节码的函数，将字节码转换为HLSL代码。
//ID3DBlob：二进制大型对象接口，用于表示和处理字节数组。

### #include <DirectXMath.h>

//< DirectXMath.h > 是DirectX Math库的头文件，它提供了一组C++函数和结构体，用于处理三维向量、矩阵和四元数等计算机图形学中常见的数学运算。
//DirectX Math是Microsoft DirectX技术中的一个子系统，它是一个高性能的数学库，用于支持计算机图形学、物理模拟、游戏编程和人工智能等领域。DirectX Math主要包括以下功能：
//三维向量：包括向量加减、点积、叉积、长度归一化和插值等运算。
//矩阵变换：包括矩阵乘法、逆矩阵、转置矩阵、缩放矩阵、旋转矩阵和平移矩阵等运算。
//四元数：包括四元数加减、点积、叉积、长度归一化和插值等运算。
//几何算法：包括射线检测、AABB包围盒测试、球体测试和平面测试等运算。
//<DirectXMath.h>头文件包含了一些关键的函数和结构体，其中一些常用的函数和结构体包括：
//XMVECTOR：向量类型，用于表示三维向量或四元数。
//XMMATRIX：矩阵类型，用于表示4x4矩阵。
//XMStoreFloat3 / 4：将XMVECTOR类型的向量转换为FLOAT3 / FLOAT4类型的向量。
//XMMatrixRotationQuaternion：根据四元数创建旋转矩阵。

### #include <DirectXPackedVector.h>

//< DirectXPackedVector.h > 是DirectXMath库的一个头文件，它提供了一组C++结构体和函数，用于将32位浮点数和16位整数向量打包成4字节或2字节的格式。
//DirectXMath库是Microsoft DirectX技术中的一个子系统，它提供了高性能的数学库，用于支持计算机图形学、物理模拟、游戏编程和人工智能等领域。其中<DirectXPackedVector.h>头文件作为DirectXMath库的一部分，主要用于压缩数据大小，减少内存带宽。
//<DirectXPackedVector.h>头文件包含了一些关键的结构体和函数，其中一些常用的结构体和函数包括：
//XMVECTORU8/XMUBYTEN4：用于存储无符号8位整数向量。
//XMVECTORI4/XMSHORTN4：用于存储有符号16位整数向量。
//XMVectorPackFloat2/4：将两个或四个32位浮点数打包成16位整数向量。
//XMVectorPackUnsigned/Int2/4：将两个或四个16位整数打包成32位整数向量。

### #include <DirectXColors.h>

//< DirectXColors.h > 是DirectX Tool Kit的一个头文件，它包含了一组C++常量和函数，用于表示常见颜色和生成颜色数组等功能。
//DirectX Tool Kit是Microsoft DirectX技术中的一个子系统，它提供了一组工具库，用于简化DirectX应用程序的开发。其中<DirectXColors.h>头文件作为DirectX Tool Kit的一部分，主要提供了方便快捷地获取常见颜色值和生成颜色数组的方法。
//<DirectXColors.h>头文件包含了一些关键的常量和函数，其中一些常用的常量和函数包括：
//DirectX::Colors::White / DarkGray / Gray / Black / Red / Green / Blue / Yellow / Cyan / Magenta：表示常见颜色的常量。
//DirectX::PackedVector::XMCOLOR：用于存储RGBA颜色的结构体。
//DirectX::PackedVector::XMUBYTEN4：用于存储无符号8位整数颜色值的结构体。
//DirectX::PackedVector::XMUDECN4 / XMUNIBBLE4：用于存储无符号4位和5位整数颜色值的结构体。
//DirectX::PackedVector::XMConvertColorTo * ：将不同类型颜色值转换成XMCOLOR或XMUBYTEN4类型。

### #include <DirectXCollision.h>

//< DirectXCollision.h > 是DirectX Tool Kit的一个头文件，它包含了一组C++结构体和函数，用于计算和处理常见的碰撞检测和物理模拟问题。
//DirectX Tool Kit是Microsoft DirectX技术中的一个子系统，它提供了一组工具库，用于简化DirectX应用程序的开发。其中<DirectXCollision.h>头文件作为DirectX Tool Kit的一部分，主要提供了碰撞检测和物理模拟等功能。
//<DirectXCollision.h>头文件包含了一些关键的结构体和函数，其中一些常用的结构体和函数包括：
//DirectX::BoundingBox / DirectX::BoundingOrientedBox / DirectX::BoundingSphere / DirectX::BoundingFrustum：表示不同形状的包围盒的结构体。
//DirectX::BoundingVolumeHierarchy：表示包围盒层级结构的结构体。
//DirectX::Intersects：判断两个包围盒是否相交的函数。
//DirectX::ComputeIntersection：计算包围盒之间的最小平移距离和碰撞点等信息的函数。



## HRESULT错误码

HRESULT是Windows操作系统中的一种错误码类型，它是由Microsoft定义的32位整数值，用于表示函数和方法的执行结果。HRESULT通常用于COM（Component Object Model）接口和Win32 API等Windows编程模型中，用于指示函数和方法的状态和错误信息

​	// HRESULT
​	// S_OK					0x00000000	执行成功
​	// E_UNEXPECTED			0x8000FFFF	意外的失败
​	// E_NOTIMPL			0x80004001	未实现
​	// E_OUTOFMEMORY		0x8007000E	未能分配所需内存
​	// E_INVALIDARG			0x80070057	一个或者多个参数无效
​	// E_NOINTERFACE		0x80004002	不支持此接口
​	// E_POINTER			0x80004003	无效指针
​	// E_HANDLE				0x80070006	无效句柄
​	// E_ABORT				0x80004004	操作终止
​	// E_ACCESSDENIED		0x80070005	一般的访问被拒绝错误



## DirectX渲染图形的步骤通常包括以下几个阶段：

1. 初始化Direct3D 12：在应用程序启动时，需要初始化Direct3D 12设备和命令队列。这一步可以使用D3D12CreateDevice和ID3D12Device::CreateCommandQueue等函数来实现。
2. 创建交换链和后台缓冲区：交换链是用于管理前后缓冲区的一种机制，它负责接收来自GPU的渲染结果，并将其显示到屏幕上。交换链可以通过IDXGIFactory4::CreateSwapChainForHwnd等函数创建。
3. 创建顶点缓冲区和索引缓冲区：在场景中显示一个模型通常需要使用顶点缓冲区和索引缓冲区来存储模型数据。顶点缓冲区保存每个顶点的位置、法向量、纹理坐标等信息；索引缓冲区保存顶点索引，以便进行三角形列表或者三角形条带的绘制。顶点缓冲区和索引缓冲区可以通过ID3D12Device::CreateCommittedResource等函数创建。
4. 编写着色器代码：着色器是用于计算每个像素颜色的程序。它们通常由 HLSL (High Level Shader Language) 编写并编译成字节码。着色器包括顶点着色器（Vertex Shader）、像素着色器（Pixel Shader）、几何着色器（Geometry Shader）等。
5. 创建根签名和管线状态对象：根签名是用于描述着色器输入数据的结构体，管线状态对象则包含了管线的各种状态信息（如采样器、深度测试、光栅化等）。它们都可以使用ID3D12Device::CreateRootSignature和ID3D12Device::CreateGraphicsPipelineState等函数来创建。
6. 设置命令列表：在渲染场景之前，需要设置命令列表来告诉GPU如何执行绘制操作。命令列表包括清空背景、设置根签名、设置管线状态对象、设置视口和裁剪矩形等。
7. 执行命令列表：将命令列表提交给GPU，让它开始执行绘制操作。这可以通过调用ID3D12CommandQueue::ExecuteCommandLists函数来实现。
8. 呈现：当GPU完成绘制操作后，需要呈现结果到屏幕上。这可以通过IDXGISwapChain3::Present函数来实现。



