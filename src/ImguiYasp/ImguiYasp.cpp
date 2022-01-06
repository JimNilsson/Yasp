
#include <Yasp/ImguiYasp/ImguiYasp.h>
#include <Yasp/Window/Window.h>
#include <Yasp/Rendering/RenderContext.h>
#include <Yasp/SwizzleVec.h>
#include <Yasp/ImguiYasp/imgui_impl_win32.h>

static yasp::RenderContext* imguiRenderContext = nullptr;
//static yasp::Window* imguiWindow = nullptr;
static yasp::IGPUResourceManager* resourceManager = nullptr;


struct ImguiRenderData {
	yasp::GPUBuffer vertexBuffer;
	yasp::GPUBuffer indexBuffer;
	yasp::Shader vertexShader;
	yasp::Shader pixelShader;
	yasp::GPUResourceID fontSampler;
	yasp::GPUResourceID fontTexture;
	yasp::GPUResourceID rasterizer;
	yasp::GPUResourceID depthStencilState;
	yasp::GPUResourceID blendState;
};

ImguiRenderData renderData;

IMGUI_IMPL_API bool ImGui_ImplYasp_Init(yasp::Window & window, yasp::RenderContext & renderContext)
{
	
	imguiRenderContext = &renderContext;
	resourceManager = imguiRenderContext->ResourceManager();
	//imguiWindow = &window;

	ImGui_ImplWin32_Init(window.GetWindowHandle());
	

	yasp::BufferDesc vertexBufferDesc = {
		10000,
		sizeof(ImDrawVert),
		yasp::BufferBinding::BIND_VERTEX_BUFFER,
		yasp::Usage::GPU_READ_CPU_WRITE
	};
	renderData.vertexBuffer = resourceManager->CreateBuffer(vertexBufferDesc, nullptr);

	yasp::BufferDesc indexBufferDesc = {
		10000,
		sizeof(ImDrawIdx),
		yasp::BufferBinding::BIND_INDEX_BUFFER,
		yasp::Usage::GPU_READ_CPU_WRITE
	};
	renderData.indexBuffer = resourceManager->CreateBuffer(indexBufferDesc, nullptr);
	
	static const char* imguiVertexShaderCode =
		"cbuffer cameraBuffer : register(b0) \
            {\
              float4x4 ProjectionMatrix; \
            };\
            struct VS_INPUT\
            {\
              float2 pos : POSITION;\
              float2 uv : TEXCOORD0;\
              float4 col  : COLOR0;\
            };\
            \
            struct PS_INPUT\
            {\
              float4 pos : SV_POSITION;\
              float4 col : COLOR0;\
              float2 uv  : TEXCOORD0;\
            };\
            \
            PS_INPUT main(VS_INPUT input)\
            {\
              PS_INPUT output;\
              output.pos = mul( ProjectionMatrix, float4(input.pos.xy, 0.f, 1.f));\
              output.col = input.col;\
              output.uv  = input.uv;\
              return output;\
            }";
	renderData.vertexShader = resourceManager->CreateVertexShader(imguiVertexShaderCode, strlen(imguiVertexShaderCode));

	renderData.rasterizer = resourceManager->CreateRasterizer({
		yasp::FillMode::SOLID,
		yasp::CullMode::NONE,
		yasp::WindingOrder::CLOCKWISE,
		false,
		true
	});

	yasp::DepthStencilDesc dsd;
	dsd.depthTest = false;
	dsd.depthWriteMask = yasp::DepthWriteMask::ALL;
	dsd.comparisonMode = yasp::ComparisonFunc::ALWAYS;

	renderData.depthStencilState = resourceManager->CreateDepthStencilState(dsd);

	
	ImGuiIO& io = ImGui::GetIO();
	uint8_t* pixels = nullptr;
	int32_t width, height;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
	
	yasp::Texture2DDesc texDesc;
	texDesc.width = width;
	texDesc.height = height;
	texDesc.mipLevels = 1;
	texDesc.arraySize = 1;
	texDesc.format = yasp::TextureFormat::UNORM8_4;
	texDesc.sampleCount = 1;
	texDesc.usage = yasp::Usage::GPU_READ_CPU_WRITE;
	texDesc.bind = yasp::TextureBind::TEXTURE_BIND_SHADER_VIEW;
	texDesc.sampleQuality = 0;

	auto texture = resourceManager->CreateTexture2D(texDesc, pixels);
	
	yasp::Texture2DViewDesc tvd;
	tvd.format = yasp::TextureFormat::UNORM8_4;
	tvd.mipLevels = 1;
	tvd.mostDetailedMip = 0;
	renderData.fontTexture = resourceManager->CreateTexture2DView(tvd, texture);
	texture.Release();
	
	yasp::SamplerDesc sd;
	sd.filter = yasp::TextureFilter::TRILINEAR;
	sd.wrapModeU = yasp::TextureWrapping::REPEAT;
	sd.wrapModeV = yasp::TextureWrapping::REPEAT;
	sd.wrapModeW = yasp::TextureWrapping::REPEAT;
	sd.maxAnisotropy = 0;
	sd.mipLODBias = 0.0f;
	sd.comparisonFunc = yasp::ComparisonFunc::ALWAYS;
	sd.minLOD = 0.0f;
	sd.maxLOD = 0.0f;
	
	renderData.fontSampler = resourceManager->CreateSampler(sd);
	
	static const char* imguiPixelShaderCode =
		"struct PS_INPUT\
            {\
            float4 pos : SV_POSITION;\
            float4 col : COLOR0;\
            float2 uv  : TEXCOORD0;\
            };\
            sampler fontSampler;\
            Texture2D fontTexture;\
            \
            float4 main(PS_INPUT input) : SV_Target\
            {\
            float4 out_col = input.col * fontTexture.Sample(fontSampler, input.uv); \
            return out_col; \
            }";

	renderData.pixelShader = resourceManager->CreatePixelShader(imguiPixelShaderCode, strlen(imguiPixelShaderCode));
	renderData.pixelShader["fontSampler"] = renderData.fontSampler;
	renderData.pixelShader["fontTexture"] = renderData.fontTexture;
	
	yasp::BlendStateDesc bsd;
	bsd.alphaToCoverage = false;
	bsd.alphaToCoverage = false;
	bsd.renderTargetBlend[0].enable = true;
	bsd.renderTargetBlend[0].srcBlend = yasp::BlendFactor::SRC_ALPHA;
	bsd.renderTargetBlend[0].destBlend = yasp::BlendFactor::INV_SRC_ALPHA;
	bsd.renderTargetBlend[0].blendOpColor = yasp::BlendOp::ADD;
	bsd.renderTargetBlend[0].srcBlendAlpha = yasp::BlendFactor::ONE;
	bsd.renderTargetBlend[0].destBlendAlpha = yasp::BlendFactor::INV_SRC_ALPHA;
	bsd.renderTargetBlend[0].blendOpAlpha = yasp::BlendOp::ADD;
	renderData.blendState = resourceManager->CreateBlendState(bsd);

	return true;
}

IMGUI_IMPL_API void ImGui_ImplYasp_Shutdown()
{
	ImGui_ImplWin32_Shutdown();
	renderData.vertexBuffer.Release();
	renderData.indexBuffer.Release();
	renderData.vertexShader.Release();
	renderData.pixelShader.Release();
	renderData.fontSampler.Release();
	renderData.fontTexture.Release();
	renderData.rasterizer.Release();
	renderData.depthStencilState.Release();
	renderData.blendState.Release();
	return IMGUI_IMPL_API void();
}

IMGUI_IMPL_API void ImGui_ImplYasp_NewFrame()
{
	ImGui_ImplWin32_NewFrame();
	return IMGUI_IMPL_API void();
}

IMGUI_IMPL_API void ImGui_ImplYasp_RenderDrawData(ImDrawData * drawData)
{
	if (!drawData)
	{
		return;
	}
	resourceManager->PushState();

	yasp::Viewport vp;
	vp.width = drawData->DisplaySize.x;
	vp.height = drawData->DisplaySize.y;
	vp.minDepth = 0.0f;
	vp.maxDepth = 1.0f;
	vp.topLeftX = 0.0f;
	vp.topLeftY = 0.0f;
	imguiRenderContext->SetViewport(vp);
	imguiRenderContext->SetTopology(yasp::Topology::TRIANGLE_LIST);

	size_t vtxOffset = 0;
	size_t idxOffset = 0;
	for (int n = 0; n < drawData->CmdListsCount; n++)
	{
		auto vtxData = drawData->CmdLists[n]->VtxBuffer;
		auto idxData = drawData->CmdLists[n]->IdxBuffer;
		renderData.vertexBuffer.Stage(vtxData.Data, vtxData.Size * sizeof(ImDrawVert), vtxOffset);
		renderData.indexBuffer.Stage(idxData.Data, idxData.Size * sizeof(ImDrawIdx), idxOffset);
		vtxOffset += vtxData.Size * sizeof(ImDrawVert);
		idxOffset += idxData.Size * sizeof(ImDrawIdx);
	}
	renderData.vertexBuffer.Update();
	renderData.indexBuffer.Update();
	float L = drawData->DisplayPos.x;
	float R = drawData->DisplayPos.x + drawData->DisplaySize.x;
	float T = drawData->DisplayPos.y;
	float B = drawData->DisplayPos.y + drawData->DisplaySize.y;
	yasp::mat4 projMatrix = {
		{ 2.0f / (R - L),   0.0f,           0.0f,       0.0f },
		{ 0.0f,         2.0f / (T - B),     0.0f,       0.0f },
		{ 0.0f,         0.0f,           0.5f,       0.0f },
		{ (R + L) / (L - R),  (T + B) / (B - T),    0.5f,       1.0f },
	};
	renderData.vertexShader["cameraBuffer"]["ProjectionMatrix"] = projMatrix;
	renderData.vertexShader["cameraBuffer"].Update();
	
	resourceManager->SetVertexBuffer(renderData.vertexBuffer, sizeof(ImDrawVert), 0);
	resourceManager->SetIndexBuffer(renderData.indexBuffer, yasp::IndexFormat::UINT16, 0);
	renderData.vertexShader.Bind();
	renderData.pixelShader.Bind();
	
	const float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	resourceManager->SetBlendState(renderData.blendState, blendFactor, 0xffffffff);
	resourceManager->SetDepthStencilState(renderData.depthStencilState, 0);
	resourceManager->SetRasterizer(renderData.rasterizer);
	
	imguiRenderContext->DrawIndexed(idxOffset / sizeof(ImDrawIdx), 0, 0);
	resourceManager->PopState();
	return IMGUI_IMPL_API void();
}
