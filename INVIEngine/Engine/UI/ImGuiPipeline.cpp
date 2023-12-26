#include "EngineMinimal.h"
#include "ImGuiPipeline.h"

FImGuiPipeline::FImGuiPipeline()
{

}

void FImGuiPipeline::Initialize(ID3D12DescriptorHeap* heap, UINT offset)
{
	// 检查版本
	IMGUI_CHECKVERSION();

	// 初始化上下文
	ImGui::CreateContext();

	// 设置样式
	ImGui::StyleColorsDark();

	// 初始化Win32平台
	ImGui_ImplWin32_Init(GetMainWindowsHandle());

	// 获取偏移后的CPU/GPU句柄
	const auto cpuHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(heap->GetCPUDescriptorHandleForHeapStart(), offset, GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
	const auto gpuHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(heap->GetGPUDescriptorHandleForHeapStart(), offset, GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));

	// 初始化DX12平台
	ImGui_ImplDX12_Init(GetD3dDevice().Get(), 1, DXGI_FORMAT_R8G8B8A8_UNORM, heap, cpuHandle, gpuHandle);

}

void FImGuiPipeline::Draw(float deltaTime)
{
	// 开始新帧
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();

	// 绘制UI
	Tick(deltaTime);

	// 渲染
	ImGui::Render();
	// 画到屏幕上 注册到DX12的命令列表中
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), GetD3dGraphicsCommandList().Get());
}

void FImGuiPipeline::Exit()
{
}

void FImGuiPipeline::Tick(float deltaTime)
{
	ImGui::NewFrame();

	// 绘制UI
	ImGui::Begin("Hello, world!");
	ImGui::Text("This is some useful text.");
	ImGui::End();
}
