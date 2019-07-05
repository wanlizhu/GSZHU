#include "ZHUENGINE_PCH.h"
#include "HeapCreator.h"
#include "Log.h"

namespace ZHU
{
    HeapCreator::HeapCreator()
    {
        if (weak_from_this().expired()) {
#ifdef _DEBUG
            LOG_FATAL("Object(%p) is not created using std::make_shared<>()", this);
#else
            LOG_ERROR("Object(%p) is not created using std::make_shared<>()", this);
#endif
        }
    }
}