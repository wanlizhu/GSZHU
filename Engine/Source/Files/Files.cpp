#include "Files.h"
#include "Utils/Log.h"

namespace Wanli
{
    Files::Files()
    {}

    Files::~Files()
    {
        LOG_DEBUG("[IModule] Files Destroy\n");
    }

    void Files::Update()
    {}

    void Files::AddSearchPath(const Path& path)
    {
        mSearchPaths.push_back(path);
    }

    void Files::ClearSearchPath()
    {
        mSearchPaths.clear();
    }
}