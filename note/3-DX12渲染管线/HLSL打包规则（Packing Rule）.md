## HLSL打包规则（Packing Rule）

## 一个BUG

在我们使用DirectX API做渲染的时候，经常需要使用到Constant Buffer来将CPU数据传递到GPU中。假设我们在CPU端有一个DataCpp结构体和一个实例化对象data，如下：

```cpp
struct DataCpp
{
    XMFLOAT4 val1;
    XMFLOAT3 val2;
    XMFLOAT2 val3;
    float val4;
};

DataCpp data;
data.val1 = { 0.1f, 0.0f, 0.4f, 1.0f };
data.val2 = { 0.5f, 0.8f, 0.6f };
data.val3 = { 0.7f, 0.2f };
data.val4 = 0.3f;
```

如果我们要利用Constant Buffer来将data里的数据传递到GPU端供Shader使用，首先在Shader中我们需要一个对应的cbuffer来接收这些数据，如下：

```cpp
cbuffer DataHLSL : register(b0)
{
    float4 val1;
    float3 val2;
    float2 val3;
    float val4;
};
```

接着我们需要创建一个Constant Buffer类型的Resource，然后利用**memcpy**将data里的数据拷贝到Constant Buffer中。经过噼里啪啦一顿操作后，在Shader中成功拿到了从CPU传递过来的data值，但是此时你会发现Shader里val3、val4的值和CPU中的data.val3、data.val4并不一样，Why？

```cpp
// shader中的四个值如下：
val1 = { 0.1f, 0.0f, 0.4f, 1.0f };
val2 = { 0.5f, 0.8f, 0.6f };
val3 = { 0.2f, 0.3f };
val4 = 0.0f;
```

造成上诉问题的罪魁祸首就是：在HLSL中，Constant Buffer遵循了打包规则。



## 打包规则

从Direct3D 10开始，HLSL中常量变量（Constant Variable）遵循了打包规则，它规定了数据存储时排列的紧密程度。

当我们有一批数据，它们的类型可能是int（4Byte）、double（8Byte）、float（4Byte）、float4（16Byte）等等，在打包规则下，这些数据会被依次装进**寄存器**（register）中，**一个寄存器可以存储16Byte的数据**，因此我们可以把它看作是一个四维向量，每个分量为一个float（4Byte）。当某个数据无法再完整的塞进当前的寄存器（/四维向量）时，会将其塞进一个新的寄存器中，也就是说一个数据没法存储在不同的寄存器中（对于Matrix这类大小可能大于16Byte的类型，后续介绍）。然后用这个新的寄存器继续塞后续的数据，如果又塞不下了，就再用一个新的，直到将所有数据都塞完。因此对于任意数据的组合，应用了打包规则后，其大小一定可以被16Byte整除。

如果把寄存器看做是大小相同的箱子，这些数据就是大小不一的物体。打包规则就是将这些物体依次往箱子里装，当某个物体装不下时，就需要用一个新的箱子继续装，直到装完所有的物体。

前面提到的Constant Buffer作为常量变量中的一种，自然也应用了打包规则。我们来看看DataHLSL中的成员在内存中是如何存储的：

1. 第一个成员是float4类型的，占用16Byte，正好可以装满第一个寄存器（register 0）。

![img](https://pic3.zhimg.com/80/v2-b87edc75f35d959c0377a74e2914d632_720w.webp)

pack val1

2. 此时register 0已经装满了，因此需要一个新的寄存器（register 1）来存储。第二个成员是float3类型，占用12Byte，因此会占用register 1的前三个分量。

![img](https://pic4.zhimg.com/80/v2-21677d8be709c15d93c04788b73e68af_720w.webp)

pack val2

3. 此时register 1还有4Byte可以用来存储，但是第三个成员是float2，占用8Byte。因为register 1的剩余空间并不够用，且成员不能分开来存储在不同的寄存器，因此我们又需要一个新的寄存器（register 2）来存储，占用前两个分量。

![img](https://pic4.zhimg.com/80/v2-c7c6ceac3e7bacedd3f3eeaef028768f_720w.webp)

pack val3

4. register 2还有8Byte可以用来存储，且第四个成员是float，占用4Byte，完全够用，因此可以直接存储在register 2的第三个分量上。

![img](https://pic2.zhimg.com/80/v2-9be040421970863351417f0f62483d9d_720w.webp)

pack val4

上图即DataHLSL成员在显存中的排列情况，这4个成员占用了3个寄存器，44Byte的大小（register 2还空了4Byte）。实际上cbuffer的大小是256Byte对齐的，因此DataHLSL的四个成员后，还会有256-44=212Byte的padding。

利用**fxc.exe的/Fc选项**生成Shader编译后的汇编代码文件，我们可以看到当前Shader所用到的cbuffer的内存排列情况。例如生成的汇编文件中对于DataHLSL的注释如下：

```cpp
// cbuffer DataHLSL
// {
//   float4 val1;                       // Offset:    0 Size:    16 [unused]
//   float3 val2;                       // Offset:   16 Size:    12 [unused]
//   float2 val3;                       // Offset:   32 Size:     8 [unused]
//   float val4;                        // Offset:   40 Size:     4
// }
```

> 注：[unused] 表示该成员在Shader中没被用到。

Offset即表示当前成员在内存中的偏移量，Size则表示当前成员在内存中所占用的大小。val3的Offset并不是val2的Offset+Size之和，说明val2和val3并不是连续的，中间空了4Byte。

此外也可以使用D3D12专属的调试工具[PIX](https://link.zhihu.com/?target=https%3A//devblogs.microsoft.com/pix/documentation/)来看cbuffer的内存分布情况，并且利用PIX还可以很直观的看的cbffer中的值：

![img](https://pic1.zhimg.com/80/v2-94aefc12ba0f0fd1d65c61010f1ccc5c_720w.webp)

DataHLSL

> 注：[[offset(32)]]float2 val3; 即代表val3的内存偏移量为32。pad 212表示212Byte的内存空间没被用到。



## C++与HLSL的内存不匹配

由于打包规则会让很多的成员在内存中并不是连续的，因此当我们利用cpu给cbuffer传值时，就可能出现两端内存不匹配的问题，这也就导致了最初说到的那个BUG，即cbuffer的值和期望的并不一致。

例子中C++中的DataCpp，成员看着和DataHLSL一模一样，但是它在内存中排序和DataHLSL却不一致。如下图，DataCpp中的val2和val3是连续的，DataCpp只占用40Byte。

![img](https://pic4.zhimg.com/80/v2-ff78b29bd2a7b102cea52fe68f1f4bf3_720w.webp)

DataCpp的内存分布

当我们使用memcpy进行拷贝的时候，会把cpu端这连续的40Byte的内存里的数据拷贝到显存中。而GPU读取的时候，是按照DataHLSL在显存中的结构去读取的，而显存中DataHLSL.val3的内存位置对应的是CPU端DataCpp.val3.y和DataCpp.val4的内存位置。这就导致了memcpy后，DataHLSL.val3=(DataCpp.val3.y, DataCpp.val4)。而DataHLSL.val4所在的内存位置并没有被memcpy所影响到（只拷贝了前40Byte对应的数据），因此它的值为默认值。

为了解决这样的问题，我们可以手动添加padding成员来使得两端的内存分布一致，例如：

```cpp
struct DataCpp
{
    XMFLOAT4 val1;
    XMFLOAT3 val2;
    float padding;
    XMFLOAT2 val3;
    float val4;
};
```

亦或者，我们可以通过一些巧妙的排序来避免由于打包规则导致的内存不匹配，例如：

```cpp
struct DataCpp
{
    XMFLOAT4 val1;
    XMFLOAT3 val2;
    float val4;
    XMFLOAT2 val3;
};

cbuffer DataHLSL : register(b0)
{
    float4 val1;
    float3 val2;
    float val4;
    float2 val3;
};
```

我们只是简单的改了下val3和val4的顺序，就可以完美避开问题。

> 注：同时也需要注意C++的**字节对齐**（Byte Alignment）规则。

还有一种方法是在HLSL中利用packoffset来手动打包，后续介绍。



## Shader的输入输出

打包规则还被用于Vertex Shader的输出，Geometry Shader的输入及输出，Fragment/Pixel Shader的输入及输出。对于Vertex Shader的输入，不会通过该规则被打包，因为Input Assembler阶段没法对打包后的数据进行拆包。

相关信息我们同样可以通过Shader的汇编文件来了解。例如Vertex Shader的输入和输出如下：

```cpp
struct VSOutput
{
    float4 position : SV_POSITION;
    float blendWeight : BLENDWEIGHT;
    float pointSize : PSIZE;
    float3 positionInWorld : POSITION;
    float3 normalInWorld : NORMAL;
};

VSOutput VSMain(float3 position : POSITION, float blendWeight : BLENDWEIGHT,
    float pointSize : PSIZE, float3 normal : NORMAL)
{
    VSOutput result;
    ......
    return result;
}
```

生成的汇编文件中会有如下注释：

```cpp
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// POSITION                 0   xyz         0     NONE   float   xyz 
// BLENDWEIGHT              0   x           1     NONE   float   x   
// PSIZE                    0   x           2     NONE   float   x   
// NORMAL                   0   xyz         3     NONE   float   xyz 
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_POSITION              0   xyzw        0      POS   float   xyzw
// BLENDWEIGHT              0   x           1     NONE   float   x   
// PSIZE                    0    y          1     NONE   float    y  
// POSITION                 0   xyz         2     NONE   float   xyz 
// NORMAL                   0   xyz         3     NONE   float   xyz 
```

Input signature为Vertex Shader的输入，从中可以发现每个成员（不管什么类型）都在一个单独的寄存器中。Output signature为Vertex Shader的输出，可以发现BLENDWEIGHT和PSIZE被打包在了同一个寄存器中，其他成员也符合打包规则。

![img](https://pic3.zhimg.com/80/v2-a3020761cf39ed8a7472f108aa536412_720w.webp)

VS Output

> 注：PIX中，Vertex Shader输出的大小显示是60Byte，即这些变量在内存中是连续的，并不符合打包规则，不清楚是不是哪里理解错了~



## 一些特殊类型

前面介绍的都是一些基本类型，其大小都在16Byte（一个寄存器大小）内，但是我们知道HLSL中还有类似float4x4这样的矩阵类型，float[]这样的数组类型，亦或是我们自定义的Struct，它们的大小都可能超过16Byte，我们来看看它们是如何被打包的。



### Struct

先来看看Struct，测试代码如下：

```cpp
struct Custom1
{
    uint c1;
};
struct Custom2
{
    int c1;
    float2 c2;
    float3 c3;
};
cbuffer DataHLSL : register(b0)
{
    bool val1;
    Custom1 val2;
    double val3;
    Custom2 val4;
    float val5;
};
```

同样的，来看看汇编文件里的注释：

```cpp
// cbuffer DataHLSL
// {
//   bool val1;                         // Offset:    0 Size:     4 [unused]
//   
//   struct Custom1
//   {
//       uint c1;                       // Offset:   16
//   } val2;                            // Offset:   16 Size:     4 [unused]
//   double val3;                       // Offset:   24 Size:     8 [unused]
//   
//   struct Custom2
//   {
//       int c1;                        // Offset:   32
//       float2 c2;                     // Offset:   36
//       float3 c3;                     // Offset:   48
//   } val4;                            // Offset:   32 Size:    28
//   float val5;                        // Offset:   60 Size:     4 [unused]
// }
```

> 注：HLSL中，bool占用4Byte，c++中只占用1Byte。

可以发现，**对于Struct成员，不管该Struct占用多少大小，都会用一个新的寄存器来存储该Struct对象**，即使之前的寄存器还有足够的空间可以存下该Struct（val1和val2都只有4Byte，但却不在同一个寄存器中）。**对于Struct内的成员，同样遵守打包规则**（val4.c1和val4.c2在同一个寄存器中，val4.c3在下一个寄存器中）。**当Struct内的成员都存储完毕后，若当前的寄存器还有足够的空间可以存储cbuffer后续的成员，则可以继续使用当前的寄存器**（val3和val2在同一个寄存器中，val5和val4.c3在同一个寄存器中）。



### Matrix

接着再来看看Matrix类型的存储规则，测试代码如下：

```cpp
cbuffer DataHLSL : register(b0)
{
    float2 val1;
    float2x1 val2;
    float2 val3;
    float1x2 val4;
    float val5;
    float3x3 val6;
    float val7;
};
```

对应汇编文件里的注释：

```cpp
// cbuffer DataHLSL
// {
//   float2 val1;                       // Offset:    0 Size:     8 [unused]
//   float2x1 val2;                     // Offset:    8 Size:     8 [unused]
//   float2 val3;                       // Offset:   16 Size:     8 [unused]
//   float1x2 val4;                     // Offset:   32 Size:    20 [unused]
//   float val5;                        // Offset:   52 Size:     4 [unused]
//   float3x3 val6;                     // Offset:   64 Size:    44 [unused]
//   float val7;                        // Offset:  108 Size:     4
// }
```

可以发现，**对于floatmxn类型，当n=1时，floatmx1等价于floatm，例如float2x1的打包方式和float2一样**。而对于其他情况，就像Struct类型，都**需要从一个新的寄存器开始存储，mxn可以理解为需要n个寄存器，每个寄存器的前m个分量用于存储**。例如float1x2，需要两个寄存器，每个寄存器的第一个分量用来存储，因此Size大小为第一个寄存器的大小加上第二个寄存器第一个分量的大小，即16+4=20。同理float3x3，需要3个寄存器，Size=16*2+3*4=44。**Matrix存储完后，若当前的寄存器还有足够的空间可以存储cbuffer后续的成员，同样可以继续使用**（val5和val4._01在一个寄存器）。

可以发现，这种情况下，**每个矩阵同一列的数据会存放在同一个寄存器中，而不同列的数据则在不同的寄存器中**。这是因为编译器默认使用**column_major**来处理矩阵，该方式有利于向量与矩阵相乘的计算。例如，在HLSL中，我们把向量和矩阵相乘的代码如下：

```cpp
cbuffer DataHLSL : register(b0)
{
    float4x4 mvp;
};
float4 vec = float4(1.0f, 0.0f, 0.5f, 0.0f);
float4 res = mul(vec, mvp);
```

通过矩阵的乘法我们知道，res.x的值等于vec各个分量的值与mvp第一列中各个分量的值的乘积之和。根据打包规则我们知道，vec的各个分量肯定在同一个寄存器中，而column_major下mvp的各列的分量也在同一个寄存器中，那么只需要将vec所在的寄存器与mvp第一列所在的寄存器各个分量相乘再相加即可得到res.x的值，res.yzw也同理。

上面的HLSL对应的汇编代码如下：

```cpp
// Resource Bindings:
//
// Name                                 Type  Format         Dim      ID      HLSL Bind  Count
// ------------------------------ ---------- ------- ----------- ------- -------------- ------
// DataHLSL                          cbuffer      NA          NA     CB0            cb0      1 

dcl_constantbuffer CB0[0:0][4], immediateIndexed, space=0

mov r0.xyzw, l(1.000000,0,0.500000,0)

dp4 r1.x, r0.xyzw, CB0[0][0].xyzw
dp4 r1.y, r0.xyzw, CB0[0][1].xyzw
dp4 r1.z, r0.xyzw, CB0[0][2].xyzw
dp4 r1.w, r0.xyzw, CB0[0][3].xyzw
```

注释部分表示将register(b0)上的DataHLSL绑定到了CB0上，[dcl_constantbuffer](https://link.zhihu.com/?target=https%3A//learn.microsoft.com/en-us/windows/win32/direct3dhlsl/dcl-constantbuffer)声明了有一个Constant Buffer CB0在space 0、slot 0上，占用了4个寄存器。

mov操作即给vec赋值，r0表示vec所在的寄存器。

[dp4](https://link.zhihu.com/?target=https%3A//learn.microsoft.com/en-us/windows/win32/direct3dhlsl/dp4--sm4---asm-) dest, src0, src1 则表示分量的相乘之和，即：

> dest=src0.r * src1.r + src0.g * src1.g + src0.b * src1.b + src0.a * src1.a

r1表示res所在的寄存器，CB0[0][n]代表CB0[0]中的第n个寄存器，也就是mvp第n列。因此使用column_major来存储矩阵，只需要通过四次的dp4就实现了向量与矩阵的相乘。

特别注意：**CPU端存储矩阵是先行后列的顺序，与GPU的默认情况（column_major）正好相反**，示意图如下：

![img](https://pic2.zhimg.com/80/v2-f01de01e45e5788d914d69fde06be031_720w.webp)

CPU矩阵传递到GPU

因此当我们要把CPU的矩阵通过Constant Buffer传递到GPU时，可以在存储矩阵时进行**矩阵的转置**操作，使其在内存中也变成先列后行的顺序：

```cpp
XMStoreFloat4x4(&constantBuffer.matrix, XMMatrixTranspose(matrix));
```

举个例子，假如传递一个3x3的矩阵如下：

```text
// C++
struct DataCpp
{
    XMFLOAT3X3 val1 = { 0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f };
};

// HLSL
cbuffer DataHLSL : register(b0)
{
    float3x3 val1;
};
```

在不进行任何特殊处理的情况下，它们的映射关系如下图所示：

![img](https://pic3.zhimg.com/80/v2-a4ea3f93656135da419de76dcd28ca22_720w.webp)

3x3矩阵的映射关系

例如CPU端矩阵第三行第一列DataCpp.val1._31的值为0.7，而在GPU端第三行第一列的值DataCpp.val1._31（或._m20）的值为0.3。因此在传递矩阵的时候一定要注意内存上的映射关系。

除了前面默认的column_major情况，我们还可以通过**row_major**关键字来改变矩阵的存储关系，使其变成**矩阵的每一行数据在同一个寄存器中**。此时，对于floatmxn类型，当m=1时，float1xn等价于floatn。其他情况下**，**mxn可以理解为需要m个寄存器，每个寄存器的前n个分量用于存储。

```cpp
// cbuffer DataHLSL
// {
//   float2 val1;                       // Offset:    0 Size:     8 [unused]
//   row_major float2x1 val2;           // Offset:   16 Size:    20 [unused]
//   float2 val3;                       // Offset:   36 Size:     8 [unused]
//   row_major float1x2 val4;           // Offset:   48 Size:     8 [unused]
//   float val5;                        // Offset:   56 Size:     4 [unused]
//   row_major float3x3 val6;           // Offset:   64 Size:    44 [unused]
//   float val7;                        // Offset:  108 Size:     4
// }
```

使用row_major后，对应的向量与矩阵的乘法的汇编写法也会变得复杂，如下：

```cpp
mul r1.xyzw, r0.xxxx, CB0[3][0].xyzw
mul r2.xyzw, r0.yyyy, CB0[3][1].xyzw
add r1.xyzw, r1.xyzw, r2.xyzw
mul r2.xyzw, r0.zzzz, CB0[3][2].xyzw
add r1.xyzw, r1.xyzw, r2.xyzw
mul r0.xyzw, r0.wwww, CB0[3][3].xyzw
add r0.xyzw, r0.xyzw, r1.xyzw 
```

如果所有矩阵都想用row_major来存储，我们也不需要在声明矩阵的时候一个个添加row_major关键字，可以利用**#pragma pack_matrix**一键搞定：

```cpp
#pragma pack_matrix( row_major )
```



### Array

对于数组类型，同样先来看一个例子：

```cpp
cbuffer DataHLSL : register(b0)
{
    float2 val1[4];
    float val2[4];
    float val3;
};
```

对应汇编文件里的注释：

```cpp
// cbuffer DataHLSL
// {
//   float2 val1[4];                    // Offset:    0 Size:    56 [unused]
//   float val2[4];                     // Offset:   64 Size:    52 [unused]
//   float val3;                        // Offset:  116 Size:     4
// }
```

可以发现，**对于数组成员，都会从一个新的寄存器开始存储**（val2[0]没有和val1[4]在同一个寄存器中）**，数组中的每个成员都会占用一个寄存器**（val1和val2占用的寄存器数相同）**。当整个数组都存储完毕后，若当前的寄存器还有足够的空间可以存储cbuffer后续的成员，则可以继续使用当前的寄存器**（val3和val2[1]在同一个寄存器中）。

因此使用数组时，我们要合理选择数组成员的类型，来减少内存的浪费。例如我们要使用float[18]，如果在cbuffer中声明这样的数组会占用18*16=288Byte的空间，由于Constant Buffer是256Byte对齐的，直接导致float[18]在CPU和GPU各需要占用512Byte的内存，但是其中实际有效的数据只有18*4=72Byte。但是如果我们一定要在HLSL中使用float[]这样的数组（比如一些遍历操作）怎么办呢？可以通过下面办法来优化：

```cpp
cbuffer DataHLSL : register(b0)
{
    float4 arrayData[5];
};

static float array[18] = (float[18])arrayData;
```

先在cbuffer中利用float4来接收数组信息，5个float4只占用80Byte即可接收所有的数据（还免了不少在CPU端给数组数据做padding的操作）。而后利用类型强制转换使其变成float[]，供我们在shader中使用，从而减少内存的浪费。

上诉方法对应的汇编语言中，会先使用[dcl_indexableTemp](https://link.zhihu.com/?target=https%3A//learn.microsoft.com/en-us/windows/win32/direct3dhlsl/dcl-indexabletemp)来声明一系列的临时寄存器，然后使用mov来将arrayData中的值赋值到这些临时寄存器中供后续的使用。因此虽然节省了内存但是却多了很多的汇编指令，具体使用应该视情况而定。

```cpp
dcl_indexableTemp x0[16], 4
mov x0[0].x, CB0[0][0].x
mov x0[1].x, CB0[0][0].y
.
.
.
mov x0[15].x, CB0[0][3].w
```



## packoffset

除了打包规则自动打包以外，我们还可以通过packoffset关键字来手动进行打包，语法如下：

> : packoffset( c[Subcomponent][.component] )

| Subcomponent | 寄存器下标       |
| ------------ | ---------------- |
| component    | 寄存器的四个分量 |

利用packoffset，我们可以指定当前成员从哪个寄存器的哪个分量开始存储，并且一旦某个成员使用了packoffset，那么其它所有成员都需要通过packoffset来指定位置。还需要注意的是手动指定时，**需要确保当前位置到寄存器末尾能够放下该成员**，例如我们不能将float4类型指定到寄存器的y、z、w这三个分量上。

示例代码如下：

```cpp
cbuffer DataHLSL : register(b0)
{
    bool val1 : packoffset(c1.y);
    bool val2 : packoffset(c0.w);
    int val3 : packoffset(c0.x);
    double val4 : packoffset(c1.z);
    float4 val5 : packoffset(c2);
};
```

对应汇编文件里的注释：

```text
// cbuffer DataHLSL
// {
//   bool val1;                         // Offset:   20 Size:     4 [unused]
//   bool val2;                         // Offset:   12 Size:     4 [unused]
//   int val3;                          // Offset:    0 Size:     4 [unused]
//   double val4;                       // Offset:   24 Size:     8 [unused]
//   float4 val5;                       // Offset:   32 Size:    16
// }
```

可以发现默认情况下，val1的Offset应该为0，但是通过packoffset(c1.y) 我们将其指定到了第二个寄存器的y分量上，因此此时的Offset变为了20。

因此我们也可以通过packoffset手动设置成员在内存中的位置使其和C++端相匹配，来避免两端值不对应的问题。