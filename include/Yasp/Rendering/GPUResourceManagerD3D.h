#ifndef YASP_GPU_RESOURCE_MANAGER_D3D_H
#define YASP_GPU_RESOURCE_MANAGER_D3D_H

#include <Yasp/Config.h>
#include <Yasp/Rendering/IGPUResourceManager.h>
#include <Yasp/Rendering/RenderContextD3D.h>
#include <d3d11.h>
#include <unordered_map>

namespace yasp
{
	class GPUResourceManagerD3D : public IGPUResourceManager
	{
	public:
		GPUResourceManagerD3D(RenderContextD3D* renderContext);
		~GPUResourceManagerD3D();

		GPUResourceID CreateBuffer(BufferDesc bufferDesc, void* initialData) override final;
		GPUResourceID CreateVertexShader(const std::string& filename) override final;
		GPUResourceID CreatePixelShader(const std::string& filename) override final;

		void UpdateBuffer(GPUResourceID id, void* data, uint32 size) override final;

		void SetVertexBuffer(GPUResourceID id, uint32 stride, uint32 offset);
		void SetVertexShaderBuffer(GPUResourceID id, uint32 slot);
		void SetVertexShader(GPUResourceID id);
		void SetPixelShader(GPUResourceID id);

	private:
		void VertexShaderReflection(ID3DBlob* shaderByteCode);
	private:
		ID3D11Device* device;
		ID3D11DeviceContext* deviceContext;
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
				ID3D11Texture2D* texture;
				ID3D11ShaderResourceView* shaderResourceView;
				ID3D11RenderTargetView* renderTargetView;
				ID3D11DepthStencilView* depthStencilView;
				ID3D11DepthStencilState* depthStencilState;
				ID3D11RasterizerState* rasterizerState;
				ID3D11BlendState* blendState;
				ID3D11InputLayout* inputLayout;
			};
			
		};
		uint32 resourceCounter;
		std::unordered_map<GPUResourceID, GPUResourceD3D, GPUResourceID::Hasher> resourceMap;
	};
};

#endif
