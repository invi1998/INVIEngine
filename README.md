# INVIEngine

## DirectX 12渲染流程

DirectX 12渲染流程是基于命令列表和命令队列的，并且相较于之前版本的DirectX，它的渲染流程更加灵活，可以更好地与多核CPU和GPU结合协同工作。下面是一般的渲染流程：

1. 初始化DX12设备和资源

在应用程序启动时，首先需要创建DX12设备对象，然后再根据需要创建交换链、渲染目标视图（RTV）、深度模板缓冲（DSV）等资源。

2. 创建命令队列和命令分配器

在初始化阶段，还需要创建一个或多个命令队列和命令分配器。命令队列用于存储渲染命令，而命令分配器则用于为每个帧分配一组可重用的命令列表。

3. 创建资源描述符堆

资源描述符堆（Descriptor Heap）是一种管理GPU资源的机制，包括纹理、缓冲、采样器等。在创建资源描述符堆时，需要指定相关资源的属性和数量。

4. 创建顶点缓冲区和索引缓冲区

在渲染场景之前，需要将几何体的顶点数据和索引数据上传到GPU端的顶点缓冲区和索引缓冲区中。

5. 创建着色器资源

着色器资源包括顶点着色器（Vertex Shader）、像素着色器（Pixel Shader）等。在渲染过程中需要使用这些着色器资源来对顶点和像素进行处理。

6. 设置渲染管线状态

在开始绘制场景之前，需要设置渲染管线的状态，包括顶点缓冲区、索引缓冲区、顶点着色器、像素着色器、光栅化器（Rasterizer）等。

7. 开始记录命令列表

通过命令列表来记录渲染命令，每个命令列表都必须与一个命令分配器关联。可以使用命令列表来执行一系列渲染操作，如绘制三角形、清除颜色缓冲、设置视口等。

8. 执行命令列表

在命令列表录制完成后，需要将其提交给命令队列，并在GPU上执行。如果有多个命令列表，则需要按照正确的顺序提交，以确保渲染操作按照预期顺序执行。

9. 呈现结果

当所有渲染操作完成后，需要将渲染结果呈现到屏幕上。通常，这需要使用交换链中的后台缓冲区，将渲染结果从当前帧呈现到屏幕上，并将交换链的后台缓冲区切换为下一帧的前置缓冲区。

以上是一个典型的DX12渲染流程，不同的应用程序可能会有所差异，但整个流程的大致框架都是相似的。



## Vulkan渲染流程

Vulkan渲染流程是通过使用命令缓冲区（Command Buffer）和命令池（Command Pool）来实现的。与DirectX 12相似，Vulkan也更加灵活，可以更好地利用多核处理器。

下面是一般的Vulkan渲染流程：

1. 初始化Vulkan设备和资源

在应用程序启动时，需要创建Vulkan设备对象，并根据需要创建交换链、渲染目标视图、深度模板缓冲等资源。

2. 创建命令池和命令缓冲区

在初始化阶段，还需要创建一个或多个命令池和命令缓冲区。命令池用于存储命令缓冲区，而命令缓冲区则用于记录渲染命令。

3. 创建顶点缓冲区和索引缓冲区

在渲染场景之前，需要将几何体的顶点数据和索引数据上传到GPU端的顶点缓冲区和索引缓冲区中。

4. 创建着色器资源

着色器资源与Vulkan中的管线状态绑定，包括顶点着色器、片段着色器等。

5. 设置管线状态

在开始绘制场景之前，需要设置管线状态。这包括顶点输入、光栅化器（Rasterizer）、混合器（Blender）等。与DirectX 12不同的是，在Vulkan中需要显式指定每个状态信息。

6. 准备渲染

在开始进行渲染操作之前，需要执行一些准备工作，包括为每个帧创建一个命令缓冲区，并将它关联到对应的交换链图像上。

7. 开始记录命令

通过命令缓冲区来记录渲染命令，每个命令缓冲区都必须与一个命令池关联。可以使用命令缓冲区来执行一系列渲染操作，如绘制三角形、清除颜色缓冲等。

8. 执行渲染命令

在命令缓冲区录制完成后，需要提交给队列，并在GPU上执行。如果有多个命令缓冲区，则需要按照正确的顺序提交，以确保渲染操作按照预期顺序执行。

9. 呈现结果

当所有渲染操作完成后，需要将渲染结果呈现到屏幕上。通常，这需要使用交换链中的后台缓冲区，将渲染结果从当前帧呈现到屏幕上，并将交换链的后台缓冲区切换为下一帧的前置缓冲区。

以上是一个典型的Vulkan渲染流程，不同的应用程序可能会有所差异，但整个流程的大致框架都是相似的。



## Metal 渲染流程

Metal 渲染流程是基于命令缓冲区和命令编码器的，并且与其他图形API一样，也是在GPU上执行的。下面是一般的Metal渲染流程：

1. 初始化 Metal 设备和资源

在应用程序启动时，需要创建 Metal 设备对象，并根据需要创建纹理、渲染目标视图、深度模板缓冲等资源。

2. 创建命令队列和命令编码器

在初始化阶段，还需要创建一个或多个命令队列和命令编码器。命令队列用于存储渲染命令，而命令编码器则用于为每个帧分配一组可重用的命令列表。

3. 创建顶点缓冲区和索引缓冲区

在渲染场景之前，需要将几何体的顶点数据和索引数据上传到GPU端的顶点缓冲区和索引缓冲区中。

4. 创建着色器资源

着色器资源包括顶点着色器（Vertex Shader）、片段着色器（Fragment Shader）等。在渲染过程中需要使用这些着色器资源来对顶点和像素进行处理。

5. 设置渲染管线状态

在开始绘制场景之前，需要设置渲染管线的状态，包括顶点缓冲区、索引缓冲区、顶点着色器、像素着色器、光栅化器（Rasterizer）等。

6. 开始记录命令列表

通过命令编码器来记录渲染命令，每个命令编码器都必须与一个命令队列关联。可以使用命令编码器来执行一系列渲染操作，如绘制三角形、清除颜色缓冲、设置视口等。

7. 执行命令列表

在命令列表录制完成后，需要将其提交给命令队列，并在GPU上执行。如果有多个命令列表，则需要按照正确的顺序提交，以确保渲染操作按照预期顺序执行。

8. 呈现结果

当所有渲染操作完成后，需要将渲染结果呈现到屏幕上。通常，这需要使用交换链中的后台缓冲区，将渲染结果从当前帧呈现到屏幕上，并将交换链的后台缓冲区切换为下一帧的前置缓冲区。

以上是一个典型的Metal渲染流程，不同的应用程序可能会有所差异，但整个流程的大致框架都是相似的。



## OpenGL 渲染流程

OpenGL 渲染流程分为初始化阶段和渲染阶段。

1. 初始化阶段

在 OpenGL 的初始化阶段，需要设置窗口大小、视角、颜色及其他一些 OpenGL 与 GPU 交互的参数。主要步骤包括：

* 创建 OpenGL 窗口
* 初始化 OpenGL 上下文
* 创建场景对象、实体和材质等数据
* 创建顶点缓冲区对象和索引缓冲区对象并上传至 GPU
* 创建着色器程序（包括顶点着色器和片元着色器等），进行编译和链接
* 设置纹理贴图、帧缓冲区等可选项
* 将所有设置上传到 GPU 中

2. 渲染阶段

在 OpenGL 渲染过程中，每次迭代都会重复执行以下几个步骤：

* 清空屏幕缓冲区
* 绑定相应的顶点缓冲区对象和索引缓冲区对象
* 绑定相应的着色器程序
* 给着色器传递变量值（如 MVP 变换矩阵）
* 发送绘制命令
* 交换前后缓冲区内容

其中，顶点缓冲区对象和索引缓冲区对象是存储定义场景的几何信息的关键组件，它们被上传至 GPU 中，并通过着色器程序来转换为具体的像素颜色。

在每次渲染过程中，OpenGL 都会将当前状态保存在状态机中，下次渲染时会基于上一次的状态来进行渲染。因此，我们可以通过调整状态机中的参数来控制 OpenGL 的渲染结果。

最后，当所有的渲染操作完成后，还需要使用交换前后缓冲区的方法将渲染结果呈现在屏幕上，以供用户观察。

总的来说，OpenGL渲染流程比较简单，但是也需要掌握一些基本概念和 API 调用方法方能顺利进行。



## DX12初始化到底初始化了啥

DirectX 12 的初始化过程是相对复杂的，需要对硬件设备进行查询、创建资源、启动线程等多个环节。下面简单介绍一下 DirectX 12 初始化期间创建了哪些对象：

1. D3D12Device

D3D12Device 是 DirectX 12 的核心设备对象，它代表了与 GPU 的一个会话。在初始化过程中，需要创建该对象并根据硬件设备的特性选择合适的驱动版本。

2. 命令队列

命令队列是向 GPU 发送指令和任务的主要通道，可以有多个命令队列，每个队列用于提交不同类型的任务，如渲染命令、计算命令等。

3. 命令分配器

命令分配器用于从 CPU 向 GPU 分配内存资源，并将命令序列化，以便在 GPU 上执行。每个命令分配器都必须与命令队列关联。

4. 命令列表

命令列表用于记录 GPU 执行的命令和操作，包括设置管线状态、绑定顶点缓冲区、绘制三角形、上传纹理等操作。

5. 交换链

交换链用于处理屏幕刷新和前后缓冲区的交换，它是通过 D3D12Device 和 DXGI（DirectX Graphics Infrastructure）来实现的。在初始化阶段，需要创建交换链，以确定帧缓冲区的数量和格式，并确定呈现模式和显示器刷新率等参数。

6. 渲染目标视图（RTV）和深度模板缓冲视图（DSV）

RTV 是表示帧缓冲区颜色输出的对象，而 DSV 则表示深度模板缓冲区的视图。在初始化阶段，需要创建这两个视图，并与交换链相关联。

7. 顶点缓冲区和索引缓冲区

顶点缓冲区用于存储顶点数据，而索引缓冲区则用于存储渲染物体的索引数据。在初始化阶段，需要创建这两个缓冲区，并上传至 GPU 端。

8. 着色器

着色器用于指定管线的渲染方式，包括顶点着色器、像素着色器、几何着色器和计算着色器等。在初始化阶段，需要编译和链接相应的着色器代码，并创建着色器对象。

9. 根签名

根签名是用于向 GPU 传递常量和资源地址的对象，它是由着色器程序定义的。在初始化阶段，需要创建根签名，并将其与命令列表关联。

总之，DirectX 12 初始化阶段创建了许多对象，这些对象相互关联，构成了整个渲染管线。需要注意的是，在实际应用中，可能还需要根据具体需求创建其他对象，如纹理、常量缓冲区、采样器等。



## DX12命令录制功能。

DirectX 12中，GPU执行的命令需要使用命令列表（Command List）进行录制，并通过命令队列（Command Queue）提交。命令列表包含的指令覆盖了图形管线中的每个阶段，包括顶点输入、光栅化、像素处理等。

1. 创建命令列表

在开始命令录制之前，需要创建一个命令列表对象，并为其分配一个命令分配器（Command Allocator）。命令列表是与 GPU 关联的工作流程，它记录着多个命令序列，这些命令序列最终在 GPU 上执行。命令分配器用于分配内存，以供 GPU 对命令序列进行记录。

2. 设置渲染目标和视口

在命令列表中可以设置渲染目标视图和深度模板缓冲视图，以及视口和裁剪矩形等参数。这些参数主要决定了渲染操作的输出结果和范围。

3. 设置管线状态

设置管线状态是命令列表中非常重要的一步，包括绑定渲染目标视图、深度模板缓冲视图、着色器程序、根签名等参数。这些状态既可以在命令列表中指定，也可以在命令分配器上进行预定义。

4. 设置顶点缓冲区输入

命令列表还需要指定顶点缓冲区对象的属性，如每个属性的数据类型、偏移量等参数。这些信息将被用来定义输入布局，以便 GPU 在后续操作中正确地访问顶点数据。

5. 描述图元拓扑

图元拓扑是指存储在顶点缓冲区中的几何图元的类型，如点、线、三角形等。在命令列表中需要指定图元拓扑，以便 GPU 在绘制时能够正确地解析顶点缓冲区中的数据。

6. 绘制图元

在命令列表中，可以通过调用 ID3D12GraphicsCommandList::DrawInstanced 函数来绘制图元，该函数需要指定要绘制的顶点数量、实例数量和起始索引号等参数。

7. 绑定索引缓冲区

如果需要绘制具有复杂形状的物体，可以使用索引缓冲区来优化渲染性能。在命令列表中，可以通过调用 ID3D12GraphicsCommandList::IASetIndexBuffer 函数来绑定索引缓冲区，该函数需要指定缓冲区的地址、格式和偏移量等参数。

8. 绘制索引缓冲区图元

在命令列表中，可以通过调用 ID3D12GraphicsCommandList::DrawIndexedInstanced 函数来绘制索引缓冲区图元，该函数需要指定要绘制的索引数目、实例数量和起始索引号等参数。

9. 其他命令

除了上述命令外，命令列表还支持其他一些命令，如资源转换、资源屏障、清空颜色缓冲区和深度模板缓冲区、设置常量缓冲区、绑定采样器等。这些命令可以根据实际需求进行添加。

10. 完成命令记录

在命令列表完成记录后，需要通过调用 ID3D12GraphicsCommandList::Close 函数来结束命令录制。此时，命令列表已经准备好提交给命令队列进行执行。

通过以上步骤，就可以在 DX12 中使用命令列表



## 深入理解CPU和GPU信号同步策略

在现代图形编程中，CPU 和 GPU 通常是独立的处理器，并且在并行处理时需要进行同步。信号同步是实现 CPU 和 GPU 之间数据交互的重要机制，它能确保各个线程之间的执行顺序和数据一致性。

1. 等待信号

等待信号是 GPU 在执行完某个任务后，等待 CPU 向其发送信号的一种方式。在 DirectX 12 中，可以使用 ID3D12CommandQueue::Signal 函数向 GPU 发送信号，并通过 ID3D12Fence::SetEventOnCompletion 函数在信号完成时通知 CPU。在 CPU 端，可以调用 WaitForSingleObject 或 WaitForMultipleObjects 函数来等待信号完成。

2. CPU 信号

CPU 信号是指在 CPU 执行完某个任务后向 GPU 发送的信号。在 DirectX 12 中，可以使用 ID3D12Fence::Signal 函数将信号值写入信号对象（Fence Object）。GPU 在执行过程中会读取这些信号对象，以确定何时开始或结束执行某个任务。

3. 并发栅栏

并发栅栏（Concurrency Barrier）是一种在 GPU 中等待当前所有工作项完成后再执行下一个阶段的机制。它允许 CPU 和 GPU 在处理大量任务时进行协调，以便 GPU 不会访问尚未由 CPU 更新的数据。在 DirectX 12 中，可以使用 ID3D12CommandList::ResourceBarrier 函数添加并发栅栏。这个函数可以设置资源状态转换，例如从读取状态到写入状态，以确保 GPU 访问资源时不会出现竞态条件。

4. 分离式采样器（Disjoint Sampler）

分离式采样器也是一种 CPU 和 GPU 之间同步的机制。该技术可确保查询计算涉及的对象具有一致的状态。在 DirectX 12 中，可以创建 ID3D12QueryHeap 对象，并使用 ID3D12GraphicsCommandList::BeginQuery 和 ID3D12GraphicsCommandList::EndQuery 函数将时间戳插入查询堆。CPU 可以读取时间戳来检查 GPU 执行和渲染的时间，并确保任务按预期顺序执行。

5. 内存屏障

内存屏障是确保 CPU 和 GPU 访问相同内存区域时数据一致性的一种机制。在 DirectX 12 中，可以使用 ID3D12GraphicsCommandList::ResourceBarrier 来设置内存屏障。内存屏障可以定义读取、写入和同步操作，以确保 CPU 和 GPU 之间进行正确的内存访问。

总之，信号同步是确保 CPU 和 GPU 数据交互的关键机制。通过以上几种方法，CPU 和 GPU 可以有效地协调和同步，以实现高效而稳定的图形渲染和计算。



## DX12流程中cpu和gpu包含的数据

在 DirectX 12 中，CPU 和 GPU 在图形渲染过程中分别包含以下数据：

1. CPU 数据

（1）顶点和索引缓冲区数据：包含了定义几何模型的顶点数据和索引数据。

（2）常量缓冲区数据：包含了需要在 GPU 中使用的常量变量值，如世界矩阵、视图矩阵、投影矩阵等。

（3）纹理数据：包含了用于渲染的纹理贴图数据，如漫反射贴图、法线贴图等。

（4）命令列表数据：包含了一组需要提交给 GPU 执行的命令序列。

（5）Fence 对象数据：用于 CPU 和 GPU 之间同步信号的机制。CPU 可以向 Fence 对象写入信号，GPU 能够读取这些信号，并在其完成后通知 CPU。

2. GPU 数据

（1）图形管线状态：包括着色器程序、根签名、渲染目标视图、深度模板缓冲视图等配置信息。

（2）绘制操作数据：包括顶点数据、索引数据、图元拓扑等信息。

（3）常量缓冲区数据：由 CPU 写入，GPU 可以按需读取。

（4）采样器状态数据：包括纹理过滤、着色模式和边界条件等信息。

（5）纹理贴图数据：包括漫反射贴图、法线贴图等。

总之，在 DX12 中，CPU 和 GPU 分别包含了各自需要处理的数据。通过共享这些数据来协调两个处理器的工作，以实现高效而稳定的图形渲染和计算。



## SNC和RPC

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



# GDI

GDI是Windows图形设备接口（Graphics Device Interface）的缩写，是Microsoft Windows操作系统中用于绘制平面图形和文本等的底层图形API。GDI提供了一系列函数和服务，使得开发人员可以在Windows应用程序中轻松绘制2D图形、文字和图像等。

GDI支持多种输出设备和显示器，包括打印机、显示器、文件和内存等。它还提供了丰富的绘图对象和属性，如线条、矩形、椭圆、笔刷、字体和颜色等，以满足各种绘图需求。同时，GDI还提供了一些高级功能，如透明度、旋转、位图混合和蒙板等，可用于创建复杂的图形效果和动画。

尽管现代的Windows应用程序通常使用更高级的图形API，如Direct2D、WPF和OpenGL等，但GDI仍然具有广泛的应用和重要性。特别是在老旧的Windows桌面应用程序中，如Office、Photoshop和WinZip等，GDI仍然是必不可少的API之一。



# HLSL寄存器

在HLSL中，寄存器的编号通常由两部分组成：资源类型和寄存器号。不同的资源类型有不同的寄存器编号范围，而每个寄存器编号则用于唯一标识一个特定的资源。

以下是常见的几种资源类型及其寄存器编号的范围：

- 静态纹理（Texture2D）：t0 ~ t255
- 动态纹理（Texture2DArray）：d0 ~ d15
- 立方体贴图（TextureCube）：t0 ~ t15
- 缓冲区（Buffer）：b0 ~ b15
- 常量缓冲区（ConstantBuffer）：cb0 ~ cb15
- 采样器（SamplerState）：s0 ~ s15

需要注意的是，某些资源类型的寄存器编号范围可能会根据驱动程序、硬件平台或者API版本等因素而略有不同，具体应以当前环境下的文档或示例代码为准。

在实际使用中，可以在shader中声明出需要使用的资源类型和寄存器号，并在渲染管线中进行绑定。例如，在HLSL中声明一个常量缓冲区并将其绑定到寄存器cb1上，可以像下面这样写：

```c++
cbuffer MyConstants : register(b1)
{
    float4x4 g_WorldViewProjectionMatrix;
    float3   g_LightDirection;
    float    g_Time;
};
```

在上述代码中，cbuffer表示声明一个常量缓冲区，MyConstants是缓冲区的名称，register(b1)则指明了该缓冲区应该绑定到寄存器b1上。最后，我们定义了三个变量作为缓冲区的内容，包括一个4x4矩阵、一个3元向量和一个标量。这样，在渲染管线中使用该缓冲区时，只需要将其绑定到管线上下文的对应寄存器即可。



一、图形API使用方式
无论具体 API 如何设计，图形应用程序大都遵循以下执行顺序：

初始化 API - 创建访问 API 内部工作原理所需的核心数据结构。
加载资产 - 创建加载着色器等内容所需的数据结构，图形管道，创建和填充要执行的命令缓冲区，以及将资源发送到 GPU 。
更新资源 - 将任何制服更新到着色器，并在此处执行应用程序级逻辑。
显示 - 将命令缓冲区列表发送到命令队列并显示到交换链。
重复 2、3 和 4，直到应用程序发出关闭信号。
销毁 - 等待 GPU 完成渲染工作，并销毁所有数据结构和句柄。
所有图形API的使用都是遵循此顺序执行图形 API 数据结构的创建和使用。

二、依赖项（Importing Dependencies）
API	Structure
Vulkan	#include <vulkan/vulkan.hpp>
DirectX 12	#include <d3d12.h>
DirectX 11	#include <d3d11.h>
Metal	#import <Metal/Metal.h>
WebGPU	Canary版本浏览器
OpenGL	视操作系统
启动应用程序时，需要包含对外部 API 的依赖项，图形 API 也不例外。根据 API 的不同，您可能还需要项目中的其他库，如着色器编译器等。

OpenGL是所有其他图形API的例外，因为根据操作系统和您的个人设置，可以从不同位置进行各种导入。

2.1 着色器编译器（Shader Compiler）
API	Structure
Vulkan	#include “glslang/Include/revision.h”
DirectX 12	<D3Dcompiler.h>
DirectX 11	<D3Dcompiler.h>
Metal	#import <Metal/Metal.h>
WebGPU	无
OpenGL	void glShaderSource(…)
Vulkan 需要你使用外部着色器编译器（例如 glslang 或 DirectX Shader Compiler）生成 SPIR-V 。

DirectX建议你使用 DirectX Shader Compiler 而不是头文件中包含的着色器编译器，因为DirectX Shader Compiler 支持较新的着色器版本以及可以优化和提升编译速度。

Metal着色器可以在运行时编译，也可以在构建时使用 MacOS 路径中包含的命令行工具进行编译。

OpenGL不需要外部库来编译着色器，因为它包含在库中，在OpenGL 4.6及之后版本中GLSL也可以使用SPIR-V替代。

WebGPU 着色器是纯文本字符串，因此无需编译它们，。

着色器的具体区分与使用我们之后详细展开。

三、图形初始化（Initialize API）
3.1 图形入口（Entry Point）
API	Structure
Vulkan	vk::Instance
DirectX 12	IDXGIFactory4
DirectX 11	IDXGIFactory
Metal	CAMetalLayer
WebGPU	GPU
OpenGL	视操作系统
图形 API 的入口（感觉叫实例对象更贴切）通常允许您访问 API 的内部类。

Vulkan 的入口包括选择您准备使用的 API 版本以及您想要开启的任何扩展或层，例如错误检查、窗口表面等。

DirectX 11 和 12 要求您创建工厂和Debug数据结构（可选）。

在 Metal 上，一旦CAMetalLayer 层存在并附加到窗口（NSWindow、NSView、CAMetalLayer），该窗口就可以使用 Metal API 的其余部分。

对于 OpenGL，最接近入口的是操作系统特定的上下文，您可以在创建操作系统窗口后请求该上下文。

3.2 物理设备（Physical Device）
物理设备图

API	Structure
Vulkan	vk::PhysicalDevice
DirectX 12	vk::PhysicalDevice
DirectX 11	IDXGIAdapter
Metal	MTLDevice
WebGPU	GPUAdapter
OpenGL	glGetString(GL_VENDOR)
物理设备允许您查设备（显卡）的特定详细信息，例如内存大小和功能支持版本。

Metal 的物理设备和逻辑设备都由相同的数据结构共享。

OpenGL 无法查询任何设备详细信息，除非您使用制造商专用扩展。您可以获取一些杂项数据，例如驱动程序供应商名称，渲染器和OpenGL版本。

3.3 逻辑设备（Logical Device）
图

API	Structure
Vulkan	vk::Device
DirectX 12	ID3D12Device
DirectX 11	ID3D11Device
Metal	MTLDevice
WebGPU	GPUDevice
OpenGL	无
设备允许您访问 API 的核心内部功能，例如创建图形数据结构，如纹理、缓冲区、队列、管道等。这种类型的数据结构在所有现代图形 API 中大部分都是相同的，很少会变更。

Vulkan 和 DirectX 12 通过设备创建内存数据结构来提供对内存的控制。

3.4 队列（Queue）
队列图

API	Structure
Vulkan	vk::Queue
DirectX 12	ID3D12CommandQueue
DirectX 11	ID3D11DeviceContext
Metal	MTLCommandQueue
WebGPU	GPUQueue
OpenGL	无
队列允许您将任务排队以供 GPU 执行。GPU 是一种异步计算设备，因此显示最优解便是始终保持GPU忙碌，同时异步控将命令添加到队列中。

Vulkan 队列要求您在创建设备之前指定设备将使用的队列（绘制、计算、拷贝）。

3.5 命令池（Command Pool）
命令池

API	Structure
Vulkan	vk::CommandPool
DirectX 12	ID3D12CommandAllocator
DirectX 11	ID3D11DeviceContext
Metal	MTLCommandQueue
WebGPU	GPUDevice
OpenGL	无
命令池是一种数据结构，可用于创建命令缓冲区。

Metal 的命令池设计的比其他API好一些，因为MTLCommandQueue队列中也能分配命令缓冲区的数据结构。

四、帧缓冲（Frame）
4.1 窗口界面（Window Surface）
API	Structure
Vulkan	vk::Surface
DirectX 12	ID3D12Resource
DirectX 11	ID3D11Texture2D
Metal	CAMetalLayer
WebGPU	GPUCanvasContext
OpenGL	视操作系统
Window Surface 允许您将所有绘制调用绑定到特定于操作系统的窗口。

在 DirectX 上，由于只有 Windows / Xbox 作为 API 的目标，窗口将从交换链中接收后台缓冲区中的纹理。交换链中接收窗口句柄，并从那里创建 DirectX 驱动程序内部的图面。

由于 MacOS 和 iOS 窗口具有分层结构，其中应用程序包含一个视图，该视图可以包含一个图层，因此 Metal 可以在该图层中显示。

4.2 交换链（Swapchain）
交换链图

API	Structure
Vulkan	vk::Swapchain
DirectX 12	IDXGISwapChain3
DirectX 11	IDXGISwapChain
Metal	CAMetalDrawable
WebGPU	GPUCanvasContext
OpenGL	视操作系统
交换链在给定窗口的不同后台缓冲区之间翻转替换显示，并控制呈现的各操作，例如刷新率和后台缓冲区的交换行为。

Metal 和 OpenGL API 中没有交换链的概念，而是将其留给操作系统窗口 API。

4.3 帧缓冲区（Frame Buffers）
帧缓冲图

API	Structure
Vulkan	vk::Framebuffer
DirectX 12	ID3D12Resource
DirectX 11	ID3D11RenderTargetView
Metal	MTLRenderPassDescriptor
WebGPU	GPURenderPassDescriptor
OpenGL	GLuint
帧缓冲区是基于光栅化图形管线执行期间用作输出的输出纹理组。

DirectX 12 和 11 没有为此提供显式数据结构，而是可以传递一组视图（RTV）。

五、初始化资源（Initialize Resources）
5.1 纹理（Texture）
API	Structure
Vulkan	vk::Image & vk::ImageView
DirectX 12	ID3D12Resource
DirectX 11	ID3D11Texture2D
Metal	MTLTexture
WebGPU	GPUTexture & GPUTextureView
OpenGL	GLuint
纹理是存储颜色信息的数据数组，并用作渲染的输入/输出。Vulkan、DirectX 12 和 WebGPU 引入了具有给定纹理的多个视图的思路，这些视图可以以不同的编码格式或颜色空间查看该纹理。Vulkan 引入了图像和缓冲区托管内存的概念，因此纹理是图像的三元组，在使用时的图像视图（可以有多个），以及仅设备或 CPU-GPU 可访问空间中的内存。

对于在 Vulkan和DX12 中内存管理可以看一下AMD开发者的博客：AMD Vulkan Memory Allocator & AMD D3D12 Memory Allocator

5.2 缓冲区（Buffer）
缓冲图

API	Structure
Vulkan	vk::Buffer & vk::BufferView
DirectX 12	ID3D12Resource
DirectX 11	ID3D11Buffer
Metal	MTLBuffer
WebGPU	GPUBuffer & GPUBufferView
OpenGL	GLuint
缓冲区是一个数据数组，例如网格的位置数据、颜色数据、索引数据等。类似的图像规则适用于 Vulkan 和 WebGPU 中的缓冲区。

5.3 着色器（Shader）
API	Structure
Vulkan	vk::ShaderModule
DirectX 12	ID3DBlob
DirectX 11	ID3D11VertexShader or ID3D11PixelShader…
Metal	MTLLibrary
WebGPU	GPUShaderModule
OpenGL	GLuint
着色器往往是已编译的着色器代码的句柄（HLSL、GLSL、MSL 等），这些代码要送到给定的渲染管线。

5.4 着色器绑定（Shader Bindings）
命令缓冲区

API	Structure
Vulkan	vk::PipelineLayout & vk::DescriptorSet
DirectX 12	ID3D12RootSignature
DirectX 11	ID3D11DeviceContext::VSSetConstantBuffers(…)
Metal	[MTLRenderCommandEncoder setVertexBuffer: uniformBuffer]
WebGPU	GPUPipelineLayout
OpenGL	GLint
大多现代图形 API 都具有绑定数据结构，可以将统一的缓冲区和纹理连接到需要该数据的图形管道（性能更优）。Metal 的独特之处在于您可以在命令编码器中绑定常量缓冲，与 Vulkan、DirectX 12 和 WebGPU 相比，架构变得更加容易。

5.5 管线（Pipeline）
API	Structure
Vulkan	vk::Pipeline
DirectX 12	ID3D12PipelineState
DirectX 11	各种状态调用
Metal	MTLRenderPipelineState
WebGPU	GPURenderPipeline
OpenGL	各种状态调用
管线是对执行光栅化绘制调用、计算调度或光线追踪调度时将执行的内容的总体描述。

DirectX 11 和 OpenGL 它们没有图形管道的专用对象，而是使用调用在执行绘制调用之间设置管道状态。

5.6 命令缓冲区（Command Buffer）
命令缓冲区

API	Structure
Vulkan	vk::CommandBuffer
DirectX 12	ID3D12GraphicsCommandList
DirectX 11	ID3D11DeviceContext、
Metal	MTLRenderCommandEncoder
WebGPU	GPUCommandEncoder
OpenGL	GL_NV_command_list
命令缓冲区是一个异步计算单元，您可以在其中描述 GPU 要执行的过程，例如绘制调用、将数据从 CPU-GPU 可访问内存（共享内存）复制到 GPU 独占内存，以及动态设置图形管线的各个阶段。

GPU 本质上是异步的，由驱动程序负责确定何时将任务调度到 GPU。

5.7 命令列表（Command List）
命令列表图

API	Structure
Vulkan	vk::SubmitInfo
DirectX 12	ID3D12CommandList[]
DirectX 11	ID3D11CommandList
Metal	MTLCommandBuffer
WebGPU	GPUCommandEncoder[]
OpenGL	GL_NV_command_list
命令列表是批量推送到 GPU 的命令缓冲区组。这样让让 GPU 持续工作，从而减少 CPU 和 GPU 之间的同步。

5.8 栅栏（Fence）
API	Structure
Vulkan	vk::Fence
DirectX 12	ID3D12Fence
DirectX 11	ID3D11Fence
Metal	MTLFence
WebGPU	无
OpenGL	glFenceSync
栅栏是用于同步 CPU 和 GPU 的对象。可以指示CPU和GPU在围栏处等待，以便对方可以赶上。可用于管理资源分配和解除分配，从而更轻松地管理整体图形内存使用情况。

5.9 屏障（Barrier）
API	Structure
Vulkan	vkCmdPipelineBarrier
DirectX 12	D3D12_RESOURCE_BARRIER
DirectX 11	无
Metal	MTLFence
WebGPU	无
OpenGL	glMemoryBarrier
屏障是命令缓冲区内更精细的同步形式。具体可以参照知乎上介绍同步相关的文章。

5.10 信号量（Semaphore）
API	Structure
Vulkan	vk::Semaphore
DirectX 12	HANDLE
DirectX 11	HANDLE
Metal	HANDLE
WebGPU	无
OpenGL	根据操作系统
信号量是用于引入操作之间的依赖关系的对象，例如在将命令缓冲区提交到设备队列之前在获取交换链中的下一个图像之前等待。

Vulkan的独特之处在于信号量是API的一部分，DirectX和Metal将其委托给操作系统调用。

其实还有Vulkan 中的 Event 、SubPass 的隐式同步，其中Event可以看成分成前后两端的 Pipeline Barrier，控制粒度更细，通过 vkCmdSetEvent 指定 Source Stage，然后通过 vkCmdWaitEvent 指定 Destinition Stage，只等待同步的事件发生，而不需要停滞 GPU Pipeline，使 GPU 的并行性更强。各种同步的作用和作用域各不相同，大家有兴趣的可以自行查阅相关资料（所以说VK熟悉了，其他的都是大同小异）。

六、空间/对齐（Spaces, Alignments）
每个图形 API 对坐标系、NDC 坐标方向/范围、矩阵对齐、纹理对齐等具有不同的默认值，在大多数情况下，只需在片段着色器中翻转 UV中的值即可。

6.1 纹理对齐方式
API	Structure
Vulkan	Bottom Left
DirectX 12	Top Left
DirectX 11	Top Left
Metal	Top Left
WebGPU	Bottom Left
OpenGL	Bottom Left
DirectX 使用左上角作为像素空间坐标
