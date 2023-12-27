#include "EngineMinimal.h"
#include "ImGuiPipeline.h"

#include "EditorEngine.h"

FImGuiPipeline::FImGuiPipeline()
{

}

void FImGuiPipeline::Initialize(ID3D12DescriptorHeap* heap, UINT offset)
{
	// ���汾
	IMGUI_CHECKVERSION();

	// ��ʼ��������
	ImGui::CreateContext();

	// ����Docking
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

	// io.Fonts->AddFontFromFileTTF("Asserts/fonts/pingfang/pingfangtc-medium.otf", 16.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());
	// io.FontDefault = io.Fonts->AddFontFromFileTTF("Asserts/fonts/pingfang/pingfangtc-regular.otf", 16.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());
	io.Fonts->AddFontFromFileTTF("Asserts/fonts/pingfang/pingfangtc-medium.otf", 18.0f);
	io.FontDefault = io.Fonts->AddFontFromFileTTF("Asserts/fonts/pingfang/pingfangtc-regular.otf", 18.0f);

	// ��ʼ��Win32ƽ̨
	ImGui_ImplWin32_Init(GetMainWindowsHandle());

	// ������ʽ
	ImGui::StyleColorsDark();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	SetDarkThemeColors();

	// ��ȡƫ�ƺ��CPU/GPU���
	const auto cpuHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(heap->GetCPUDescriptorHandleForHeapStart(), offset, GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
	const auto gpuHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(heap->GetGPUDescriptorHandleForHeapStart(), offset, GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));

	// ��ʼ��DX12ƽ̨
	ImGui_ImplDX12_Init(GetD3dDevice().Get(), 3, DXGI_FORMAT_R8G8B8A8_UNORM, heap, cpuHandle, gpuHandle);

	// ����Editor�ؼ�
	GetEditorEngine()->BuildEditor();

}

void FImGuiPipeline::Draw(float deltaTime)
{
	// ��ʼ��֡
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();

	// ����UI
	Tick(deltaTime);

	// ��Ⱦ
	ImGui::Render();

	ImGuiIO& io = ImGui::GetIO();

	// Update and Render additional Platform Windows
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault(nullptr, (void*)GetD3dGraphicsCommandList().Get());
	}

	// ������Ļ�� ע�ᵽDX12�������б���
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), GetD3dGraphicsCommandList().Get());
}

void FImGuiPipeline::Exit()
{
	// �ͷ�DX12ƽ̨
	ImGui_ImplDX12_Shutdown();

	// �ͷ�Win32ƽ̨
	ImGui_ImplWin32_Shutdown();

	// �ͷ�������
	ImGui::DestroyContext();

	GetEditorEngine()->ExitEditor();
}

void FImGuiPipeline::SetDarkThemeColors()
{
	auto& colors = ImGui::GetStyle().Colors;

	colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

	// Header
	colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Button
	colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Frame BG
	colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Tabs
	colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
	colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
	colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

	// Title
	colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
}

void FImGuiPipeline::Tick(float deltaTime)
{
	// ����UI
	GetEditorEngine()->DrawEditor();
}
