#pragma once

#include "GIPlatformVk.h"
#include "GICommonVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIDeviceVk;

	class GIIDebugObjectVk
	{
	public:
        struct DebugTag
        {
            uint64_t tagName = 0;
            size_t tagSize = 0;
            const void* tag = nullptr;
        };

		virtual bool IsValid() const = 0;
        virtual void SetDebugName(const char* name) const = 0;
        virtual void SetDebugTag(const DebugTag& tag) const = 0;
	};

    class GIDeviceObjectVk 
        : public GIIDebugObjectVk 
		, public std::enable_shared_from_this<GIDeviceObjectVk>
    {
        friend class GIDeviceVk;
    public:
        virtual ~GIDeviceObjectVk();

        SharedPtr<GIDeviceVk> GetDevice() const;

    protected:
        GIDeviceObjectVk(SharedPtr<GIDeviceVk> device);
        GIDeviceObjectVk(const GIDeviceObjectVk&) = delete;
        GIDeviceObjectVk(GIDeviceObjectVk&& rhs) = default;
        GIDeviceObjectVk& operator=(const GIDeviceObjectVk&) = delete;
        GIDeviceObjectVk& operator=(GIDeviceObjectVk&& rhs) = default;

        void SetDebugNameInternal(void* object, VkDebugReportObjectTypeEXT type, const char* name) const;
        void SetDebugTagInternal(void* object, VkDebugReportObjectTypeEXT type, const DebugTag& tag) const;

    protected:
        SharedPtr<GIDeviceVk> mDevice;
    };

    class GIWeakDeviceObjectVk 
        : public GIIDebugObjectVk
        , public std::enable_shared_from_this<GIWeakDeviceObjectVk>
    {
    public:
        virtual ~GIWeakDeviceObjectVk();

        WeakPtr<GIDeviceVk> GetDevice() const;

    protected:
        GIWeakDeviceObjectVk(WeakPtr<GIDeviceVk> device);
        GIWeakDeviceObjectVk(const GIWeakDeviceObjectVk&) = delete;
        GIWeakDeviceObjectVk(GIWeakDeviceObjectVk&& rhs) = default;
        GIWeakDeviceObjectVk& operator=(const GIWeakDeviceObjectVk&) = delete;
        GIWeakDeviceObjectVk& operator=(GIWeakDeviceObjectVk && rhs) = default;

        void SetDebugNameInternal(void* object, VkDebugReportObjectTypeEXT type, const char* name) const;
        void SetDebugTagInternal(void* object, VkDebugReportObjectTypeEXT type, const DebugTag& tag) const;

    protected:
        WeakPtr<GIDeviceVk> mDevice;
    };
}