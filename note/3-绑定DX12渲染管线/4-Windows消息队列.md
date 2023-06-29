# Windows消息队列

Windows消息队列是Windows系统中一种用于处理窗口消息的机制。每个线程都拥有自己的消息队列，其中存储着该线程需要处理的所有消息。

每个消息都是一个具有特定格式的结构体，称为消息结构（MSG）。消息结构包括以下字段：

- HWND hwnd：指定接收该消息的窗口句柄。
- UINT message：指定要发送的消息类型。
- WPARAM wParam：包含与消息相关的附加信息。
- LPARAM lParam：包含与消息相关的其他附加信息。
- DWORD time：指定消息被发送的时间。
- POINT pt：指定鼠标光标的屏幕坐标位置。

当Windows操作系统向应用程序发送消息时，它将该消息插入到应用程序的消息队列中，并等待应用程序处理该消息。应用程序可以通过调用GetMessage或PeekMessage函数从消息队列中读取消息，并根据消息类型执行相应的操作。

以下是使用Windows消息队列处理窗口消息的示例代码：

```c++
MSG msg = {0};
while (GetMessage(&msg, NULL, 0, 0))
{
    TranslateMessage(&msg);
    DispatchMessage(&msg);
}
```

在此示例中，我们使用GetMessage函数从当前线程的消息队列中获取下一个消息。如果有消息，则TranslateMessage函数将其转换为键盘或鼠标输入事件，并将其发送到对应的窗口。然后，DispatchMessage函数将该消息分派给正确的窗口过程函数进行处理。

总之，Windows消息队列是一种重要的Windows系统机制，用于处理窗口消息并实现基本的用户界面交互功能。了解Windows消息队列的工作原理和使用方法对于开发Windows应用程序非常重要。



## PeekMessage

PeekMessage是Windows API中用于从线程的消息队列中检索和移除消息的函数之一。与GetMessage函数不同，PeekMessage函数不会将应用程序挂起等待消息的到来，而是立即返回并允许应用程序继续执行其他操作。

PeekMessage函数有多个参数，包括消息过滤标志、窗口句柄、最小和最大消息值、移除标志和是否阻塞等。其中最常用的参数如下：

1. lpMsg：指向一个MSG结构体的指针，用于接收检索到的消息。

2. hWnd：指定要处理消息的窗口句柄。如果该参数为NULL，则表示检索所有消息。

3. wMsgFilterMin和wMsgFilterMax：分别指定消息过滤的最小和最大值。如果这两个参数都为0，则表示不进行任何过滤。

4. wRemoveMsg：指定是否将检索到的消息从消息队列中移除。如果该值为PM_REMOVE，则表示要移除消息；如果该值为PM_NOREMOVE，则表示仅检索消息而不移除。

5. wFlags：指定是否在没有可用消息时阻塞调用线程。如果该值为PM_NOREMOVE | PM_NOYIELD，则表示不阻塞，并且不将任何消息从队列中移除或添加。

以下是调用PeekMessage函数从消息队列中检索和移除消息的示例代码：

```c++
MSG msg = {0};
while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
{
    TranslateMessage(&msg);
    DispatchMessage(&msg);
}
```

在此示例中，我们使用PeekMessage函数从当前线程的消息队列中获取下一个消息。如果有消息，则TranslateMessage函数将其转换为键盘或鼠标输入事件，并将其发送到对应的窗口。然后，DispatchMessage函数将该消息分派给正确的窗口过程函数进行处理。

需要注意的是，在使用PeekMessage函数时，需要注意避免死循环或过度消耗CPU资源的问题，因为该函数不会挂起线程等待新消息的到来。因此，在实际编程中，通常需要结合其他机制（例如计时器或事件对象）来控制消息处理的频率和时间间隔，以确保程序的正常运行和响应性能。

#### 消息类型

它和其他标志一起组成了wFlags参数，常见的标志包括：

- PM_NOREMOVE：仅查找消息，不将其从队列中移除。
- PM_REMOVE：查找消息并将其从队列中移除。
- PM_NOYIELD：在没有消息时，不让线程进入等待状态。
- PM_QS_SENDMESSAGE：查找等待通过SendMessage或PostMessage发送的消息。
- PM_QS_PAINT：查找WM_PAINT消息。
- PM_QS_POSTMESSAGE：查找等待通过PostMessage发送的消息。
- PM_QS_HOTKEY：查找WM_HOTKEY消息。
- PM_QS_ALLPOSTMESSAGE：查找所有等待通过PostMessage发送的消息。
- PM_QS_INPUT：查找输入消息（鼠标和键盘输入）。

通过组合使用上述标志，可以选择要检索的消息类型以及PeekMessage函数的行为方式。例如，在常见的Win32消息循环中，可以使用以下代码段来获取所有输入消息并在没有消息时等待：

```c++
while (GetMessage(&msg, NULL, 0, 0))
{
    if (msg.message == WM_QUIT)
    {
        // 处理退出消息
        break;
    }

    if ((msg.hwnd == NULL) &&
        (msg.message >= WM_MOUSEFIRST) && (msg.message <= WM_MOUSELAST))
    {
        // 处理鼠标输入消息
    }
    else if ((msg.hwnd == NULL) &&
             (msg.message >= WM_KEYFIRST) && (msg.message <= WM_KEYLAST))
    {
        // 处理键盘输入消息
    }

    TranslateMessage(&msg);
    DispatchMessage(&msg);
}
```

在此示例中，我们使用GetMessage函数从消息队列中获取下一个消息，并根据消息类型进行相应的处理。如果检测到退出消息，则退出消息循环。否则，如果消息是鼠标输入或键盘输入消息，则执行相应的操作。最后，调用TranslateMessage和DispatchMessage函数将该消息分配给正确的窗口过程进行处理。

总之，PeekMessage函数提供了灵活的消息检索和处理机制，使得Windows程序可以响应各种用户输入、系统事件和操作系统消息。熟练掌握PeekMessage函数的使用方法可以帮助开发者实现可靠和高性能的Windows应用程序。