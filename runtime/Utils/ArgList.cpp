#include "ArgList.h"
#include "Utils/Strings.h"
#include "Utils/Logger.h"

namespace Wanlix
{
    bool ArgVal::IsEmpty() const
    {
        return mValue.empty();
    }

    bool ArgVal::ToBool() const
    {
        String val = ToLower(mValue);
        if (StringEqual3(val.c_str(), "true", "on", "1")) {
            return true;
        }
        if (StringEqual3(val.c_str(), "false", "off", "0")) {
            return false;
        }
        LOG_WARNING("Unable to convert \"", mValue, "\" to Bool.");
        return false;
    }

    Int ArgVal::ToInt() const
    {
        try
        {
            return std::stoi(mValue);
        }
        catch (std::invalid_argument& e)
        {
            LOG_WARNING("Unable to convert \"", mValue, "\" to int. Exception: ", e.what());
            return -1;
        }
        catch (std::out_of_range& e)
        {
            LOG_WARNING("Unable to convert \"", mValue, "\" to int. Exception: ", e.what());
            return -1;
        }
    }

    Uint ArgVal::ToUint() const
    {
        try
        {
            return std::stoul(mValue);
        }
        catch (std::invalid_argument& e)
        {
            LOG_WARNING("Unable to convert \"", mValue, "\" to unsigned int. Exception: ", e.what());
            return -1;
        }
        catch (std::out_of_range& e)
        {
            LOG_WARNING("Unable to convert \"", mValue, "\" to unsigned int. Exception: ", e.what());
            return -1;
        }
    }

    Uint64 ArgVal::ToUint64() const
    {
        try
        {
            return std::stoull(mValue);
        }
        catch (std::invalid_argument& e)
        {
            LOG_WARNING("Unable to convert \"", mValue, "\" to unsigned int64. Exception: ", e.what());
            return -1;
        }
        catch (std::out_of_range& e)
        {
            LOG_WARNING("Unable to convert \"", mValue, "\" to unsigned int64. Exception: ", e.what());
            return -1;
        }
    }

    float ArgVal::ToFloat() const
    {
        try
        {
            return std::stof(mValue);
        }
        catch (std::invalid_argument& e)
        {
            LOG_WARNING("Unable to convert \"", mValue, "\" to float. Exception: ", e.what());
            return -1;
        }
        catch (std::out_of_range& e)
        {
            LOG_WARNING("Unable to convert \"", mValue, "\" to float. Exception: ", e.what());
            return -1;
        }
    }

    String ArgVal::ToString() const
    {
        return mValue;
    }

    void ArgList::AddArgs(int argc, char** argv)
    {
        String str;
        for (int i = 0; i < argc; i++) {
            str += argv[i];
        }
        //todo
    }

    void ArgList::AddArg(const String& key)
    {
        size_t n = mMap[key].size();
    }

    void ArgList::AddArg(const String& key, const ArgVal& val)
    {
        mMap[key].push_back(val);
    }

    bool ArgList::HasArg(const String& key) const
    {
        return mMap.find(key) != mMap.end();
    }

    Array<ArgVal> ArgList::GetPosArgs() const
    {
        return mPosArgs;
    }

    Array<ArgVal> ArgList::GetValues(const String& key) const
    {
        auto it = mMap.find(key);
        if (it == mMap.end()) {
            return {};
        }
        return it->second;
    }

    const ArgVal& ArgList::operator[](const String& key) const
    {
        return mMap.at(key)[0];
    }
}