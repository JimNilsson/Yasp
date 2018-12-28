#ifndef YASP_BUFFER_H
#define YASP_BUFFER_H

#include <Yasp/Config.h>
#include <Yasp/Rendering/IBuffer.h>

class Buffer
{
private:
	IBuffer * buffer;
	Buffer(IBuffer* buffer);
public:
	Buffer();
	~Buffer();

};


#endif
