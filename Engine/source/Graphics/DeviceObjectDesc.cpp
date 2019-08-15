#include "Graphics/RHI/IDeviceObject.h"

namespace ZHU
{
	IDeviceObjectDesc::IDeviceObjectDesc(const std::string& name)
		: Object(name)
	{}

	void IDeviceObjectDesc::AddDependency(const std::string& name)
	{
		mObjectDependencies.emplace_back(name);
	}

	size_t IDeviceObjectDesc::GetDependencyCount() const
	{
		return mObjectDependencies.size();
	}

	const std::string& IDeviceObjectDesc::GetDependency(size_t index) const
	{
		return mObjectDependencies[index];
	}
}