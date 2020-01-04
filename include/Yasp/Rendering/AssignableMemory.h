#ifndef YASP_ASSIGNABLE_MEMORY_H
#define YASP_ASSIGNABLE_MEMORY_H

#include <string>
#include <cassert>

namespace yasp
{
	class AssignableMemory
	{
	public:
		AssignableMemory() : data(nullptr), size(0), offset(0) {}
		AssignableMemory(void* data, size_t size) : data(data), size(size), offset(0) {}
		~AssignableMemory() {}

		template <typename T>
		void operator=(const T& object)
		{
			assert(data != nullptr);
			assert(size - (offset * sizeof(object)) >= sizeof(object));
			auto realOffset = sizeof(object) * offset;
			memcpy(static_cast<uint8_t*>(data) + realOffset, (void*)&object, sizeof(object));
		}

		AssignableMemory operator[](size_t index)
		{
			return AssignableMemory(data, size, index);
		}

	private:
		AssignableMemory(void* data, size_t size, int32_t offset) : data(data), size(size), offset(offset) {}
		void* data;
		size_t size;
		int32_t offset;

	};
}


#endif