#include "RHI/Class/CPipelineBase.h"
#include "Utils/Logger.h"

namespace Wanlix
{
    CPipelineBase::CPipelineBase(IDevice* device,
                                      const PipelineDesc& desc,
                                      const String& name)
        : CDeviceObjectBase<IPipeline>(device, desc, name)
    {}
}