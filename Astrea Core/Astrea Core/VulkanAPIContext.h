#pragma once

#include <vulkan/vulkan.h>
#include <loguru.hpp>
#include <Windows.h>

#ifdef NDEBUG
#define DBG
#else
#define DBG LOG_SCOPE_FUNCTION(INFO);
#endif


#include "VulkanInstanceLayerHandler.h"
#include "VKDBGUtility.h"
#include "QueueHandler.h"
#include "VulkanExtensionHandler.h"
#include "VulkanDeviceHandler.h"

struct VulkanDeviceDescriptor
{
    VkInstance              instance;
    VkSurfaceKHR            surface;
    VkPhysicalDevice        physicalDevice; //main one
    VkDevice                device;
};

class VulkanAPIContext
{
public:
    VulkanAPIContext();
    VulkanAPIContext(HWND hInst, bool validation);
    ~VulkanAPIContext();

public:    
    void AttachExtensionList(std::vector<const char*> list);
    void AttachLayerList(std::vector<const char*> list);
    void ForceFullSupport();

    void AttachWindowContext(HWND context)
    {
        this->hWindow = context;
    }

    void CopyDescriptor(VulkanDeviceDescriptor desc);
    VulkanDeviceDescriptor GetDescriptor();

    VkDevice           GetDevice();
    VkInstance         VkGetInstance();
    VkSurfaceKHR       GetSurface();
    VkPhysicalDevice   GetPhysicalDevice();

    VkDevice            InitDevice();
    VkInstance          InitInstance();
    VkSurfaceKHR        InitSurface();
    VkPhysicalDevice    InitPhysicalDevice();

    
private:
    VkDevice                device;
    VkInstance              instance;
    VkSurfaceKHR            surface;
    VulkanPhysDeviceDesc    physicalDeviceDesc;

    HWND                    hWindow;


    std::vector<VkPhysicalDevice> gpuList;

    VulkanInstanceLayerHandler  vlHandler;
    VKDBGUtility                dbgUtility;
    QueueHandler                queueHandler;
    VulkanExtensionHandler      extensionHandler;


    bool isInstanceInit;
    bool isSurfaceInit;
    bool isPhysicalDeviceInit;
    bool isDeviceInit;
};

