#pragma once

#include "Wanlix/Core3D/Utility/NonCopyable.h"
#include "Types.h"
#include "Flags.h"

namespace Wanlix
{
    class Device;

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

    protected:
        GObject(
            StringCRef name,
            std::shared_ptr<Device> device
        ) noexcept;

    protected:
        String mName;
        std::shared_ptr<Device> mDevice;

    private:
        static std::unordered_map<String, GObject*> mObjects;
    };
}