#ifndef YASP_GPU_RESOURCE_ID_H
#define YASP_GPU_RESOURCE_ID_H

#include <Yasp/Config.h>

namespace yasp
{
	class GPUResourceID
	{
	private:
		int32 uid;
		
	public:
		GPUResourceID(int32 uid) : uid(uid) {}
		operator int32() { return uid; }
		~GPUResourceID() {};

	};
};


#endif
