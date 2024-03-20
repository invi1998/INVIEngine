#include "AOCommon.hlsl"
#include "AOFunc.hlsl"

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
	
	float3 NoiseSampleValue = SampleNoiseMap.SampleLevel(TextureSampler, mvOut.Texcoord, 0).xyz;
	
	float3 DepthNdc = SampleDepthMap.SampleLevel(DepthSampler, mvOut.Texcoord, 0).rrr;
	
	float A_ViewDepth = DepthNDCToView(DepthNdc.r);
	
	float3 AViewPos = A_ViewDepth * mvOut.ViewPosition.xyz / mvOut.ViewPosition.z;
	
	// 从噪波中采样环境光，获得环境光的方向，这里得到的值是[0,1]
	float3 AmbientLightDirection = SampleNoiseMap.SampleLevel(TextureSampler, mvOut.Texcoord, 0.f);
	
	// 将 [0,1] 映射到 [-1, 1]
	AmbientLightDirection = AmbientLightDirection * 2.f - 1.f;
	
	// 遮蔽值
	float Occlusion = 0.f;
	
	for (int i = 0; i < SAMPLE_VOLUME_NUM; i++)
	{
		// 将我们当前的采样点的数据作为法线，然后将环境光的方向作为入射光，然后计算反射光得到环境光反射的方向
		float3 AmbientLightReflect = reflect(SampleVolumeData[i].xyz, AmbientLightDirection);
		
		// sign函数返回一个值的符号，如果值为负数则返回-1，如果值为正数则返回1，如果值为0则返回0
		float SignValue = sign(dot(AmbientLightReflect, NormalizedSampleValue));
		
		float3 BViewPos = AViewPos + SignValue * AmbientLightReflect * OcclusionRadius;	// 计算出我们的B点的位置，用反射光的方向乘以遮蔽半径，然后再乘以符号值，如果是正数则是加，如果是负数则是减
		
		// 求C点位置过程
		// 首先，因为我们B和C在同一条直线上，我们先将B转换到纹理空间下，转换到纹理空间后我们再除以w，就将B点的位置转换到近景层面，然后我们提取他的X,Y,然后我们再在（X,Y)这个像素下去提取深度信息
		// 如果这个深度信息大于B点的深度信息，那么我们就认为这个点是可见的，如果这个点是可见的，那么我们就认为这个点是遮蔽的，我们就将遮蔽值加1
		
		float4 CTexturePosition = mul(float4(BViewPos, 1.f), TextureProjectionMatrix);
		CTexturePosition /= CTexturePosition.w;		// 齐次除法将值归一化到近景层面里
		
		float CDepth = SampleDepthMap.SampleLevel(DepthSampler, CTexturePosition.xy, 0.f).r;
		
		float3 CViewPos = CDepth * BViewPos / BViewPos.z;	// C点的位置
		
		// 如何判断SSAO半球遮蔽
		// 首先我们需要判断C点的位置是否在B点的位置的前面，如果在前面，那么我们就认为这个点是可见的，如果这个点是可见的，那么我们就认为这个点是遮蔽的，我们就将遮蔽值加1
		// 其次，如果我们的深度差太远，这个值我们也是不要的
		float DepthDistance = AViewPos.z - CViewPos.z;	// 点和遮挡物之间的距离
		
		// 法线和A点到C点连线的点积
		float NDotAC = max(dot(NormalizedSampleValue, normalize(CViewPos - AViewPos)), 0.f);
		
		// 遮蔽值，有多少光线被遮蔽了
		Occlusion += NDotAC * OcclusionFuncion(DepthDistance);

	}
	
	// 可及率，这个值越大，表示这个点越亮，越小表示这个点越暗，表示光能到达这个点的可能性
	float Accessibility = 1.f - Occlusion;
	
	// return float4(NormalizedSampleValue, 1.f);
	// return float4(DepthNdc.rrr, 1.f);
	// return float4(NoiseSampleValue, 1.f);
	return float4(saturate(pow(Accessibility, 6.f)), saturate(pow(Accessibility, 6.f)), saturate(pow(Accessibility, 6.f)), 1.f);
}
