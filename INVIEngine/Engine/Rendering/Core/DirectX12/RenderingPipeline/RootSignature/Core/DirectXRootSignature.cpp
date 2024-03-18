#include "EngineMinimal.h"

#include "DirectXRootSignature.h"

FDirectXRootSignature::FDirectXRootSignature()
{
}

void FDirectXRootSignature::BuildRootSignature(UINT textureNum)
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///构建根签名

	// 创建根参数，使用上面的描述符范围
	CD3DX12_ROOT_PARAMETER RootParam[10];

	// 纹理 CBV描述表
	CD3DX12_DESCRIPTOR_RANGE DescriptorRangeTextureSRV;	// 常量缓冲区区描述符范围 描述符范围（Descriptor Range）的创建
	DescriptorRangeTextureSRV.Init(
		D3D12_DESCRIPTOR_RANGE_TYPE_SRV,	// 指定视图（这里指向常量缓冲区视图 （描述符类型）），对于纹理，这里我们选择shaderRenderingView
		textureNum,							// 贴图数量
		4);						// 基于那个着色器的寄存器（绑定寄存器（shaderRegister 和 registerSpace））

	// 静态CubeMap CBV描述表
	CD3DX12_DESCRIPTOR_RANGE DescriptorRangeCubeMapSRV;
	DescriptorRangeCubeMapSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	// 万向阴影贴图 CBV描述表
	CD3DX12_DESCRIPTOR_RANGE DescriptorCubeMapShadowSRV;
	DescriptorCubeMapShadowSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);

	// 阴影贴图 CBV描述表
	CD3DX12_DESCRIPTOR_RANGE DescriptorRangeShadowMapSRV;
	DescriptorRangeShadowMapSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 2);

	// SSAO CBV描述表
	CD3DX12_DESCRIPTOR_RANGE DescriptorRangeSSAOSRV;
	DescriptorRangeSSAOSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 3);

	// register(t0, space1)
	RootParam[0].InitAsConstantBufferView(0);		// Mesh对象
	RootParam[1].InitAsConstantBufferView(1);		// 视口
	RootParam[2].InitAsConstantBufferView(2);		// 灯光
	RootParam[3].InitAsConstantBufferView(3);		// 雾
	// register(t0, space1)
	RootParam[4].InitAsShaderResourceView(0, 1);		// 材质


	// 2D贴图
	RootParam[5].InitAsDescriptorTable(
		1,								// 描述符数量
		&DescriptorRangeTextureSRV,			// 指向描述符范围数组的指针
		D3D12_SHADER_VISIBILITY_PIXEL	// 着色器可见性(该值默认为shader可见，一般不用设置）
	);

	// cube map
	RootParam[6].InitAsDescriptorTable(
		1,								// 描述符数量
		&DescriptorRangeCubeMapSRV,			// 指向描述符范围数组的指针
		D3D12_SHADER_VISIBILITY_PIXEL	// 着色器可见性(该值默认为shader可见，一般不用设置）
	);

	// Shadow Map
	RootParam[7].InitAsDescriptorTable(
		1,								// 描述符数量
		&DescriptorRangeShadowMapSRV,			// 指向描述符范围数组的指针
		D3D12_SHADER_VISIBILITY_PIXEL	// 着色器可见性(该值默认为shader可见，一般不用设置）
	);

	// CubeMap Shadow
	RootParam[8].InitAsDescriptorTable(
		1,								// 描述符数量
		&DescriptorCubeMapShadowSRV,			// 指向描述符范围数组的指针
		D3D12_SHADER_VISIBILITY_PIXEL	// 着色器可见性(该值默认为shader可见，一般不用设置）
	);

	// SSAO
	RootParam[9].InitAsDescriptorTable(
		1,								// 描述符数量
		&DescriptorRangeSSAOSRV,			// 指向描述符范围数组的指针
		D3D12_SHADER_VISIBILITY_PIXEL	// 着色器可见性(该值默认为shader可见，一般不用设置）
	);

	// 序列化根签名，将我们当前的描述二进制连续的一个内存(将根签名（Root Signature）序列化为字节流数据)

	// 设置采样方式-静态采样方式
	StaticSamplerObject.BuildStaticSampler();

	// 根签名（Root Signature）描述结构体的创建
	CD3DX12_ROOT_SIGNATURE_DESC RootSignatureDesc(
		10,			// 参数数量
		RootParam,	// 根签名参数
		StaticSamplerObject.GetSize(),			// 静态采样数量
		StaticSamplerObject.GetData(),			// 静态采样数据（传入采样数据指针）
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT // 指定根签名布局选项 表示根签名允许输入汇编程序访问根常量数据。
	);

	// 这个函数可以将根签名序列化为一个 ID3DBlob 对象，该对象包含了一个字节流，可以用于在后续的操作中传递、保存和加载根签名。
	ComPtr<ID3DBlob> SerializeRootSignatureBlob;	// 存储序列化成功的二进制数据流
	ComPtr<ID3DBlob> ErrorBlob;						// 存储序列化失败的信息
	D3D12SerializeRootSignature(
		&RootSignatureDesc,							// 传入要序列化的根签名描述结构体指针
		D3D_ROOT_SIGNATURE_VERSION_1,				// 根签名的版本号
		SerializeRootSignatureBlob.GetAddressOf(),	// 序列化后的根签名数据
		ErrorBlob.GetAddressOf()					// 该对象包含了在序列化过程中出现错误时产生的错误信息
	);

	if (ErrorBlob)
	{
		ENGINE_LOG_ERROR("%s", static_cast<char*>(ErrorBlob->GetBufferPointer()));
	}

	// 成功，创建根签名
	GetD3dDevice()->CreateRootSignature(
		0,			// 表示要创建的根签名对象所属的节点掩码，通常设置为 0, 表示单个CPU。(设备的物理适配器）
		SerializeRootSignatureBlob->GetBufferPointer(),			// 表示包含序列化根签名数据的内存地址。
		SerializeRootSignatureBlob->GetBufferSize(),			// 表示序列化根签名数据的字节长度。
		IID_PPV_ARGS(&RootSignature)							// 传入根签名接收地址
	);
}

void FDirectXRootSignature::PreDraw(float DeltaTime)
{
	// 设置根签名
	GetD3dGraphicsCommandList()->SetGraphicsRootSignature(RootSignature.Get());
}

void FDirectXRootSignature::Draw(float DeltaTime)
{
}

void FDirectXRootSignature::PostDraw(float DeltaTime)
{
}
