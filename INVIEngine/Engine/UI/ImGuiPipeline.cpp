#include "EngineMinimal.h"
#include "ImGuiPipeline.h"

FImGuiPipeline::FImGuiPipeline()
{

}

void FImGuiPipeline::Initialize(ID3D12DescriptorHeap* heap, UINT offset)
{
	// ���汾
	IMGUI_CHECKVERSION();

	// ��ʼ��������
	ImGui::CreateContext();

	// ������ʽ
	ImGui::StyleColorsDark();

	// ��ʼ��Win32ƽ̨
	ImGui_ImplWin32_Init(GetMainWindowsHandle());

	// ��ȡƫ�ƺ��CPU/GPU���
	const auto cpuHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(heap->GetCPUDescriptorHandleForHeapStart(), offset, GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
	const auto gpuHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(heap->GetGPUDescriptorHandleForHeapStart(), offset, GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));

	// ��ʼ��DX12ƽ̨
	ImGui_ImplDX12_Init(GetD3dDevice().Get(), 1, DXGI_FORMAT_R8G8B8A8_UNORM, heap, cpuHandle, gpuHandle);

}

void FImGuiPipeline::Draw(float deltaTime)
{
	// ��ʼ��֡
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();

	// ����UI
	Tick(deltaTime);

	// ��Ⱦ
	ImGui::Render();
	// ������Ļ�� ע�ᵽDX12�������б���
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), GetD3dGraphicsCommandList().Get());
}

void FImGuiPipeline::Exit()
{
}

void FImGuiPipeline::Tick(float deltaTime)
{
	ImGui::NewFrame();

	// ����UI
	ImGui::Begin("Hello, world!");
	ImGui::Text("This is some useful text.");
	ImGui::End();
}
