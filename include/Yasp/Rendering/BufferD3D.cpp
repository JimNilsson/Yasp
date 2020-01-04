#include <Yasp/Rendering/BufferD3D.h>
#include <Yasp/Rendering/GPUResourceManagerD3D.h>
#include <string>

yasp::BufferD3D::BufferD3D(GPUResourceManagerD3D * resourceManager, size_t size, void* initialData) : resourceManager(resourceManager), size(size)
{
	data = new uint8_t[size];
	if (initialData)
	{
		memcpy(data, initialData, size);
	}
	else
	{
		memset(data, 0, size);
	}
}

yasp::BufferD3D::~BufferD3D()
{
	delete[] data;
}

yasp::BufferD3D::BufferD3D(const BufferD3D & other)
{
	this->data = other.data;
	this->size = other.size;
}

yasp::BufferD3D& yasp::BufferD3D::operator=(const BufferD3D & other)
{
	IBuffer::operator=(other);
	this->data = other.data;
	this->size = other.size;
	this->dataPoints = other.dataPoints;
	return *this;
}

yasp::AssignableMemory yasp::BufferD3D::operator[](const std::string & identifier)
{
	if (auto f = dataPoints.find(identifier); f != dataPoints.end())
	{
		return AssignableMemory(static_cast<uint8_t*>(data) + f->second.offset, f->second.size);
	}
	return AssignableMemory(data, size);
}

void yasp::BufferD3D::Update(const GPUResourceID& id)
{
	resourceManager->UpdateBuffer(id, data, size);
}

void yasp::BufferD3D::RegisterProperty(const std::string & identifier, int32_t size, int32_t offset)
{
	dataPoints[identifier] = { size, offset };
}
