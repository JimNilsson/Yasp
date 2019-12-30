#ifndef YASP_GPU_BUFFER_H
#define YASP_GPU_BUFFER_H

#include <Yasp/Rendering/IBuffer.h>
#include <Yasp/Rendering/GPUResourceID.h>
#include <Yasp/Rendering/AssignableMemory.h>

namespace yasp
{
	class GPUBuffer : public GPUResourceID
	{
	public:
		GPUBuffer(uint32_t id, IBuffer* buffer) : GPUResourceID(id), buffer(buffer) {}
		virtual ~GPUBuffer() 
		{
			if(*(this->refCount) == 2)
				delete buffer;
		}
		GPUBuffer(const GPUBuffer& other) : GPUResourceID(other)
		{
			this->buffer = other.buffer;
		}
		virtual GPUBuffer& operator=(const GPUBuffer& other)
		{
			GPUResourceID::operator=(other);
			this->buffer = other.buffer;
			return *this;
		}

		AssignableMemory operator[](const std::string& identifier)
		{
			return (*buffer)[identifier];
		}
		void Update()
		{
			buffer->Update(*this);
		}

	private:
		IBuffer * buffer;

	};
};

#endif
