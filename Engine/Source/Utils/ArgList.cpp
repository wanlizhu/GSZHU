#include "ArgList.h"

namespace Wanli
{
    ArgList::ArgList(int argc, char** argv)
    {
        mArgs.reserve(argc);
        for (int i = 0; i < argc; ++i)
        {
            mArgs.emplace_back(argv[i]);
            if (mArgs.back()[0] == '-')
            {
                String name(mArgs.back());
                name.erase(0, name.find_first_not_of("- "));
                mOptionIndexMap[name] = i;
            }
        }
    }

    int ArgList::GetArgCount() const
    {
        return (int)mArgs.size();
    }

    Optional<String> ArgList::operator[](int index) const
    {
        if (mArgs.size() <= index)
        {
            return std::nullopt;
        }
        return mArgs[index];
    }

    Optional<String> ArgList::operator[](const String& option) const
    {
        if (mOptionIndexMap.find(option) == mOptionIndexMap.end())
        {
            return std::nullopt;
        }
        
        int index = mOptionIndexMap.at(option);
        if (index == (mArgs.size() - 1) ||
            mArgs[index + 1][0] == '-')
        {
            return "";
        }
        else
        {
            String value;
            for (int i = index + 1; i < mArgs.size(); i++)
            {
                if (mArgs[i][0] == '-')
                    break;
                value += (mArgs[i] + " ");
            }
            value.pop_back();
            return value;
        }

    }
}