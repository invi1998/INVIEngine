#include "EngineMinimal.h"

#include "EngineFactory.h"
#include "Log/SimpleLog.h"

/**
 * \brief 
 * \param hInstance ��ǰ����ʵ��
 * \param prevInstance �ϴ��������̵Ķ���ʵ�����ϴ������Ĵ���ʵ����
 * \param cmdLine ��������
 * \param showCMD ��ǰ������ʲô��ʽ��ʾ
 *SW_HIDE	���ش��ڲ���������һ������
 *SW_MINIMIZE	��С��ָ�����ڣ���������ϵͳ����Ĵ���
 *SW_RESTORE �����ʾ���ڣ���������Ѿ���С��������󻯣�ϵͳ���ָ���ԭ���ߴ��λ����ʾ���ڣ�����SW_SHOWNORMAL��ͬ��
 *SW_SHOW ����һ�����ڲ���ԭ���ĳߴ��λ����ʾ����
 *SW_SHOWMAXIMIZED ����ڲ��ҽ������
 *SW_SHOWMINIMIZED ����ڲ��ҽ�����С��
 *SW_SHOWMINNOACTIVE ��һ��������ʾΪͼ�꣬�������ά�ֻ״̬
 *SW_SHOWNA �Դ��ڵĵ�ǰ״̬��ʾ���ڣ�����ڱ��ֻ״̬
 *SW_SHOWNOACTIVETE �Դ��ڵ����һ�εĳߴ��λ����ʾ���ڣ��������
 *SW_SHOWNORMAL	�������ʾ����
 * \return 
 */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCMD)
{

	int ReturnValue = 0;

	if (FEngine* Engine = FEngineFactory::CreateEngine())
	{
		ReturnValue = Engine->PreInit(
#if defined(_WIN32)
			FWinMainCommandParameters(hInstance, prevInstance, cmdLine, showCMD)
#endif
		);
		if (ReturnValue != 0)
		{
			ENGINE_LOG_ERROR("[%i]Engine pre initialization error, check and initialization problem.", ReturnValue)
			return ReturnValue;
		}

		ReturnValue = Engine->Init();
		if (ReturnValue != 0)
		{
			ENGINE_LOG_ERROR("[%i]Engine initialization error, please check the initialization problem.", ReturnValue)
			return ReturnValue;
		}

		ReturnValue = Engine->PostInit();
		if (ReturnValue != 0)
		{
			ENGINE_LOG_ERROR("[%i]Engine post initialization error, please check initialization problem.", ReturnValue)
			return ReturnValue;
		}

		while(true)
		{
			Engine->Tick();
		}

		ReturnValue = Engine->PreExit();
		if (ReturnValue != 0)
		{
			ENGINE_LOG_ERROR("[%i]Engine pre exit failed.", ReturnValue)
			return ReturnValue;
		}

		ReturnValue = Engine->Exit();
		if (ReturnValue != 0)
		{
			ENGINE_LOG_ERROR("[%i]Engine exit failed.", ReturnValue)
			return ReturnValue;
		}

		ReturnValue = Engine->PostExit();
		if (ReturnValue != 0)
		{
			ENGINE_LOG_ERROR("[%i]Engine post exit failed.", ReturnValue)
			return ReturnValue;
		}

		return 0;
	}
	else
	{
		ReturnValue = 1;
	}


	ENGINE_LOG("[%i]The engine has exited.", ReturnValue)
	return 1;
}