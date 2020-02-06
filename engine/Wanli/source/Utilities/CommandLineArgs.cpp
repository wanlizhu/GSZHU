#include "Utilities/CommandLineArgs.h"
#include "Utilities/Log.h"

namespace Wanli
{
    CommandLineArgs::CommandLineArgs(int argc, char** argv, const CommandLineArgsLayout& layout)
        : argc(argc)
        , argv(argv)
    {
        String argName = "";
        int count = 0;

        for (int i = 0; i < argc; i++)
        {
            if (argv[i][0] == '-')
            {
                argName = argv[i];
                argName.erase(0, argName.find_first_not_of("- \t"));
                mNamedArgs[argName] = {};
                count = 0;
                continue;
            }

            if (argName.empty())
            {
                mUnnamedArgs.push_back(argv[i]);
            }
            else
            {
                mNamedArgs[argName].push_back(argv[i]);
                if (layout.argsNumMap.find(argName) != layout.argsNumMap.end())
                {
                    if (count < layout.argsNumMap.at(argName))
                    {
                        count++;
                    }
                    else
                    {
                        argName = "";
                        count = 0;
                    }
                }
            }
        }

        for (const auto& pair : layout.argsNumMap)
        {
            if (HasArg(pair.first))
            {
                if (mNamedArgs[pair.first].size() < pair.second)
                {
                    LOG_ERROR("There is no enough args for '%s'.\n", pair.first.c_str());
                }
            }
        }

        for (const auto& name : layout.requiredArgs)
        {
            if (!HasArg(name))
            {
                LOG_ERROR("Missing required argument '%s'.\n", name.c_str());
            }
        }
    }

    bool CommandLineArgs::HasArg(const String& name) const
    {
        return mNamedArgs.find(name) != mNamedArgs.end();
    }

    bool CommandLineArgs::IsArgFlag(const String& name) const
    {
        auto it = mNamedArgs.find(name);
        if (it != mNamedArgs.end())
        {
            return it->second.size() == 0;
        }
        return false;
    }

    bool CommandLineArgs::IsArgArray(const String& name) const
    {
        auto it = mNamedArgs.find(name);
        if (it != mNamedArgs.end())
        {
            return it->second.size() > 1;
        }
        return false;
    }

    bool CommandLineArgs::GetArgFlag(const String& name) const
    {
        return IsArgFlag(name);
    }

    const String& CommandLineArgs::GetArg(const String& name) const
    {
        return mNamedArgs.at(name)[0];
    }

    const StringArray& CommandLineArgs::GetArgArray(const String& name) const
    {
        return mNamedArgs.at(name);
    }

    const StringArray& CommandLineArgs::GetUnnamedArgs() const
    {
        return mUnnamedArgs;
    }
}