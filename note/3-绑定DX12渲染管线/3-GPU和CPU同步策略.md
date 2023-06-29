# GPU和CPU同步策略

## DirectX12 Fence （围栏）

DirectX 12中的Fence是一种用于同步CPU和GPU之间操作的机制。它允许CPU在等待GPU完成某些操作之前暂停，并且可以检查GPU是否已经完成了这些操作。

Fence本质上是一个计数器，每个命令队列都有一个对应的Fence对象。当CPU提交一个命令列表时，它会将Fence值增加1。而当GPU开始执行该命令列表时，它也会获取该Fence值，并将其存储到内部状态中。在GPU完成执行命令列表后，它将会更新Fence值。然后，CPU可以通过查询Fence值来检查GPU是否已经完成了相应的操作。

以下是使用Fence进行同步的示例代码：

```c++
// 创建Fence对象
ComPtr<ID3D12Fence> fence;
DX::ThrowIfFailed(m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));

// 命令列表执行完毕后设置Fence值
UINT64 fenceValue = m_fenceValue;
DX::ThrowIfFailed(m_commandQueue->Signal(fence.Get(), fenceValue));

// 等待GPU执行完毕
if (fence->GetCompletedValue() < fenceValue)
{
    HANDLE eventHandle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);
    DX::ThrowIfFailed(fence->SetEventOnCompletion(fenceValue, eventHandle));
    DWORD waitResult = WaitForSingleObject(eventHandle, INFINITE);
    CloseHandle(eventHandle);
}
```

在此示例中，我们首先创建了一个Fence对象。然后，在命令列表执行完毕后，我们向命令队列提交一个信号，以便通知GPU该命令列表已经执行完毕。接着，我们通过查询Fence对象的已完成值来检查GPU是否已经完成了该命令列表的执行。如果未完成，则通过调用Fence对象的SetEventOnCompletion函数等待GPU完成操作。

总之，Fence是DirectX 12中一种非常重要的机制，可用于同步CPU和GPU之间的操作，以及避免CPU等待GPU操作完成时的任务空转。但是，使用Fence需要注意，过多地等待Fence可能会降低性能并增加延迟。

### Fence计数机制

Fence是DirectX 12中一种用于同步CPU和GPU之间操作的机制，它的计数机制如下：

1. Fence对象有一个64位无符号整数值，称为Fence值。

2. 当CPU提交一个命令列表时，会将Fence值增加1，并将该值作为参数传递给命令队列的Signal函数。

3. 在GPU开始执行该命令列表时，它会获取该Fence值，并将其存储到内部状态中。

4. 在GPU完成执行命令列表后，它将会更新Fence值，并将其存储到内部状态中。

5. CPU可以通过查询Fence对象的已完成值来检查GPU是否已经完成了相应的操作。对于每个Fence对象，都可以使用GetCompletedValue函数查询其已完成值。

6. 如果GPU尚未完成相应的操作，则CPU可以通过调用Fence对象的SetEventOnCompletion函数等待GPU完成操作。SetEventOnCompletion函数将Fence对象的事件句柄与特定的Fence值相关联，并在该Fence值达到时发出信号。

7. CPU还可以通过轮询Fence对象的已完成值来检查GPU是否已经完成了相应的操作。如果已完成值小于预期值，则意味着GPU尚未完成相应的操作。在这种情况下，CPU可以继续轮询或等待一段时间后再次查询。

需要注意的是，使用Fence可能会降低性能并增加延迟。过多地等待Fence可能会导致CPU空转，并使得GPU没有足够的工作量来保持高效率。因此，在使用Fence时，需要根据具体情况找到合适的平衡点，以避免过度依赖Fence导致性能问题。



## CreateEventEx

CreateEventEx是Windows API中用于创建同步对象的函数之一。它可以创建一个事件（Event）对象，用于通知线程某个操作已经完成或某个状态已经发生变化。

CreateEventEx函数有多个参数，包括安全性、自动重置标志、初始状态、名称和属性等。其中最常用的参数如下：

1. lpEventAttributes：指向SECURITY_ATTRIBUTES结构体的指针，用于设置事件对象的安全属性。如果该参数为NULL，则表示使用默认安全性。

2. bManualReset：指定事件对象是否为手动重置。如果该值为TRUE，则表示事件对象被触发后需要手动重置才能返回非信号状态；如果该值为FALSE，则表示事件对象被触发后会自动重置为非信号状态。

3. bInitialState：指定事件对象的初始状态。如果该值为TRUE，则表示事件对象初始化为有信号状态；如果该值为FALSE，则表示事件对象初始化为无信号状态。

4. lpName：指向一个以NULL结尾的字符串，用于指定事件对象的名称。如果该值为NULL，则表示创建一个匿名事件对象。

以下是调用CreateEventEx函数创建事件对象的示例代码：

```c++
HANDLE hEvent = CreateEventEx(NULL, FALSE, FALSE, EVENT_ALL_ACCESS);
if (hEvent == NULL)
{
    // 处理错误
}
```

在此示例中，我们调用CreateEventEx函数创建了一个新的事件对象。由于没有指定安全属性、自动重置标志和初始状态等参数，因此使用了默认值。如果函数执行成功，则返回一个有效的事件句柄；否则返回NULL表示创建失败。

总之，CreateEventEx是Windows API中一种用于创建事件对象的函数，可用于同步线程之间的操作，以及通知线程某个操作已经完成或某个状态已经发生变化。



### 同步函数

```c++
void FWindowsEngine::WaitGPUCommandQueueComplete()
{
	CurrentFenceIndex++;

	// 向GPU设置新的隔离点 等待GPU处理完信号 （设置信号）
	ANALYSIS_RESULT(CommandQueue->Signal(Fence.Get(), CurrentFenceIndex));

	if (Fence->GetCompletedValue() < CurrentFenceIndex)
	{
		// 创建或者打开一个事件内核对象，并返回该内核对象的句柄
		// 参数1 SECURITY_ATTRIBUTES
		// 参数2 事件名，null表示匿名事件
		// 参数3
		// CREATE_EVENT_INITIAL_SET		0x00000002	表示我们的对象的初始化是否被触发了，FALSE表示未触发
		// CREATE_EVENT_MANUAL_RESET	0x00000001	表示这个事件对象必须要用ResetEvents重置，你不设置这个标志的话，我们的系统内核就会进行默认重置
		// 参数4 事件对象的访问权限
		HANDLE EventEX = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);

		// 我们在上面已经设置好信号了，那么在这里信号触发就会通知这个事件，下面这行代码达到的效果就是
		// 我们在上面设置的GPU信号，在GPU完成后，就会通知到事件EventX，从而让CPU知道GPU的命令执行情况
		// GPU完成后悔通知我们当前的事件句柄EventEX
		ANALYSIS_RESULT(Fence->SetEventOnCompletion(CurrentFenceIndex, EventEX));

		// 等待信号事件触发，等待时间为无限等待 （等待GPU）
		// 这里会阻塞主线程，防止CPU无限向GPU提交命令
		WaitForSingleObject(EventEX, INFINITE);

		// 如果事件完成了，会释放掉当前事件，那么我们就会在这里接收到这个事件，线程就会在这里被还原，被重新唤醒，然后我们就将当前事件关闭即可
		CloseHandle(EventEX);
	}
}
```

我们需要再所有CPU向GPU提交命令的地方都执行该函数