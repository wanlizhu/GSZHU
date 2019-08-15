#pragma once

#include "Common/Config.h"
#include "Graphics/RHI/Object.h"
#include "Graphics/RHI/Constants.h"
#include "Graphics/Tools.h"

namespace ZHU
{
	struct ZHU_GS_API IDeviceObjectDesc : public Object
	{
		IDeviceObjectDesc(const std::string& name);

		void AddDependency(const std::string& name);
		size_t GetDependencyCount() const;
		const std::string& GetDependency(size_t index) const;

	private:
		std::vector<std::string> mObjectDependencies;
	};


    class ZHU_GS_API IDeviceObject : public Object
    {
    public:
        using Pointer = std::shared_ptr<IDeviceObject>;
        using Desc = IDeviceObjectDesc;

		IDeviceObject(const std::string& name) : Object(name) {}
        virtual const IDeviceObjectDesc& GetDesc() const = 0;

    protected:
        virtual bool Initialize() = 0;
    };
}