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

	private:
		ID3D11Device* device;
		ID3D11DeviceContext* deviceContext;
		struct GPUResourceD3D
		{
			ID3D11Resource* resource;
		};
		uint32 resourceCounter;
		std::unordered_map<int32, GPUResourceD3D> resourceMap;
	};
};

#endif
