#include "EngineMinimal.h"


/**
 * \brief 
 * \param hInstance 当前窗口实例
 * \param prevInstance 上次启动进程的对象实例（上次启动的窗口实例）
 * \param cmdLine 传递命令
 * \param showCMD 当前窗口以什么方式显示
 *SW_HIDE	隐藏窗口并激活另外一个窗口
 *SW_MINIMIZE	最小化指定窗口，并激活在系统表顶层的窗口
 *SW_RESTORE 激活并显示窗口，如果窗口已经最小化或者最大化，系统将恢复到原来尺寸的位置显示窗口，（与SW_SHOWNORMAL相同）
 *SW_SHOW 激活一个窗口并以原来的尺寸和位置显示窗口
 *SW_SHOWMAXIMIZED 激活窗口并且将其最大化
 *SW_SHOWMINIMIZED 激活窗口并且将其最小化
 *SW_SHOWMINNOACTIVE 将一个窗口显示为图标，并激活窗口维持活动状态
 *SW_SHOWNA 以窗口的当前状态显示窗口，激活窗口保持活动状态
 *SW_SHOWNOACTIVETE 以窗口的最近一次的尺寸和位置显示窗口，不激活窗口
 *SW_SHOWNORMAL	激活并且显示窗口
 * \return 
 */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCMD)
{
	while(true)
	{
		
	}

	return 0;
}