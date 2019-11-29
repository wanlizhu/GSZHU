#pragma once

#include "Wanlix/Core3D/Utility/NonCopyable.h"
#include "Types.h"
#include "Flags.h"

namespace Wanlix
{
    class GObject : public NonCopyable
    {
    public:
        using Handle   = void*;
        using Ptr      = std::shared_ptr<GObject>;
        using ConstPtr = std::shared_ptr<const GObject>;
        using WeakPtr  = std::weak_ptr<GObject>;

        static GObject* Find(StringCRef name);
        virtual ~GObject();

        void       SetName(StringCRef name);
        StringCRef GetName() const;
        Handle     GetHandle() const;

    protected:
        GObject(StringCRef name);

    protected:
        String mName;
        Handle mHandle = nullptr;

    private:
        static std::unordered_map<String, GObject*> mObjects;
    };
}