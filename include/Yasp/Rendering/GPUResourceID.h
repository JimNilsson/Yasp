#ifndef YASP_GPU_RESOURCE_ID_H
#define YASP_GPU_RESOURCE_ID_H

#include <Yasp/Config.h>

namespace yasp
{
	class GPUResourceID
	{
	private:
		uint32 uid;
		int32* refCount;
	public:
		GPUResourceID(uint32 uid) : uid(uid) 
		{
			refCount = new int32(1);
		}
		operator int32() { return uid; }
		~GPUResourceID()
		{
			(*refCount)--;
			if (refCount <= 0)
				delete refCount;
		};
		GPUResourceID(const GPUResourceID& other)
		{
			this->uid = other.uid;
			this->refCount = other.refCount;
			(*refCount)++;
		}
		GPUResourceID& operator=(const GPUResourceID& other)
		{
			if (this == &other)
				return *this;
			(*refCount)--;
			if (refCount <= 0)
				delete refCount;
			this->uid = other.uid;
			this->refCount = other.refCount;
			(*refCount)++;
			return *this;
		}

		uint32 ID() const { return uid; }
		
		bool operator==(const GPUResourceID& rhs) const { return rhs.uid == this->uid; }

		struct Hasher
		{
			uint32 operator()(const GPUResourceID& id) const { return id.uid; }
		};

	};
};


#endif
