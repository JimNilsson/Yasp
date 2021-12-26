#include <Yasp\Rendering\GPUBuffer.h>
#include <Yasp/Rendering/IGPUResourceManager.h>

yasp::GPUBuffer::GPUBuffer() : GPUResourceID(), resourceManager(nullptr) {}

yasp::GPUBuffer::GPUBuffer(uint32_t id, IGPUResourceManager * resourceManager) : GPUResourceID(id), resourceManager(resourceManager) {}

yasp::GPUBuffer::~GPUBuffer() {}

yasp::GPUBuffer::GPUBuffer(const GPUBuffer & other) : GPUResourceID(other)
{
	this->resourceManager = other.resourceManager;
}

yasp::GPUBuffer& yasp::GPUBuffer::operator=(const GPUBuffer & other)
{
	GPUResourceID::operator=(other);
	this->resourceManager = other.resourceManager;
	return *this;
}

yasp::AssignableMemory yasp::GPUBuffer::operator[](const std::string & identifier)
{
	return resourceManager->GetBufferSegment(*this, identifier);
}

void yasp::GPUBuffer::Update()
{
	resourceManager->UpdateBuffer(*this);
}

void yasp::GPUBuffer::OnEachElement(std::function<void(const std::string&, AssignableMemory)> callback)
{
	auto count = resourceManager->GetBufferElementCount(*this);
	for (size_t i = 0; i < count; i++)
	{
		auto name = resourceManager->GetBufferElementName(*this, i);
		auto bufferSegment = resourceManager->GetBufferSegment(*this, name);
		callback(name, bufferSegment);
	}
}
