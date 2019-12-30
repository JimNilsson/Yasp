#ifndef YASP_BUFFER_D3D_H
#define YASP_BUFFER_D3D_H

#include <Yasp/Rendering/IBuffer.h>
#include <Yasp/Rendering/AssignableMemory.h>
#include <unordered_map>

namespace yasp
{
	class GPUResourceManagerD3D;

	class BufferD3D : public IBuffer
	{
	public:
		BufferD3D(GPUResourceManagerD3D* resourceManager, size_t size, void* initialData = nullptr);
		~BufferD3D();
		BufferD3D(const BufferD3D& other);
		BufferD3D& operator=(const BufferD3D& other);
		AssignableMemory operator[](const std::string& identifier) override final;
		void Update(const GPUResourceID& id) override final;
	
	private: 
		struct SizeOffset
		{
			size_t size;
			size_t offset;
		};
		void* data;
		size_t size;
		GPUResourceManagerD3D* resourceManager;
		std::unordered_map<std::string, SizeOffset> dataPoints;
	};
}

#endif
