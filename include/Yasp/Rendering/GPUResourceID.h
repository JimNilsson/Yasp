#ifndef YASP_GPU_RESOURCE_ID_H
#define YASP_GPU_RESOURCE_ID_H

#include <Yasp/Config.h>

namespace yasp
{
	class GPUResourceID
	{
	protected:
		uint32 uid;
		int32* refCount;
		void Increment() { if (refCount) (*refCount)++; }
		void Decrement() { if (refCount) (*refCount)--; }
		void Clean() 
		{
			if (refCount && *refCount <= 0)
			{
				delete refCount;
			}
		}
		int32_t References() const { return (refCount ? *refCount : 0); }
	public:
		GPUResourceID() : uid(0), refCount(nullptr) {}
		GPUResourceID(uint32 uid) : uid(uid) 
		{
			refCount = new int32(1);
		}
		explicit operator int32() { return uid; }
		virtual ~GPUResourceID()
		{
			Decrement();
			Clean();
		};
		GPUResourceID(const GPUResourceID& other)
		{
			this->uid = other.uid;
			this->refCount = other.refCount;
			Increment();
		}
		virtual GPUResourceID& operator=(const GPUResourceID& other)
		{
			if (this == &other || this->refCount == other.refCount)
				return *this;
			Decrement();
			Clean();
			this->uid = other.uid;
			this->refCount = other.refCount;
			Increment();
			return *this;
		}

		void Release()
		{
			Decrement();
			this->refCount = nullptr;
			this->uid = 0;
		}

		virtual uint32 ID() const { return uid; }
		
		virtual bool operator==(const GPUResourceID& rhs) const { return rhs.uid == this->uid; }

		struct Hasher
		{
			uint32 operator()(const GPUResourceID& id) const { return id.uid; }
		};

	};
};


#endif
