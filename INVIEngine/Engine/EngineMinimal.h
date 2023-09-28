#pragma once

#include <simple_library/simple_library.h>
#include "Log/SimpleLog.h"
#include "ColdeReflection/CodeReflectionMacro.h"

#include <vector>
#include <random>
#include <sstream>
#include <iostream>

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

#include "Rendering/DirectX12/d3dx12.h"
#include "Rendering/DirectX12/DDSTextureLoader.h"
#include "Debug/EngineDebug.h"
#include "Math/EngineMath.h"
#include "Component/Input/Input.h"

#include <wrl.h>
//< wrl.h > 头文件提供了一些关键类和模板，使得C++开发人员能够更轻松地创建和使用Windows Runtime组件和应用程序。其中一些类和模板包括：
// Microsoft::WRL::ComPtr：一个智能指针模板，用于管理Windows Runtime组件的引用计数。
// Microsoft::WRL::RuntimeClass：一个模板类，用于定义继承自Windows Runtime接口的C++类。
// Microsoft::WRL::Implements：一个模板类，用于实现一个或多个Windows Runtime接口。
// Microsoft::WRL::Make：一个模板函数，用于创建Windows Runtime组件的实例。
#include <dxgi1_4.h>
//< dxgi1_4.h > 是DirectX Graphics Infrastructure库（DXGI）中的一个头文件，它包含了一组C++接口和结构体，用于管理计算机上的图形适配器、显示器和窗口等硬件资源。
// DXGI库是Microsoft DirectX技术的一部分，它提供了一个通用的、中立的图形API，使得应用程序可以充分利用不同类型的硬件设备，并且可以在多个操作系统和平台上使用。DXGI库主要包括以下功能：
// 硬件资源管理：包括图形适配器、显示模式、输出源和窗口等硬件资源的管理和配置。
// 交换链：包括创建和管理与硬件显示器的交换链，以及实现双缓冲和三缓冲等高级渲染技术。
// 全屏模式：包括进入和退出全屏模式，以及处理多个显示器时的显示逻辑。
//<dxgi1_4.h>头文件是DXGI库中的一个特定版本，它引入了一些新的接口和结构体，用于支持DirectX 12和Windows 10操作系统。其中一些重要的接口和结构体包括：
// IDXGIFactory4：工厂类接口，用于创建并管理DXGI对象。
// DXGI_SWAP_CHAIN_DESC1：交换链描述结构体，用于描述交换链的属性和配置。
// DXGI_PRESENT_PARAMETERS：呈现参数结构体，用于指定呈现操作的属性和选项。
#include <d3d12.h>
//< d3d12.h > 是Direct3D 12 API的头文件，它提供了一组C++类、结构体和枚举类型，用于实现高性能图形渲染和计算。
// Direct3D 12是Microsoft DirectX技术中的一个子系统，它是一种底层的硬件抽象层（Hardware Abstraction Layer）
// 可以在Windows 10操作系统上利用多核CPU和新型GPU等硬件资源来实现更快速和更高效的图形处理功能。
// Direct3D 12相比于Direct3D 11，具有更细粒度的线程控制、更低的CPU开销、更短的渲染延迟等优势，适合于大规模游戏和虚拟现实应用程序等场景。
//<d3d12.h>头文件包含了一些关键的类和结构体，其中一些常用的类和结构体包括：
// ID3D12Device：设备接口，用于创建并管理Direct3D 12对象。
// D3D12_DESCRIPTOR_HEAP_DESC：描述符堆描述结构体，用于描述描述符堆的属性和配置。
// D3D12_RESOURCE_BARRIER：资源屏障结构体，用于描述资源状态转换的操作。
// D3D12_COMMAND_QUEUE_DESC：命令队列描述结构体，用于描述命令队列的属性和配置。
#include <D3Dcompiler.h>
//< D3Dcompiler.h > 是DirectX Shader Compiler的头文件，它提供了一组函数和结构体，用于编译和反编译HLSL着色器代码。
// DirectX Shader Compiler（DXC）是Microsoft DirectX技术中的一个子系统，它是一个独立的编译器工具，用于将高级别着色器语言（HLSL）代码转换为GPU可执行的低级别指令。DXC可以在Windows操作系统上运行，并且支持多个平台和硬件设备。
//<D3Dcompiler.h>头文件包含了一些关键的函数和结构体，其中一些常用的函数和结构体包括：
// D3DCompile：编译HLSL着色器代码的函数，将HLSL代码转换为GPU可执行的字节码。
// D3DDisassemble：反汇编GPU可执行字节码的函数，将字节码转换为HLSL代码。
// ID3DBlob：二进制大型对象接口，用于表示和处理字节数组。
#include <DirectXMath.h>
//< DirectXMath.h > 是DirectX Math库的头文件，它提供了一组C++函数和结构体，用于处理三维向量、矩阵和四元数等计算机图形学中常见的数学运算。
// DirectX Math是Microsoft DirectX技术中的一个子系统，它是一个高性能的数学库，用于支持计算机图形学、物理模拟、游戏编程和人工智能等领域。DirectX Math主要包括以下功能：
// 三维向量：包括向量加减、点积、叉积、长度归一化和插值等运算。
// 矩阵变换：包括矩阵乘法、逆矩阵、转置矩阵、缩放矩阵、旋转矩阵和平移矩阵等运算。
// 四元数：包括四元数加减、点积、叉积、长度归一化和插值等运算。
// 几何算法：包括射线检测、AABB包围盒测试、球体测试和平面测试等运算。
//<DirectXMath.h>头文件包含了一些关键的函数和结构体，其中一些常用的函数和结构体包括：
// XMVECTOR：向量类型，用于表示三维向量或四元数。
// XMMATRIX：矩阵类型，用于表示4x4矩阵。
// XMStoreFloat3 / 4：将XMVECTOR类型的向量转换为FLOAT3 / FLOAT4类型的向量。
// XMMatrixRotationQuaternion：根据四元数创建旋转矩阵。
#include <DirectXPackedVector.h>
//< DirectXPackedVector.h > 是DirectXMath库的一个头文件，它提供了一组C++结构体和函数，用于将32位浮点数和16位整数向量打包成4字节或2字节的格式。
// DirectXMath库是Microsoft DirectX技术中的一个子系统，它提供了高性能的数学库，用于支持计算机图形学、物理模拟、游戏编程和人工智能等领域。其中<DirectXPackedVector.h>头文件作为DirectXMath库的一部分，主要用于压缩数据大小，减少内存带宽。
//<DirectXPackedVector.h>头文件包含了一些关键的结构体和函数，其中一些常用的结构体和函数包括：
// XMVECTORU8/XMUBYTEN4：用于存储无符号8位整数向量。
// XMVECTORI4/XMSHORTN4：用于存储有符号16位整数向量。
// XMVectorPackFloat2/4：将两个或四个32位浮点数打包成16位整数向量。
// XMVectorPackUnsigned/Int2/4：将两个或四个16位整数打包成32位整数向量。
#include <DirectXColors.h>
//< DirectXColors.h > 是DirectX Tool Kit的一个头文件，它包含了一组C++常量和函数，用于表示常见颜色和生成颜色数组等功能。
// DirectX Tool Kit是Microsoft DirectX技术中的一个子系统，它提供了一组工具库，用于简化DirectX应用程序的开发。其中<DirectXColors.h>头文件作为DirectX Tool Kit的一部分，主要提供了方便快捷地获取常见颜色值和生成颜色数组的方法。
//<DirectXColors.h>头文件包含了一些关键的常量和函数，其中一些常用的常量和函数包括：
// DirectX::Colors::White / DarkGray / Gray / Black / Red / Green / Blue / Yellow / Cyan / Magenta：表示常见颜色的常量。
// DirectX::PackedVector::XMCOLOR：用于存储RGBA颜色的结构体。
// DirectX::PackedVector::XMUBYTEN4：用于存储无符号8位整数颜色值的结构体。
// DirectX::PackedVector::XMUDECN4 / XMUNIBBLE4：用于存储无符号4位和5位整数颜色值的结构体。
// DirectX::PackedVector::XMConvertColorTo * ：将不同类型颜色值转换成XMCOLOR或XMUBYTEN4类型。
#include <DirectXCollision.h>

//< DirectXCollision.h > 是DirectX Tool Kit的一个头文件，它包含了一组C++结构体和函数，用于计算和处理常见的碰撞检测和物理模拟问题。
// DirectX Tool Kit是Microsoft DirectX技术中的一个子系统，它提供了一组工具库，用于简化DirectX应用程序的开发。其中<DirectXCollision.h>头文件作为DirectX Tool Kit的一部分，主要提供了碰撞检测和物理模拟等功能。
//<DirectXCollision.h>头文件包含了一些关键的结构体和函数，其中一些常用的结构体和函数包括：
// DirectX::BoundingBox / DirectX::BoundingOrientedBox / DirectX::BoundingSphere / DirectX::BoundingFrustum：表示不同形状的包围盒的结构体。
// DirectX::BoundingVolumeHierarchy：表示包围盒层级结构的结构体。
// DirectX::Intersects：判断两个包围盒是否相交的函数。
// DirectX::ComputeIntersection：计算包围盒之间的最小平移距离和碰撞点等信息的函数。

using namespace Microsoft::WRL;
using namespace DirectX;
using namespace DirectX::PackedVector;

extern class CEngine *Engine;
