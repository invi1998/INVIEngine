#include "EngineMinimal.h"
#include "EngineFactory.h"

#include "Log/SimpleLog.h"


int Init(CEngine* Engine, HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCMD)
{
#if defined(_WIN32)
	FWinMainCommandParameters WinMainParameters(hInstance, prevInstance, cmdLine, showCMD);
#endif

	int ReturnValue = Engine->PreInit(
#if defined(_WIN32)
		WinMainParameters
#endif
	);
	if (ReturnValue != 0)
	{
		ENGINE_LOG_ERROR("[%i]Engine pre initialization error, check and initialization problem.", ReturnValue)
			return ReturnValue;
	}

	ReturnValue = Engine->Init(
#if defined(_WIN32)
		WinMainParameters
#endif
	);
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

	return ReturnValue;
}

void Tick(CEngine* Engine)
{
	float DeltaTime = 0.03f;
	Engine->Tick(DeltaTime);
	// Sleep(30);
}

int Exit(CEngine* Engine)
{
	int ReturnValue = Engine->PreExit();
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
}


CEngine* Engine = nullptr;


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

	Engine = FEngineFactory::CreateEngine();

	if (Engine)
	{
		Init(Engine, hInstance, prevInstance, cmdLine, showCMD);

		MSG EngineMSG = { nullptr };
		
		while(EngineMSG.message != WM_QUIT)
		{
			// �����͹���Windows��Ϣ
			// ���д����ִ��Ч�����ǣ��Ƴ����е���Ϣ���������κι���
			// PM_NOREMOVE		��Ϣ������Ϣ�������Ƴ�
			// PM_REMOVE		��Ϣ����Ϣ�������Ƴ�
			// PM_NOYIELD		�˱�־ʹ��ϵͳ���ͷŵȴ����ó�����е��߳�
			//
			// PM_QS_INPUT		�������ͼ�����Ϣ
			// PM_QS_PAINT		����ͼ��Ϣ
			// PM_QS_POSTMESSAGE	�������б����͵���Ϣ��������ʱ�����ȼ�
			// PM_QS_SENDMESSAGE	�������з�����Ϣ
			if (PeekMessage(&EngineMSG, 0, 0, 0, PM_REMOVE))
			{
				// ��ȡ����Ϣ��������Ҫ����Ϣ���з��룬����Ϊ�ַ���
				TranslateMessage(&EngineMSG);
				// ������Ϣ������Ϣ���͸����ǵĳ��򴰿ڣ�Ȼ�����ǵĳ�����ܽ���ʹ���� (�ַ���Ϣ��
				DispatchMessage(&EngineMSG);
			}
			else
			{
				Tick(Engine);
			}
		}

		Exit(Engine);

		return 1;
	}
	else
	{
		ReturnValue = 1;
	}


	ENGINE_LOG("[%i]The engine has exited.", ReturnValue)
	return 1;
}
