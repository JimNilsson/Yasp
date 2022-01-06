#ifndef YASP_GPU_RESOURCE_MANAGER_D3D_H
#define YASP_GPU_RESOURCE_MANAGER_D3D_H

#include <Yasp/Config.h>
#include <Yasp/Rendering/IGPUResourceManager.h>
#include <Yasp/Rendering/RenderContextD3D.h>
#include <d3d11.h>
#include <d3d11shader.h>
#include <unordered_map>
#include <Yasp/Rendering/GPUBuffer.h>
#include <Yasp/Rendering/ShaderD3D.h>
#include <Yasp/Rendering/Shader.h>
#include <Yasp/Rendering/AssignableMemory.h>

namespace yasp
{
	class GPUResourceManagerD3D : public IGPUResourceManager
	{
	public:
		GPUResourceManagerD3D(RenderContextD3D* renderContext);
		~GPUResourceManagerD3D();

		GPUBuffer CreateBuffer(const BufferDesc& bufferDesc, void* initialData) override final;
		Shader CreateVertexShader(const void* shaderSourceCode, size_t shaderSourceSize) override final;
		Shader CreateVertexShader(const std::string& filename) override final;
		Shader CreatePixelShader(const std::string& filename) override final;
		Shader CreatePixelShader(const void* shaderSourceCode, size_t shaderSourceSize) override final;
		Shader GetShader(const GPUResourceID& id) override final;
		GPUResourceID CreateRasterizer(RasterizerDesc rasterizerDesc) override final;
		GPUResourceID CreateTexture2D(const Texture2DDesc& textureDesc, void* data = nullptr) override final;
		GPUResourceID CreateTexture2DView(const Texture2DViewDesc& textureViewDesc, const GPUResourceID& texture) override final;
		GPUResourceID CreateSampler(const SamplerDesc& samplerDesc) override final;
		GPUResourceID CreateDepthStencilState(const DepthStencilDesc& depthStencilDesc) override final;
		GPUResourceID CreateBlendState(const BlendStateDesc& blendStateDesc) override final;

		void UpdateBuffer(const GPUResourceID& id, void* data, uint32 size) override final;
		void UpdateBuffer(const GPUResourceID& id) override final;
		void StageBuffer(const GPUResourceID& id, void* data, size_t size, size_t offset = 0) override final;
		size_t GetBufferElementCount(const GPUResourceID& id)  override final;
		size_t GetBufferSize(const GPUResourceID& id) const override final;
		const std::string& GetBufferElementName(const GPUResourceID& id, size_t offset) override final;
		AssignableMemory GetBufferSegment(const GPUResourceID& id, const std::string& identifier) override final;
		AssignableMemory GetBufferSegment(const GPUResourceID& id, size_t offset) override final;

		void PushState() override final;
		void PopState() override final;

		void SetVertexBuffer(const GPUResourceID& id, uint32 stride, uint32 offset) override final;
		void SetIndexBuffer(const GPUResourceID& id, IndexFormat format, uint32 offset) override final;
		void SetShaderBuffers(ShaderType shader, GPUResourceID* buffers, uint32 startSlot, uint32 count) override final;
		void SetShaderTextureViews(ShaderType shader, GPUResourceID* textureViews, uint32 startSlot, uint32 count) override final;
		void SetShaderSamplers(ShaderType shader, GPUResourceID* samplers, uint32 startSlot, uint32 count) override final;
		void SetVertexShader(const GPUResourceID& id) override final;
		void SetGeometryShader(const GPUResourceID& id) override final;
		void SetPixelShader(const GPUResourceID& id) override final;
		void SetRasterizer(const GPUResourceID& id) override final;
		void SetDepthStencilState(const GPUResourceID& id, uint32_t reference = 0) override final;
		void SetBlendState(const GPUResourceID& id, const float* blendFactor, uint32 mask) override final;


	private:
		ID3D11Buffer * CreateInternalBuffer(const BufferDesc& bufferDesc, void* initialData);
		void PixelShaderReflection(ID3DBlob* shaderByteCode, ShaderD3D& shader);
		void VertexShaderReflection(ID3DBlob* shaderByteCode, ShaderD3D& shader);
		void RegisterResourceBindings(const D3D11_SHADER_DESC& desc, ID3D11ShaderReflection* reflection, ShaderD3D& shader);
		void RegisterProperty(const GPUResourceID& id, const std::string& name, int32_t size, int32_t offset);

	private:
		ID3D11Device* device;
		ID3D11DeviceContext* deviceContext;
		struct SizeOffset
		{
			int32_t size;
			int32_t offset;
		};
		struct GPUResourceD3D
		{
			ResourceType type;
			union
			{
				void* ptr;
				ID3D11Buffer* buffer;
				ID3D11VertexShader* vertexShader;
				ID3D11GeometryShader* geometryShader;
				ID3D11PixelShader* pixelShader;
				ID3D11ComputeShader* computeShader;
				ID3D11Texture2D* texture2D;
				ID3D11ShaderResourceView* shaderResourceView;
				ID3D11RenderTargetView* renderTargetView;
				ID3D11DepthStencilView* depthStencilView;
				ID3D11DepthStencilState* depthStencilState;
				ID3D11RasterizerState* rasterizerState;
				ID3D11BlendState* blendState;
				ID3D11InputLayout* inputLayout;
				ID3D11SamplerState* sampler;
			};
			void* resourceData = nullptr;
			size_t resourceDataSize = 0;
			std::unordered_map<std::string, SizeOffset> namedOffsets;
			std::vector<std::string> names;
			std::vector<SizeOffset> offsets;
		};
		uint32 resourceCounter;
		std::unordered_map<GPUResourceID, GPUResourceD3D, GPUResourceID::Hasher> resourceMap;
		struct D3DStateBackup
		{
			D3D11_PRIMITIVE_TOPOLOGY topology;
			ID3D11InputLayout* inputLayout;
			ID3D11Buffer* vertexBuffers[8] = {};
			UINT vertexBufferStrides[8] = {};
			UINT vertexBufferOffsets[8] = {};
			ID3D11Buffer* indexBuffer = nullptr;
			DXGI_FORMAT indexFormat = {};
			UINT indexOffset = 0;
			ID3D11Buffer* vsBuffers[8] = {};
			ID3D11SamplerState* vsSamplers[8] = {};
			ID3D11VertexShader* vshader = nullptr;
			UINT vsClassInstanceCount = 0;
			ID3D11ClassInstance* vsClassInstances[256] = {};
			ID3D11ShaderResourceView* vsResources[8] = {};
			ID3D11RasterizerState* rasterizer = nullptr;
			D3D11_RECT scissorRects[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
			UINT scissorRectsCount = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
			D3D11_VIEWPORT viewPorts[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
			UINT viewPortsCount = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
			ID3D11PixelShader* pshader = nullptr;
			ID3D11ClassInstance* psClassInstances[256];
			UINT psClassInstanceCount = 0;
			ID3D11Buffer* psBuffers[8] = {};
			ID3D11SamplerState* psSamplers[8] = {};
			ID3D11ShaderResourceView* psResources[8] = {};
			ID3D11BlendState* blendState = nullptr;
			FLOAT blendFactor[4] = {};
			UINT sampleMask = 0;
			ID3D11DepthStencilState* depthStencilState = nullptr;
			UINT stencilRef = 0;
			ID3D11UnorderedAccessView* unorderedAccessViews[8] = {};
			ID3D11RenderTargetView* renderTargetViews[8] = {};
			ID3D11DepthStencilView* depthStencilView = nullptr;

			~D3DStateBackup()
			{
				if (inputLayout) inputLayout->Release();
				for (auto& vb : vertexBuffers) if (vb) vb->Release();
				if (indexBuffer) indexBuffer->Release();

				for (auto& b : vsBuffers) if (b) b->Release();
				for (auto& b : vsSamplers) if (b) b->Release();
				for (auto& b : vsResources) if (b) b->Release();
				if (vshader) vshader->Release();
				for (auto& b : vsClassInstances) if (b) b->Release();

				if (rasterizer) rasterizer->Release();

				for (auto& b : psBuffers) if (b) b->Release();
				for (auto& b : psSamplers) if (b) b->Release();
				for (auto& b : psResources) if (b) b->Release();
				if (pshader) pshader->Release();
				for (auto& b : psClassInstances) if (b) b->Release();

				if (blendState) blendState->Release();
				for (auto& b : unorderedAccessViews) if (b) b->Release();
				for (auto& b : renderTargetViews) if (b) b->Release();
				if (depthStencilState) depthStencilState->Release();
				if (depthStencilView) depthStencilView->Release();
			}
		};
		std::vector<D3DStateBackup> backups;
	};
};

#endif
