#include "Utilities/CommandLineArgs.h"
#include "Utilities/Log.h"

namespace Wanli
{
    CommandLineArgs::CommandLineArgs(int argc, char** argv)
    {
        for (int i = 0; i < argc; i++)
        {
            mArgs.push_back(argv[i]);
        }
    }

    Optional<String> CommandLineArgs::FindArg(const String& name) const
    {
        int index = -1;
        for (int i = 0; i < (int)mArgs.size() && index == -1; i++)
        {
            if (mArgs[i][0] == '-')
            {
                index = i;
            }
        }

        if (index >= 0)
        {
            if (index < ((int)mArgs.size() - 1) && mArgs[index + 1][0] != '-')
            {
                return String("");
            }
            return mArgs[index + 1];
        }

        return std::nullopt;
    }

    Optional<StringArray> CommandLineArgs::FindArgArray(const String& name) const
    {
        int index = -1;
        for (int i = 0; i < (int)mArgs.size() && index == -1; i++)
        {
            if (mArgs[i][0] == '-')
            {
                index = i;
            }
        }

        if (index >= 0)
        {
            StringArray result;
            int j = index + 1;
            while (j < (int)mArgs.size() && mArgs[j][0] != '-')
            {
                result.push_back(mArgs[j++]);
            }
            return result;
        }

        return std::nullopt;
    }
}