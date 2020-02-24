#include "Utils/UniqueID.h"
#include "Core/BasicTypes.h"

namespace Wanli
{
    static HashMap<std::type_index, Uint> gdNextIdMap;

    Uint GetNextIdFor(std::type_index type)
    {
        return gdNextIdMap[type]++;
    }
}