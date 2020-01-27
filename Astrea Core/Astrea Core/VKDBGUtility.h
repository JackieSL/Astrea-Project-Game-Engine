#pragma once

#include <vulkan/vulkan.h>
#include <loguru.hpp>

#ifdef NDEBUG
#define DBG
#else
#define DBG LOG_SCOPE_FUNCTION(INFO);
#endif

class VKDBGUtility
{
private:    
    VkDebugUtilsMessengerEXT           debugMessenger;
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
    VkInstance                         instance;
public:
    bool Enabled;

    VKDBGUtility()
    {
        instance = 0;
        debugCreateInfo = {};
        debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        debugCreateInfo.pfnUserCallback = debugCallback;

    }
    VKDBGUtility(bool enabled) : Enabled(enabled)
    {
        instance = 0;
        debugCreateInfo = {};
        debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        debugCreateInfo.pfnUserCallback = debugCallback;
    }

    ~VKDBGUtility()
    {
        DestroyDebugUtil(instance, debugMessenger, nullptr);
    }

    VkDebugUtilsMessengerCreateInfoEXT* GetCreateStruct()
    {
        DBG
        return &debugCreateInfo;
    }

    VkResult CreateDebugUtil(VkInstance instance, const VkAllocationCallbacks* pAllocator) {
        
        DBG
        this->instance = instance;
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        LOG_F(ERROR, (func == nullptr) ? "Callback is null" : "Callback Defined succesfully");
        if (func != nullptr) {
            LOG_F(INFO, "IF VALIDATED");
            return func(instance, &debugCreateInfo, pAllocator, &debugMessenger);
        }
        else {
            LOG_F(INFO, "IF NOT VALIDATED, returning VK_ERROR_EXTENSIONS_NOT_PRESENT");
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    void DestroyDebugUtil(VkInstance instance, VkDebugUtilsMessengerEXT messenger, const VkAllocationCallbacks* pAllocator) {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr) {
            func(instance, messenger, pAllocator);
        }
    }

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData)
    {


        loguru::NamedVerbosity sev;
        sev = (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) ? loguru::NamedVerbosity::Verbosity_ERROR :
            (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) ? loguru::NamedVerbosity::Verbosity_WARNING :
            (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) ? loguru::NamedVerbosity::Verbosity_INFO : loguru::NamedVerbosity::Verbosity_INVALID;

        const char* type = (messageType == VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT) ? "General" :
            (messageType == VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT) ? "Validation" :
            (messageType == VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT) ? "Performance" : "Undefined";
        if (sev != loguru::NamedVerbosity::Verbosity_INFO)
        {
            loguru::log(sev, __FILE__, __LINE__, "--Vulkan Callback Error of Type %s:--", type);
            loguru::log(sev, __FILE__, __LINE__, "%s", pCallbackData->pMessage);
            loguru::log(sev, __FILE__, __LINE__, "--End of Message--");
        }
        return VK_FALSE;
    }

};



