#ifndef YASP_ASSIGNABLE_MEMORY_H
#define YASP_ASSIGNABLE_MEMORY_H

#include <string>
#include <cassert>

namespace yasp
{
	class AssignableMemory
	{
	public:
		AssignableMemory() : data(nullptr), size(0) {}
		AssignableMemory(void* data, size_t size) : data(data), size(size) {}
		~AssignableMemory() {}

		template <typename T>
		void operator=(const T& object)
		{
			assert(data != nullptr);
			assert(size == sizeof(object));
			memcpy(data, (void*)&object, sizeof(object));
		}

	private:
		void* data;
		size_t size;
	};
}


#endif