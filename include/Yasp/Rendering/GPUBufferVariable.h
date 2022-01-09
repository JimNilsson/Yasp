#ifndef YASP_GPU_BUFFER_VARIABLE_H
#define YASP_GPU_BUFFER_VARIABLE_H

#include <Yasp/Rendering/AssignableMemory.h>
#include <Yasp/Rendering/GPUResourceID.h>
#include <string>

namespace yasp
{
	class IGPUResourceManager;
	class GPUBufferVariable : public AssignableMemory
	{
	public:
		GPUBufferVariable();
		GPUBufferVariable(void* data, size_t size, IGPUResourceManager* rscMgr, const GPUResourceID& id, const std::string& name);
		~GPUBufferVariable();

		GPUBufferVariable operator[](size_t index);
		GPUBufferVariable operator[](const std::string& identifier);

		template <typename T>
		void operator=(const T& object)
		{
			assert(data != nullptr);
			assert(size - (offset * sizeof(object)) >= sizeof(object));
			auto realOffset = sizeof(object) * offset;
			memcpy(static_cast<uint8_t*>(data) + realOffset, (void*)&object, sizeof(object));
		}
		template <typename T>
		operator T&()
		{
			return *reinterpret_cast<T*>(static_cast<uint8_t*>(data) + offset * sizeof(T));
		}

	private:
		IGPUResourceManager* resourceManager;
		GPUResourceID id;
		std::string name;
	};
}

#endif
