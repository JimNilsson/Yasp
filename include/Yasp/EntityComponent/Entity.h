#ifndef YASP_ENTITY_H
#define YASP_ENTITY_H

#include <cstdint>
#include <Yasp/Config.h>
#include <vector>
#include <ostream>

namespace yasp
{

	class Entity
	{
	public:
		static const uint32_t INDEX_BITS = 22;
		static const uint32_t INDEX_MASK = (1 << INDEX_BITS) - 1;
		static const uint32_t GENERATION_BITS = 8;
		static const uint32_t GENERATION_MASK = ((1 << GENERATION_BITS) - 1) << INDEX_BITS;

		Entity() : id(0) {}
		Entity(uint32_t id) : id(id) {}
		Entity(uint32_t gen, uint32_t index)
		{
			id = (gen << INDEX_BITS) | index;
		}
		~Entity() {}
		operator uint32_t() const { return this->id; }

		uint8_t Gen() const { return this->id & GENERATION_MASK; }
		uint32_t Index() const { return this->id & INDEX_MASK; }
		bool operator==(const Entity& rhs) const { return this->id == rhs.id; }
		bool operator!=(const Entity& rhs) const { return this->id != rhs.id; }
		bool operator<(const Entity& rhs) const { return this->id < rhs.id; }

		friend std::ostream& operator << (std::ostream& out, const Entity& e)
		{
			out << e.id;
			return out;
		}

		struct EntityHasher
		{
			size_t operator()(const Entity& e) const { return e.id; }
		};

	private:
		uint32_t id;
	};



}

#endif
