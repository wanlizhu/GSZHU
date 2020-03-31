#include "GIInstanceVk.h"

namespace AutoCAD::Graphics::Engine
{
    static PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT = nullptr;
    static PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT = nullptr;

    // VK_EXT_debug_report is officially a deprecated extension, so going forward, you should use VK_EXT_debug_utils.
    //static PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT = nullptr;
    //static PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallbackEXT = nullptr;


    VKAPI_ATTR VkBool32 VKAPI_CALL DebugMessengerCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageTypes,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData
    )
    {
        std::string prefix;
        if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
        {
            prefix = "VERBOSE: ";
        }
        if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        {
            prefix = "WARNING: ";
        }
        if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
        {
            prefix = "ERROR: ";
        }

        // Display message to default output (console/logcat)
        std::stringstream debugMsg;
        debugMsg << prefix << "[" << pCallbackData->messageIdNumber
            << "][" << pCallbackData->pMessageIdName
            << "] : " << pCallbackData->pMessage;

        if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
        {
            LOG_ERROR("%s", debugMsg.str().c_str());
        }
        else
        {
#ifdef _DEBUG
            if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
            {
                LOG_DEBUG("%s", debugMsg.str().c_str());
            }
            else if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
            {
                LOG_DEBUG("%s", debugMsg.str().c_str());
            }
#endif
        }

        return VK_TRUE;
    }

    VKAPI_ATTR VkBool32 VKAPI_CALL DebugReportCallback(
        VkDebugReportFlagsEXT flags,
        VkDebugReportObjectTypeEXT objectType,
        uint64_t object,
        size_t location,
        int32_t messageCode,
        const char* pLayerPrefix,
        const char* pMessage,
        void* pUserData
    )
    {
        if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
        {
            LOG_ERROR("ERROR: %s", pMessage);
        }
        else
        {
#ifdef _DEBUG
            if ((flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) ||
                (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT))
            {
                LOG_DEBUG("WARNING: %s", pMessage);
            }
            else if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT)
            {
                LOG_DEBUG("INFO: %s", pMessage);
            }
#endif
        }

        return VK_TRUE;
    }

    bool GIInstanceVk::IsLayerSupported(const char* name)
    {
        static std::vector<VkLayerProperties> supportedLayers;
        
        if (supportedLayers.empty())
        {
            uint32_t layerCount = 0;
            vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
            if (layerCount)
            {
                supportedLayers.resize(layerCount);
                vkEnumerateInstanceLayerProperties(&layerCount, supportedLayers.data());
            }
        }

        return std::find_if(
            supportedLayers.begin(),
            supportedLayers.end(),
            [&](const VkLayerProperties& layer) {
                return strcmp(layer.layerName, name) == 0;
            }) != supportedLayers.end();
    }

    bool GIInstanceVk::IsExtensionSupported(const char* name)
    {
        static std::vector<VkExtensionProperties> supportedExtensions;

        if (supportedExtensions.empty())
        {
            uint32_t extensionCount = 0;
            vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
            if (extensionCount)
            {
                supportedExtensions.resize(extensionCount);
                vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, supportedExtensions.data());
            }
        }

        return std::find_if(
            supportedExtensions.begin(),
            supportedExtensions.end(),
            [&](const VkExtensionProperties& layer) {
                return strcmp(layer.extensionName, name) == 0;
            }) != supportedExtensions.end();
    }

    GIInstanceVk::~GIInstanceVk()
    {
        if (IsValid())
        {
            if (mDebugUtilsHandle)
            {
                vkDestroyDebugUtilsMessengerEXT(mInstanceHandle, mDebugUtilsHandle, nullptr);
                mDebugUtilsHandle = VK_NULL_HANDLE;
            }

            vkDestroyInstance(mInstanceHandle, nullptr);
            mInstanceHandle = VK_NULL_HANDLE;
        }
    }

    GIInstanceVk::operator const VkInstance& () const
    {
        return mInstanceHandle;
    }

    bool GIInstanceVk::IsValid() const
    {
        return mInstanceHandle != VK_NULL_HANDLE;
    }

    bool GIInstanceVk::IsValidationLayerEnabled() const
    {
        return mDebugUtilsHandle != VK_NULL_HANDLE;
    }

    bool GIInstanceVk::IsLayerEnabled(const char* name) const
    {
        return std::find_if(
            mEnabledLayers.begin(),
            mEnabledLayers.end(),
            [&](const char* item) {
                return strcmp(item, name) == 0;
            }) != mEnabledLayers.end();
    }

    bool GIInstanceVk::IsExtensionEnabled(const char* name) const
    {
        return std::find_if(
            mEnabledExtensions.begin(),
            mEnabledExtensions.end(),
            [&](const char* item) {
                return strcmp(item, name) == 0;
            }) != mEnabledExtensions.end();
    }

    uint32_t GIInstanceVk::GetPhysicalDeviceCount() const
    {
        uint32_t count = 0;
        vkEnumeratePhysicalDevices(mInstanceHandle, &count, nullptr);
        return count;
    }

    VkPhysicalDevice GIInstanceVk::FirstPhysicalDevice() const
    {
        uint32_t count = GetPhysicalDeviceCount();
        std::vector<VkPhysicalDevice> devices(count);
        vkEnumeratePhysicalDevices(mInstanceHandle, &count, devices.data());
        return devices[0];
    }

    bool FindDeviceExtensions(VkPhysicalDevice device, const std::vector<const char*>& extensionsMUST)
    {
        uint32_t extensionCount = 0;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> extensionProps(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, extensionProps.data());

        for (const auto& name : extensionsMUST)
        {
            if (std::find_if(
                extensionProps.begin(),
                extensionProps.end(),
                [&](const VkExtensionProperties& item) {
                    return strcmp(item.extensionName, name) == 0;
                }) == extensionProps.end())
            {
                return false;
            }
        }

        return true;
    }

    bool FindDeviceFeatures(VkPhysicalDevice device, const VkPhysicalDeviceFeatures& features)
    {
        static VkPhysicalDeviceFeatures emptyFeatures = {};
        if (memcmp(&features, &emptyFeatures, sizeof(VkPhysicalDeviceFeatures)) == 0)
            return true;

        VkPhysicalDeviceFeatures supportedFeatures = {};
        vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

        size_t count = sizeof(VkPhysicalDeviceFeatures) / sizeof(VkBool32);
        VkBool32* src = (VkBool32*)& supportedFeatures;
        VkBool32* dst = (VkBool32*)& features;

        for (size_t i = 0; i < count; i++)
        {
            if (*(dst + i) == VK_TRUE && *(src + i) == VK_FALSE)
            {
                return false;
            }
        }

        return true;
    }

    bool FindDeviceQueues(VkPhysicalDevice device, VkQueueFlags queues)
    {
        uint32_t familyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &familyCount, nullptr);
        std::vector<VkQueueFamilyProperties> queueProps(familyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &familyCount, queueProps.data());

        VkQueueFlags supportedFlags = 0;

        for (const auto& queue : queueProps)
        {
            supportedFlags |= queue.queueFlags;
        }

        return supportedFlags & queues;
    }

    long ScorePhysicalDevice(VkPhysicalDevice device, const std::vector<const char*>& extensionsOPTL)
    {
        long score = 0;

        VkPhysicalDeviceProperties deviceProps = {};
        vkGetPhysicalDeviceProperties(device, &deviceProps);
        if (deviceProps.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
            score += 1000;
        }

        uint32_t extensionCount = 0;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> extensionProps(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, extensionProps.data());

        for (const auto& name : extensionsOPTL)
        {
            if (std::find_if(
                extensionProps.begin(),
                extensionProps.end(),
                [&](const VkExtensionProperties& item) {
                    return strcmp(item.extensionName, name) == 0;
                }) != extensionProps.end())
            {
                score += 100;
            }
        }

        score += deviceProps.limits.maxImageDimension2D;

        return score;
    }

    void LogPhysicalDeviceInfo(VkPhysicalDevice device)
    {
        VkPhysicalDeviceProperties props = {};
        vkGetPhysicalDeviceProperties(device, &props);

        std::string msg = "Current Physical Device:\n";
        switch ((int)props.deviceType)
        {
        case 1:
            msg += "\tType: Integrated GPU\n";
            break;
        case 2:
            msg += "\tType: Discrete GPU\n";
            break;
        case 3:
            msg += "\tType: Virtual GPU\n";
            break;
        case 4:
            msg += "\tType: CPU\n";
            break;
        default:
            msg += "\tType: Other\n";
        }

        msg += "\tDevice ID: " + std::to_string(props.deviceID) + "\n";

        switch (props.vendorID)
        {
        case 0x8086:
            msg += "\tVendor ID: \"Intel\"\n";
            break;
        case 0x10DE:
            msg += "\tVendor ID: \"Nvidia\"\n";
            break;
        case 0x1002:
            msg += "\tVendor ID: \"AMD\"\n";
            break;
        default:
            msg += "\tVendor ID: \"" + std::to_string(props.vendorID) + "\"\n";
        }

        msg += "\tDevice Name: " + std::string(props.deviceName) + "\n";
        uint32_t supportedVersion[3] = {
            VK_VERSION_MAJOR(props.apiVersion),
            VK_VERSION_MINOR(props.apiVersion),
            VK_VERSION_PATCH(props.apiVersion)
        };
        msg += "\tAPI Version: "
            + std::to_string(supportedVersion[0])
            + "." + std::to_string(supportedVersion[1])
            + "." + std::to_string(supportedVersion[2])
            + "\n";

        LOG_DEBUG(msg.c_str());
    }

    VkPhysicalDevice GIInstanceVk::FindPhysicalDeviceBy(
        const std::vector<const char*>& extensions,
        const std::vector<const char*>& extensionsOptional,
        VkQueueFlags queueFlags,
        const VkPhysicalDeviceFeatures& features)
    {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(mInstanceHandle, &deviceCount, nullptr);
        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(mInstanceHandle, &deviceCount, devices.data());

        // Find adequate devices
        std::vector<uint32_t> adequateDevices;
        for (uint32_t i = 0; i < devices.size(); i++)
        {
            if (FindDeviceExtensions(devices[i], extensions) &&
                FindDeviceFeatures(devices[i], features) &&
                FindDeviceQueues(devices[i], queueFlags))
            {
                adequateDevices.push_back(i);
            }
        }

        // Find the best one if there are multiple adequate devices
        uint32_t index = adequateDevices.empty() ? UINT_MAX : adequateDevices[0];
        long bestScore = -1;
        if (adequateDevices.size() > 1)
        {
            for (uint32_t i : adequateDevices)
            {
                long currentScore = ScorePhysicalDevice(devices[i], extensionsOptional);
                if (bestScore < currentScore)
                {
                    bestScore = currentScore;
                    index = i;
                }
            }
        }

        // No adequate found
        if (index == UINT_MAX)
        {
            LOG_ERROR("Failed to find adequate physical device.\n");
            return VK_NULL_HANDLE;
        }

        mChosenPhysicalDeviceHandle = devices[index];

#ifdef _DEBUG
        LogPhysicalDeviceInfo(mChosenPhysicalDeviceHandle);
#endif

        return mChosenPhysicalDeviceHandle;
    }

    VkPhysicalDevice GIInstanceVk::GetChosenGPU() const
    {
        return mChosenPhysicalDeviceHandle;
    }

    EGPUVendorID GIInstanceVk::GetChosenGPUVendorID() const
    {
        assert(mChosenPhysicalDeviceHandle != 0);
        VkPhysicalDeviceProperties props = {};
        vkGetPhysicalDeviceProperties(mChosenPhysicalDeviceHandle, &props);
        return static_cast<EGPUVendorID>(props.vendorID);
    }

    GIInstanceVk::GIInstanceVk(
        const VkInstanceCreateInfo& createInfo,
        const std::vector<const char*>& enabledLayers,
        const std::vector<const char*>& enabledExtensions
    )
        : mEnabledLayers(enabledLayers)
        , mEnabledExtensions(enabledExtensions)
    {
        VK_CHECK(vkCreateInstance(&createInfo, nullptr, &mInstanceHandle));

        if (createInfo.enabledLayerCount > 0)
        {
            if (GIInstanceVk::IsExtensionSupported(VK_EXT_DEBUG_UTILS_EXTENSION_NAME))
            {
                vkCreateDebugUtilsMessengerEXT = LOAD_INSTANCE_FUNC(mInstanceHandle, vkCreateDebugUtilsMessengerEXT);
                vkDestroyDebugUtilsMessengerEXT = LOAD_INSTANCE_FUNC(mInstanceHandle, vkDestroyDebugUtilsMessengerEXT);
                if (vkCreateDebugUtilsMessengerEXT == nullptr || vkDestroyDebugUtilsMessengerEXT == nullptr)
                {
                    LOG_DEBUG("vk*DebugUtilsMessengerEXT functions are missing, fallback to VkDebugReportCallbackEXT");
                }
                else
                {
                    VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
                    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
                    createInfo.pNext = nullptr;
                    createInfo.flags = 0;
                    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;;
                    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;;
                    createInfo.pfnUserCallback = DebugMessengerCallback;
                    createInfo.pUserData = this;
                    VK_CHECK(vkCreateDebugUtilsMessengerEXT(mInstanceHandle, &createInfo, nullptr, &mDebugUtilsHandle));
                }
            }
        }
    }

    GIInstanceBuilderVk::GIInstanceBuilderVk()
    {
        mAppInfo = {};
        mAppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        mAppInfo.pNext = nullptr;
        mAppInfo.apiVersion = VK_API_VERSION_1_2;
        mAppInfo.pApplicationName = "AutoCAD";
        mAppInfo.pEngineName = "AcadDM";
        
        mCreateInfo = {};
        mCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        mCreateInfo.pNext = nullptr;
        mCreateInfo.pApplicationInfo = &mAppInfo;
    }

    GIInstanceBuilderVk& GIInstanceBuilderVk::SetAppName(const char* name)
    {
        mAppInfo.pApplicationName = name;
        return *this;
    }

    GIInstanceBuilderVk& GIInstanceBuilderVk::SetEngineName(const char* name)
    {
        mAppInfo.pEngineName = name;
        return *this;
    }

    GIInstanceBuilderVk& GIInstanceBuilderVk::SetApiVersion(uint32_t version)
    {
        mAppInfo.apiVersion = version;
        return *this;
    }

    GIInstanceBuilderVk& GIInstanceBuilderVk::EnableValidationLayers(bool value)
    {
        mEnableValidationLayers = value;
        return *this;
    }

    GIInstanceBuilderVk& GIInstanceBuilderVk::EnableLayers(const std::vector<const char*>& names)
    {
        for (const auto& name : names)
        {
            EnableLayer(name, true);
        }
        return *this;
    }

    GIInstanceBuilderVk& GIInstanceBuilderVk::EnableLayer(const char* name, bool compulsory)
    {
        if (GIInstanceVk::IsLayerSupported(name))
        {
            mEnabledLayers.push_back(name);
        }
        else
        {
            if (compulsory)
                LOG_ERROR("Compulsory layer '%s' is not supported.\n", name);
            else
                LOG_WARNING("Optional layer '%s' is not supported.\n", name);
        }

        return *this;
    }

    GIInstanceBuilderVk& GIInstanceBuilderVk::EnableExtension(const char* name, bool compulsory)
    {
        if (GIInstanceVk::IsExtensionSupported(name))
        {
            mEnabledExtensions.push_back(name);
        }
        else
        {
            if (compulsory)
                LOG_ERROR("Compulsory extension '%s' is not supported.\n", name);
            else
                LOG_WARNING("Optional extension '%s' is not supported.\n", name);
        }

        return *this;
    }

    GIInstanceBuilderVk& GIInstanceBuilderVk::EnableExtensions(const std::vector<const char*>& names)
    {
        for (const auto& name : names)
        {
            EnableExtension(name, true);
        }
        return *this;
    }

    SharedPtr<GIInstanceVk> GIInstanceBuilderVk::Build()
    {
        if (mEnableValidationLayers)
        {
            mCreateInfo.enabledLayerCount = (uint32_t)mEnabledLayers.size();
            mCreateInfo.ppEnabledLayerNames = mEnabledLayers.data();
        }
        mCreateInfo.enabledExtensionCount = (uint32_t)mEnabledExtensions.size();
        mCreateInfo.ppEnabledExtensionNames = mEnabledExtensions.data();

        auto instance = SharedPtr<GIInstanceVk>(new GIInstanceVk(
            mCreateInfo, 
            mEnabledLayers,
            mEnabledExtensions));
        assert(instance->IsValid());

        return instance;
    }
}