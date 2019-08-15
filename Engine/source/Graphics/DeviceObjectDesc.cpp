#include "Graphics/RHI/DeviceObjectDesc.h"

namespace ZHU
{
	DeviceObjectDesc::DeviceObjectDesc(const std::string& name)
		: Object(name)
	{}

	void DeviceObjectDesc::AddDependency(const std::string& name)
	{
		mObjectDependencies.emplace_back(name);
	}

	size_t DeviceObjectDesc::GetDependencyCount() const
	{
		return mObjectDependencies.size();
	}

	const std::string& DeviceObjectDesc::GetDependency(size_t index) const
	{
		return mObjectDependencies[index];
	}
}