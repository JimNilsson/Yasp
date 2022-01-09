#include <Yasp/Rendering/GPUBufferVariable.h>
#include <Yasp/Rendering/IGPUResourceManager.h>

yasp::GPUBufferVariable::GPUBufferVariable() : yasp::AssignableMemory(), resourceManager(nullptr), id()
{
}

yasp::GPUBufferVariable::GPUBufferVariable(void * data, size_t size, IGPUResourceManager * rscMgr, const GPUResourceID & id, const std::string & name)
	: yasp::AssignableMemory(data, size), resourceManager(rscMgr), id(id), name(name)
{
}

yasp::GPUBufferVariable::~GPUBufferVariable()
{
}

yasp::GPUBufferVariable yasp::GPUBufferVariable::operator[](size_t index)
{
	return resourceManager->GetBufferVariable(id, name, index);
}

yasp::GPUBufferVariable yasp::GPUBufferVariable::operator[](const std::string & identifier)
{
	const auto internalOffsetSize = resourceManager->GetBufferVariableOffsetSize(id, name, identifier);
	const auto offset = std::get<0>(internalOffsetSize);
	const auto size = std::get<1>(internalOffsetSize);
	return GPUBufferVariable(static_cast<uint8_t*>(data) + offset, size, nullptr, {}, {});
}
