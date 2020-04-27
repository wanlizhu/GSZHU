#include "GIDeviceVk.h"
#include "GIDeviceObjectVk.h"
#include <assert.h>

namespace AutoCAD::Graphics::Engine
{
    static bool debugMarkerLoaded = false;
    static PFN_vkDebugMarkerSetObjectTagEXT vkDebugMarkerSetObjectTagEXT = VK_NULL_HANDLE;
    static PFN_vkDebugMarkerSetObjectNameEXT vkDebugMarkerSetObjectNameEXT = VK_NULL_HANDLE;

    GIDeviceObjectVk::GIDeviceObjectVk(SharedPtr<GIDeviceVk> device)
        : mDevice(device)
    {
        if (!debugMarkerLoaded)
        {
            vkDebugMarkerSetObjectTagEXT = LOAD_DEVICE_FUNC(*device, vkDebugMarkerSetObjectTagEXT);
            vkDebugMarkerSetObjectNameEXT = LOAD_DEVICE_FUNC(*device, vkDebugMarkerSetObjectNameEXT);
            debugMarkerLoaded = (vkDebugMarkerSetObjectTagEXT != nullptr && vkDebugMarkerSetObjectNameEXT != nullptr);
        }
    }

    GIDeviceObjectVk::~GIDeviceObjectVk()
    {
    }

    void GIDeviceObjectVk::SetDebugNameInternal(void* object, VkDebugReportObjectTypeEXT type, const char* name) const
    {
        if (vkDebugMarkerSetObjectNameEXT)
        {
            VkDebugMarkerObjectNameInfoEXT nameInfo = {};
            nameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_NAME_INFO_EXT;
            nameInfo.objectType = type;
            nameInfo.object = (uint64_t)object;
            nameInfo.pObjectName = name;
            vkDebugMarkerSetObjectNameEXT(*mDevice, &nameInfo);
        }
    }

    void GIDeviceObjectVk::SetDebugTagInternal(void* object, VkDebugReportObjectTypeEXT type, const DebugTag& tag) const
    {
        if (vkDebugMarkerSetObjectTagEXT)
        {
            VkDebugMarkerObjectTagInfoEXT tagInfo = {};
            tagInfo.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_TAG_INFO_EXT;
            tagInfo.objectType = type;
            tagInfo.object = (uint64_t)object;
            tagInfo.tagName = tag.tagName;
            tagInfo.tagSize = tag.tagSize;
            tagInfo.pTag = tag.tag;
            vkDebugMarkerSetObjectTagEXT(*mDevice, &tagInfo);
        }
    }

    SharedPtr<GIDeviceVk> GIDeviceObjectVk::GetDevice() const
    {
        return mDevice;
    }

    GIWeakDeviceObjectVk::GIWeakDeviceObjectVk(WeakPtr<GIDeviceVk> device)
        : mDevice(device)
    {
        if (!debugMarkerLoaded)
        {
            vkDebugMarkerSetObjectTagEXT = LOAD_DEVICE_FUNC(*device.lock(), vkDebugMarkerSetObjectTagEXT);
            vkDebugMarkerSetObjectNameEXT = LOAD_DEVICE_FUNC(*device.lock(), vkDebugMarkerSetObjectNameEXT);
            debugMarkerLoaded = (vkDebugMarkerSetObjectTagEXT != nullptr && vkDebugMarkerSetObjectNameEXT != nullptr);
        }
    }

    GIWeakDeviceObjectVk::~GIWeakDeviceObjectVk()
    {
    }

    void GIWeakDeviceObjectVk::SetDebugNameInternal(void* object, VkDebugReportObjectTypeEXT type, const char* name) const
    {
        if (vkDebugMarkerSetObjectNameEXT)
        {
            VkDebugMarkerObjectNameInfoEXT nameInfo = {};
            nameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_NAME_INFO_EXT;
            nameInfo.objectType = type;
            nameInfo.object = (uint64_t)object;
            nameInfo.pObjectName = name;
            vkDebugMarkerSetObjectNameEXT(*mDevice.lock(), &nameInfo);
        }
    }

    void GIWeakDeviceObjectVk::SetDebugTagInternal(void* object, VkDebugReportObjectTypeEXT type, const DebugTag& tag) const
    {
        if (vkDebugMarkerSetObjectTagEXT)
        {
            VkDebugMarkerObjectTagInfoEXT tagInfo = {};
            tagInfo.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_TAG_INFO_EXT;
            tagInfo.objectType = type;
            tagInfo.object = (uint64_t)object;
            tagInfo.tagName = tag.tagName;
            tagInfo.tagSize = tag.tagSize;
            tagInfo.pTag = tag.tag;
            vkDebugMarkerSetObjectTagEXT(*mDevice.lock(), &tagInfo);
        }
    }

    WeakPtr<GIDeviceVk> GIWeakDeviceObjectVk::GetDevice() const
    {
        return mDevice;
    }
}