// 采样状态
SamplerState TextureSampler : register(s0); // 贴图采样器
SamplerState AnisotropicSampler : register(s1); // 各向异性采样器
SamplerComparisonState ShadowSampler : register(s2); // 阴影采样器
SamplerState PointClampSampler : register(s3); // 点限制采样器

Texture2D SampleNormalMap : register(t0); // 法线

static const float2 TextureCoordinates[6] =
{
	float2(0.f, 1.f),
	float2(0.f, 0.f),
	float2(1.f, 0.f),
	
	float2(0.f, 1.f),
	float2(1.f, 0.f),
	float2(1.f, 1.f),
};

struct MeshVertexOut
{
	float4 ViewPosition : POSITION;
	float4 Position : SV_POSITION;
	float2 Texcoord : TEXCOORD; // UV
};

cbuffer CBufferSSAOView : register(b0)
{
	float4x4 InversiveProjectionMatrix;	// 求逆后的投影矩阵
	float4x4 ProjectionMatrix;			// 摄像机的投影矩阵
	float4x4 TextureProjectionMatrix;	// 纹理空间的投影矩阵
};


// `SV_VertexID` 是在着色器程序中使用的系统内置变量，它用于表示当前顶点在顶点缓冲区中的索引。这个变量通常用于计算每个顶点的唯一标识符或执行与顶点相关的操作。

//在图形渲染管线中， 几何图元（
//如三角形） 会被分解为多个顶点，
//并存储在顶点缓冲区中。 当渲染器在执行顶点着色器时，
//它会为每个顶点调用一次顶点着色器函数， 并为该顶点提供一些内置变量，
//其中` SV_VertexID`
//就是其中之一。

//以下是一些使用`
//SV_VertexID` 的常见用途：

//1.
//生成唯一标识符： 通过使用`
//SV_VertexID` 可以生成每个顶点的唯一标识符。
//这对于执行顶点动画、 顶点位移或其他与顶点相关的计算很有用。

//2.
//索引顶点数据： 当从顶点缓冲区中读取顶点数据时，
//可以使用` SV_VertexID`
//作为索引来访问特定顶点的数据。 这在顶点变换、
//法线计算等操作中经常用到。

//3. 顶点着色器流程控制：
//根据` SV_VertexID`
//的值， 可以在顶点着色器中执行不同的代码路径。
//例如， 可以根据`
//SV_VertexID` 执行特定的变换、
//纹理采样等操作， 以实现一些顶点特定的效果。

//需要注意的是，` SV_VertexID`
//的值是从0开始递增的， 对应于顶点在顶点缓冲区中的索引。
//它在顶点着色器中是只读的， 无法手动修改。

//总而言之，` SV_VertexID`
//是一个系统内置变量， 用于表示顶点在顶点缓冲区中的索引。
//它可以用于生成顶点的唯一标识符、 顶点数据的索引和顶点着色器中的流程控制。
MeshVertexOut VSMain(uint VertexID : SV_VertexID)
{
	
	MeshVertexOut outV;
	
	outV.Texcoord = TextureCoordinates[ VertexID];
	
	// 我们需要将 0-1 映射到NDC空间，所以我们需要将UV坐标乘以2，然后减去1，然后因为Y轴是反的，所以我们需要将Y轴的UV坐标乘以-1
	outV.Position = float4(outV.Texcoord.x * 2.f - 1.f, (outV.Texcoord.y * 2.f - 1.f) * -1.f, 0.f, 1.f);
	
	// 将点转换到视口空间下
	float4 PositionH = mul(outV.Position, InversiveProjectionMatrix);
	
	// 转到近景层面
	outV.ViewPosition.xyz = PositionH.xyz / PositionH.w;

	return outV;
}

float4 PSMain(MeshVertexOut mvOut) : SV_TARGET
{
	float3 NormalizedSampleValue = normalize(SampleNormalMap.SampleLevel(TextureSampler, mvOut.Texcoord, 0).xyz);
	return float4(NormalizedSampleValue, 1.f);
}

