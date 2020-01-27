#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <loguru.hpp>

//pickPhysicalDevice();
// createLogicalDevice();

//
//uint32_t deviceCount = 0;
//vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
//
//if (deviceCount == 0) {
//    throw std::runtime_error("failed to find GPUs with Vulkan support!");
//}
//
//std::vector<VkPhysicalDevice> devices(deviceCount);
//vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
//
//for (const auto& device : devices) {
//    if (isDeviceSuitable(device)) {
//        physicalDevice = device;
//        break;
//    }
//}
//
//if (physicalDevice == VK_NULL_HANDLE) {
//    throw std::runtime_error("failed to find a suitable GPU!");
//}




class VulkanPhysicalDeviceHandler
{
private:
	uint32_t gpuCount;
	std::vector<VkPhysicalDevice> gpuList;
	VkInstance instance;

public:
	VulkanPhysicalDeviceHandler(VkInstance instance = nullptr)
	{
		this->instance = instance;
		vkEnumeratePhysicalDevices(instance, &gpuCount, nullptr);
		if (gpuCount > 0)
		{
			gpuList.resize(gpuCount);
			vkEnumeratePhysicalDevices(instance, &gpuCount, gpuList.data());
		}
	}


	VkPhysicalDevice GetDiscrete()
	{
		for (auto d : gpuList)
		{

		}
		return gpuList[0];
	}
};

