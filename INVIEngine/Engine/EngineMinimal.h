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
//< wrl.h > ͷ�ļ��ṩ��һЩ�ؼ����ģ�壬ʹ��C++������Ա�ܹ������ɵش�����ʹ��Windows Runtime�����Ӧ�ó�������һЩ���ģ�������
// Microsoft::WRL::ComPtr��һ������ָ��ģ�壬���ڹ���Windows Runtime��������ü�����
// Microsoft::WRL::RuntimeClass��һ��ģ���࣬���ڶ���̳���Windows Runtime�ӿڵ�C++�ࡣ
// Microsoft::WRL::Implements��һ��ģ���࣬����ʵ��һ������Windows Runtime�ӿڡ�
// Microsoft::WRL::Make��һ��ģ�庯�������ڴ���Windows Runtime�����ʵ����
#include <dxgi1_4.h>
//< dxgi1_4.h > ��DirectX Graphics Infrastructure�⣨DXGI���е�һ��ͷ�ļ�����������һ��C++�ӿںͽṹ�壬���ڹ��������ϵ�ͼ������������ʾ���ʹ��ڵ�Ӳ����Դ��
// DXGI����Microsoft DirectX������һ���֣����ṩ��һ��ͨ�õġ�������ͼ��API��ʹ��Ӧ�ó�����Գ�����ò�ͬ���͵�Ӳ���豸�����ҿ����ڶ������ϵͳ��ƽ̨��ʹ�á�DXGI����Ҫ�������¹��ܣ�
// Ӳ����Դ��������ͼ������������ʾģʽ�����Դ�ʹ��ڵ�Ӳ����Դ�Ĺ�������á�
// �����������������͹�����Ӳ����ʾ���Ľ��������Լ�ʵ��˫�����������ȸ߼���Ⱦ������
// ȫ��ģʽ������������˳�ȫ��ģʽ���Լ���������ʾ��ʱ����ʾ�߼���
//<dxgi1_4.h>ͷ�ļ���DXGI���е�һ���ض��汾����������һЩ�µĽӿںͽṹ�壬����֧��DirectX 12��Windows 10����ϵͳ������һЩ��Ҫ�Ľӿںͽṹ�������
// IDXGIFactory4��������ӿڣ����ڴ���������DXGI����
// DXGI_SWAP_CHAIN_DESC1�������������ṹ�壬�������������������Ժ����á�
// DXGI_PRESENT_PARAMETERS�����ֲ����ṹ�壬����ָ�����ֲ��������Ժ�ѡ�
#include <d3d12.h>
//< d3d12.h > ��Direct3D 12 API��ͷ�ļ������ṩ��һ��C++�ࡢ�ṹ���ö�����ͣ�����ʵ�ָ�����ͼ����Ⱦ�ͼ��㡣
// Direct3D 12��Microsoft DirectX�����е�һ����ϵͳ������һ�ֵײ��Ӳ������㣨Hardware Abstraction Layer��
// ������Windows 10����ϵͳ�����ö��CPU������GPU��Ӳ����Դ��ʵ�ָ����ٺ͸���Ч��ͼ�δ����ܡ�
// Direct3D 12�����Direct3D 11�����и�ϸ���ȵ��߳̿��ơ����͵�CPU���������̵���Ⱦ�ӳٵ����ƣ��ʺ��ڴ��ģ��Ϸ��������ʵӦ�ó���ȳ�����
//<d3d12.h>ͷ�ļ�������һЩ�ؼ�����ͽṹ�壬����һЩ���õ���ͽṹ�������
// ID3D12Device���豸�ӿڣ����ڴ���������Direct3D 12����
// D3D12_DESCRIPTOR_HEAP_DESC���������������ṹ�壬���������������ѵ����Ժ����á�
// D3D12_RESOURCE_BARRIER����Դ���Ͻṹ�壬����������Դ״̬ת���Ĳ�����
// D3D12_COMMAND_QUEUE_DESC��������������ṹ�壬��������������е����Ժ����á�
#include <D3Dcompiler.h>
//< D3Dcompiler.h > ��DirectX Shader Compiler��ͷ�ļ������ṩ��һ�麯���ͽṹ�壬���ڱ���ͷ�����HLSL��ɫ�����롣
// DirectX Shader Compiler��DXC����Microsoft DirectX�����е�һ����ϵͳ������һ�������ı��������ߣ����ڽ��߼�����ɫ�����ԣ�HLSL������ת��ΪGPU��ִ�еĵͼ���ָ�DXC������Windows����ϵͳ�����У�����֧�ֶ��ƽ̨��Ӳ���豸��
//<D3Dcompiler.h>ͷ�ļ�������һЩ�ؼ��ĺ����ͽṹ�壬����һЩ���õĺ����ͽṹ�������
// D3DCompile������HLSL��ɫ������ĺ�������HLSL����ת��ΪGPU��ִ�е��ֽ��롣
// D3DDisassemble�������GPU��ִ���ֽ���ĺ��������ֽ���ת��ΪHLSL���롣
// ID3DBlob�������ƴ��Ͷ���ӿڣ����ڱ�ʾ�ʹ����ֽ����顣
#include <DirectXMath.h>
//< DirectXMath.h > ��DirectX Math���ͷ�ļ������ṩ��һ��C++�����ͽṹ�壬���ڴ�����ά�������������Ԫ���ȼ����ͼ��ѧ�г�������ѧ���㡣
// DirectX Math��Microsoft DirectX�����е�һ����ϵͳ������һ�������ܵ���ѧ�⣬����֧�ּ����ͼ��ѧ������ģ�⡢��Ϸ��̺��˹����ܵ�����DirectX Math��Ҫ�������¹��ܣ�
// ��ά���������������Ӽ����������������ȹ�һ���Ͳ�ֵ�����㡣
// ����任����������˷��������ת�þ������ž�����ת�����ƽ�ƾ�������㡣
// ��Ԫ����������Ԫ���Ӽ����������������ȹ�һ���Ͳ�ֵ�����㡣
// �����㷨���������߼�⡢AABB��Χ�в��ԡ�������Ժ�ƽ����Ե����㡣
//<DirectXMath.h>ͷ�ļ�������һЩ�ؼ��ĺ����ͽṹ�壬����һЩ���õĺ����ͽṹ�������
// XMVECTOR���������ͣ����ڱ�ʾ��ά��������Ԫ����
// XMMATRIX���������ͣ����ڱ�ʾ4x4����
// XMStoreFloat3 / 4����XMVECTOR���͵�����ת��ΪFLOAT3 / FLOAT4���͵�������
// XMMatrixRotationQuaternion��������Ԫ��������ת����
#include <DirectXPackedVector.h>
//< DirectXPackedVector.h > ��DirectXMath���һ��ͷ�ļ������ṩ��һ��C++�ṹ��ͺ��������ڽ�32λ��������16λ�������������4�ֽڻ�2�ֽڵĸ�ʽ��
// DirectXMath����Microsoft DirectX�����е�һ����ϵͳ�����ṩ�˸����ܵ���ѧ�⣬����֧�ּ����ͼ��ѧ������ģ�⡢��Ϸ��̺��˹����ܵ���������<DirectXPackedVector.h>ͷ�ļ���ΪDirectXMath���һ���֣���Ҫ����ѹ�����ݴ�С�������ڴ����
//<DirectXPackedVector.h>ͷ�ļ�������һЩ�ؼ��Ľṹ��ͺ���������һЩ���õĽṹ��ͺ���������
// XMVECTORU8/XMUBYTEN4�����ڴ洢�޷���8λ����������
// XMVECTORI4/XMSHORTN4�����ڴ洢�з���16λ����������
// XMVectorPackFloat2/4�����������ĸ�32λ�����������16λ����������
// XMVectorPackUnsigned/Int2/4�����������ĸ�16λ���������32λ����������
#include <DirectXColors.h>
//< DirectXColors.h > ��DirectX Tool Kit��һ��ͷ�ļ�����������һ��C++�����ͺ��������ڱ�ʾ������ɫ��������ɫ����ȹ��ܡ�
// DirectX Tool Kit��Microsoft DirectX�����е�һ����ϵͳ�����ṩ��һ�鹤�߿⣬���ڼ�DirectXӦ�ó���Ŀ���������<DirectXColors.h>ͷ�ļ���ΪDirectX Tool Kit��һ���֣���Ҫ�ṩ�˷����ݵػ�ȡ������ɫֵ��������ɫ����ķ�����
//<DirectXColors.h>ͷ�ļ�������һЩ�ؼ��ĳ����ͺ���������һЩ���õĳ����ͺ���������
// DirectX::Colors::White / DarkGray / Gray / Black / Red / Green / Blue / Yellow / Cyan / Magenta����ʾ������ɫ�ĳ�����
// DirectX::PackedVector::XMCOLOR�����ڴ洢RGBA��ɫ�Ľṹ�塣
// DirectX::PackedVector::XMUBYTEN4�����ڴ洢�޷���8λ������ɫֵ�Ľṹ�塣
// DirectX::PackedVector::XMUDECN4 / XMUNIBBLE4�����ڴ洢�޷���4λ��5λ������ɫֵ�Ľṹ�塣
// DirectX::PackedVector::XMConvertColorTo * ������ͬ������ɫֵת����XMCOLOR��XMUBYTEN4���͡�
#include <DirectXCollision.h>

//< DirectXCollision.h > ��DirectX Tool Kit��һ��ͷ�ļ�����������һ��C++�ṹ��ͺ��������ڼ���ʹ���������ײ��������ģ�����⡣
// DirectX Tool Kit��Microsoft DirectX�����е�һ����ϵͳ�����ṩ��һ�鹤�߿⣬���ڼ�DirectXӦ�ó���Ŀ���������<DirectXCollision.h>ͷ�ļ���ΪDirectX Tool Kit��һ���֣���Ҫ�ṩ����ײ��������ģ��ȹ��ܡ�
//<DirectXCollision.h>ͷ�ļ�������һЩ�ؼ��Ľṹ��ͺ���������һЩ���õĽṹ��ͺ���������
// DirectX::BoundingBox / DirectX::BoundingOrientedBox / DirectX::BoundingSphere / DirectX::BoundingFrustum����ʾ��ͬ��״�İ�Χ�еĽṹ�塣
// DirectX::BoundingVolumeHierarchy����ʾ��Χ�в㼶�ṹ�Ľṹ�塣
// DirectX::Intersects���ж�������Χ���Ƿ��ཻ�ĺ�����
// DirectX::ComputeIntersection�������Χ��֮�����Сƽ�ƾ������ײ�����Ϣ�ĺ�����

using namespace Microsoft::WRL;
using namespace DirectX;
using namespace DirectX::PackedVector;

extern class CEngine *Engine;
