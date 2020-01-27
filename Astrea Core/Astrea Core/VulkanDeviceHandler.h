#pragma once
#include <vulkan/vulkan.h>
#include <loguru.hpp>
#include <map>


#include "VulkanExtensionHandler.h"
#include "VulkanInstanceLayerHandler.h"
#include "VulkanQueueIndex.h"

#define	NVIDIA  0x10DE
#define	AMD		0x1002
#define	INTEL	0x8086
#define	ARM		0x13B5


static const char* TranslateVendor(unsigned int id) {
	return (id == NVIDIA) ? "NVIDIA" :
		(id == AMD) ? "AMD" :
		(id == INTEL) ? "Intel" :
		(id == ARM) ? "ARM" : "UNKNOWN";
}


struct VulkanPhysDeviceDesc
{
	VkPhysicalDevice device;
	std::pair<VkPhysicalDeviceProperties, VkPhysicalDeviceFeatures> props;
};

class VulkanDeviceHandler
{
private:
	VulkanExtensionHandler					extHandler;

	VkInstance								instance;
	bool									dbgEnabled;

	std::multimap<int, VkPhysicalDevice>	availableGPUMap;
	std::vector<VkPhysicalDevice>			availableGPUList;

	std::pair<VkPhysicalDeviceProperties, VkPhysicalDeviceFeatures> deviceDescriptor;

	uint32_t								availableGPUCntr;
	VkSurfaceKHR							surface;
	bool									isInit;

	VulkanQueueIndex idx;
public:

	VulkanDeviceHandler() : instance(nullptr), dbgEnabled(false)
	{
		vkEnumeratePhysicalDevices(instance, &availableGPUCntr, nullptr);
		if (availableGPUCntr > 0)
		{
			LOG_F(INFO, "&i GPUs found", availableGPUCntr);
			availableGPUList.resize(availableGPUCntr);
			vkEnumeratePhysicalDevices(instance, &availableGPUCntr, availableGPUList.data());
		}
		else
			LOG_F(ERROR, "No GPUs with Vulkan Suppor found!");

		isInit = false;

	}

	VulkanDeviceHandler(VkInstance instance, VkSurfaceKHR suf, bool dbgEnabled) : instance(instance), surface(suf), dbgEnabled(dbgEnabled)
	{
		vkEnumeratePhysicalDevices(instance, &availableGPUCntr, nullptr);
		if (availableGPUCntr > 0)
		{
			LOG_F(INFO, "%i GPUs found", availableGPUCntr);
			availableGPUList.resize(availableGPUCntr);
			vkEnumeratePhysicalDevices(instance, &availableGPUCntr, availableGPUList.data());
		}
		else
			LOG_F(ERROR, "No GPUs with Vulkan Suppor found!");
		isInit = false;
	}

	void AttachExtensionHandler(VulkanExtensionHandler hdnl)
	{
		this->extHandler = hdnl;
	}

	bool IsInit()
	{
		return isInit;
	}
	VulkanQueueIndex GetQueueIndex()
	{
		return idx;
	}
	VkPhysicalDevice PickDevice()
	{
		LOG_F(INFO, "-- GPU Selection Loop --");

		for (auto dev : availableGPUList)
		{
			VkPhysicalDeviceProperties deviceProperties;
			VkPhysicalDeviceFeatures deviceFeatures;

			vkGetPhysicalDeviceProperties(dev, &deviceProperties);
			vkGetPhysicalDeviceFeatures(dev, &deviceFeatures);

			idx = VulkanQueueIndex(dev, surface);
			if (idx.QuerySupportedFamilies()) {
				deviceDescriptor.first = deviceProperties;
				deviceDescriptor.second = deviceFeatures;
				LOG_F(INFO, "GPU selected. ");
				idx.CreateQueueInfoList();
				isInit = true;
				return dev;
			}

		}
		LOG_F(INFO, "Falling back to available GPU.");

		if (availableGPUCntr == 1)
		{
			isInit = true;
			return availableGPUList[0];
		}
		else
		{
			isInit = false;
			return nullptr;
		}
	}



	std::pair<VkPhysicalDeviceProperties, VkPhysicalDeviceFeatures> GetDeviceDescriptor()
	{
		return deviceDescriptor;
	}
};

