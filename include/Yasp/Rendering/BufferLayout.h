#ifndef YASP_BUFFER_LAYOUT_H
#define YASP_BUFFER_LAYOUT_H

#include <string>
#include <unordered_map>

namespace yasp
{
	class BufferLayout
	{
	public:
		BufferLayout() {}
		~BufferLayout() {}
		struct OffsetSize
		{
			int32_t offset;
			int32_t size;
		};

		OffsetSize operator[](const std::string& identifier)
		{
			if (auto f = variableBinding.find(identifier); f != variableBinding.end())
			{
				return f->second;
			}
			return { -1, -1 };
		}

	private:
		std::unordered_map<std::string, OffsetSize> variableBinding;
	};
}

#endif
