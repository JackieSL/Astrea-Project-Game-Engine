#include "VulkanAPIContext.h"
#include <stdexcept>
#include <vulkan/vulkan_win32.h>
#include <VulkanPhysicalDeviceHandler.h>

VulkanAPIContext::VulkanAPIContext()
{
	DBG
	dbgUtility.Enabled		= false;
	isPhysicalDeviceInit	= false;
	isDeviceInit			= false;
	isSurfaceInit			= false;
	isInstanceInit			= false;
}

VulkanAPIContext::VulkanAPIContext(HWND hInst, bool validation)
{
	DBG
	dbgUtility.Enabled = validation;
	this->hWindow = hInst;
	isPhysicalDeviceInit	= false;
	isDeviceInit			= false;
	isSurfaceInit			= false;
	isInstanceInit			= false;

}

VulkanAPIContext::~VulkanAPIContext()
{
	if (isSurfaceInit)
		vkDestroySurfaceKHR(instance, surface, nullptr);
	if (dbgUtility.Enabled);
		dbgUtility.~VKDBGUtility();
	if(isInstanceInit)
		vkDestroyInstance(instance, nullptr);
}

VkInstance VulkanAPIContext::InitInstance()
{
        VkInstanceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = nullptr;

		LOG_F(WARNING, "ExtHandler contains %i extensions", extensionHandler.GetSize());
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensionHandler.GetSize());
		createInfo.ppEnabledExtensionNames = extensionHandler.GetData();

        if (dbgUtility.Enabled) {
			LOG_F(WARNING, "VLHandler contains %i layers", vlHandler.GetSize());

			

            createInfo.enabledLayerCount = static_cast<uint32_t>(vlHandler.GetSize());
            createInfo.ppEnabledLayerNames = vlHandler.GetData();
            createInfo.pNext = dbgUtility.GetCreateStruct();
        }
        else {
            createInfo.enabledLayerCount = 0;
            createInfo.pNext = nullptr;
        }

        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) 
            throw std::runtime_error("failed to create instance!");
		else
			isInstanceInit = true;

		dbgUtility.CreateDebugUtil(instance, nullptr);
		return (isInstanceInit) ? instance : nullptr;
}

VkSurfaceKHR VulkanAPIContext::InitSurface()
{
	DBG
	VkWin32SurfaceCreateInfoKHR hints;
	hints.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	hints.hwnd = hWindow;
	hints.flags = 0;
	hints.pNext = nullptr;
	hints.hinstance = nullptr;

	if (vkCreateWin32SurfaceKHR(instance, &hints, nullptr, &surface) != VK_SUCCESS)
		LOG_F(ERROR, "Failed to create Surface.");
	else
	{
		LOG_F(WARNING, "Surface created successfully.");
		isSurfaceInit = isInstanceInit && true;
	}

	return (isSurfaceInit) ? surface : VkSurfaceKHR();
}

VkDevice VulkanAPIContext::InitDevice()
{
	if (isSurfaceInit && isInstanceInit) {
		VulkanDeviceHandler h(instance, surface, true);

		physicalDeviceDesc = {};
		physicalDeviceDesc.device = h.PickDevice();
		physicalDeviceDesc.props = h.GetDeviceDescriptor();

		isPhysicalDeviceInit = h.IsInit();

		if (isPhysicalDeviceInit)
		{
			//Init Logical Device
			device = VK_NULL_HANDLE;
			VkDeviceCreateInfo devCreateInfo = {};

			devCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
			devCreateInfo.enabledLayerCount = static_cast<uint32_t>(vlHandler.GetSize());
			devCreateInfo.ppEnabledLayerNames = vlHandler.GetData();
			std::vector<const char*> deviceExtensions;
			deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
			devCreateInfo.enabledExtensionCount = (uint32_t)1; //static_cast<uint32_t>(extensionHandler.GetSize());
			devCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
			devCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(h.GetQueueIndex().GetQueueCreateInfoList().size());
			devCreateInfo.pQueueCreateInfos = h.GetQueueIndex().GetQueueCreateInfoList().data();

			if (vkCreateDevice(physicalDeviceDesc.device, &devCreateInfo, nullptr, &device) == VK_ERROR_TOO_MANY_OBJECTS) {
				LOG_F(ERROR, "asdsadsadsad");
			}
		}
	}
	return device;
}



void VulkanAPIContext::AttachExtensionList(std::vector<const char*> list)
{
	LOG_SCOPE_FUNCTION(INFO);
	for (auto e : list)
	{
		if (!extensionHandler.Push(e))
			LOG_F(ERROR, "Extension: %s is unavailable on Host", e);
	}
}

void VulkanAPIContext::AttachLayerList(std::vector<const char*> list)
{
	LOG_SCOPE_FUNCTION(INFO);
	for (auto str : list)
	{
		if(!vlHandler.Push(str))
			LOG_F(ERROR, "Failed to attach Instance Layer: %s is unavailable on Host", str);
	}
}
//TODO: Broken
void VulkanAPIContext::ForceFullSupport()
{
	vlHandler.EnableAllAvailableLayerExtensions();
	extensionHandler.EnableAllAvailableInstanceExtensions();
}

void VulkanAPIContext::CopyDescriptor(VulkanDeviceDescriptor desc)
{
	this->physicalDeviceDesc.device = desc.physicalDevice;
	this->surface					= desc.surface;
	this->instance					= desc.instance;
}

VulkanDeviceDescriptor VulkanAPIContext::GetDescriptor()
{
	VulkanDeviceDescriptor desc;
	desc.device = device;
	desc.instance = instance;
	desc.physicalDevice = physicalDeviceDesc.device;
	desc.surface = surface;
	return desc;
}		


VkInstance VulkanAPIContext::VkGetInstance()
{
	return instance;
}

VkSurfaceKHR VulkanAPIContext::GetSurface()
{
	return surface;
}














VkPhysicalDevice VulkanAPIContext::InitPhysicalDevice()
{
	return VkPhysicalDevice();
}
