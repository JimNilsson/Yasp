#ifndef YASP_GPU_BUFFER_H
#define YASP_GPU_BUFFER_H

#include <Yasp/Rendering/GPUResourceID.h>
#include <Yasp/Rendering/AssignableMemory.h>
#include <functional>


namespace yasp
{
	class IGPUResourceManager;
	class GPUBuffer : public GPUResourceID
	{
	public:
		GPUBuffer();
		GPUBuffer(uint32_t id, IGPUResourceManager* resourceManager);
		virtual ~GPUBuffer();
		GPUBuffer(const GPUBuffer& other);
		virtual GPUBuffer& operator=(const GPUBuffer& other);
		AssignableMemory operator[](const std::string& identifier);
		void Update();
		void OnEachElement(std::function<void(const std::string&, AssignableMemory)> callback);

	private:
		IGPUResourceManager* resourceManager;

	};
};

#endif
