#include "Utils/Object.h"

namespace GS
{
	std::unordered_map<std::string, std::weak_ptr<Object>> Object::smObjects;

	Object::Object(const std::string& name)
		: mObjectName(name)
	{
		static uint64_t _id = 0;
		if (mObjectName.empty())
		{
			mObjectName = "Object" + std::to_string(_id++);
		}

		//assert(!weak_from_this().expired());
		assert(smObjects.find(mObjectName) == smObjects.end());

		smObjects[mObjectName] = weak_from_this();
	}

	Object::~Object()
	{
		auto it = smObjects.find(mObjectName);
		if (it != smObjects.end())
			smObjects.erase(it);
	}

	bool Object::SetObjectName(const std::string& name)
	{
		if (name == mObjectName)
			return true;

		if (smObjects.find(name) != smObjects.end())
			return false;

		auto it = smObjects.find(mObjectName);
		smObjects.erase(it);
		smObjects[name] = weak_from_this();
		mObjectName = name;

		return true;
	}

	const std::string& Object::GetObjectName() const
	{
		return mObjectName;
	}

	void Object::SetObjectProperty(const std::string& key, const std::string& value)
	{
		mObjectProperties[key] = value;
	}

	bool Object::GetObjectProperty(const std::string& key, std::string* value) const
	{
		auto it = mObjectProperties.find(key);
		if (it == mObjectProperties.end())
			return false;

		*value = it->second;
		return true;
	}

	Object::SharedPtr Object::FindObject(const std::string& name)
	{
		auto it = smObjects.find(name);
		if (it == smObjects.end())
			return nullptr;

		Object::SharedPtr ptr = it->second.lock();
		if (!ptr)
			smObjects.erase(it);
		return ptr;
	}

	void Object::ForEachObject(const std::function<void(SharedPtr)>& func)
	{
		for (auto it = smObjects.end(); it != smObjects.end(); )
		{
			Object::SharedPtr ptr = it->second.lock();
			if (!ptr)
			{
				it = smObjects.erase(it);
				continue;
			}
			
			func(ptr);
		}
	}
}