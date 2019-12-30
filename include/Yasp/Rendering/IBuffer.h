#ifndef YASP_IBUFFER_H
#define YASP_IBUFFER_H

#include <Yasp/Rendering/GPUResourceID.h>
#include <Yasp/Rendering/AssignableMemory.h>
#include <string>

namespace yasp
{
	class IBuffer
	{
	public:
		IBuffer() {}
		virtual ~IBuffer() = 0 {}
		virtual AssignableMemory operator[](const std::string&) = 0;
		virtual void Update(const GPUResourceID& id) = 0;
	};
}

#endif
