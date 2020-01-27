#pragma once

#include <optional>
#include <vector>
#include <loguru.hpp>
#include <vulkan/vulkan.h>
#include <algorithm>


class VulkanQueueIndex
{
private:
	VkPhysicalDevice	gpu;
	VkSurfaceKHR		surface;
	std::vector<std::optional<uint32_t>> gfxIdxList;
	std::vector<std::optional<uint32_t>> cmpIdxList;
	std::vector<std::optional<uint32_t>> tsfIdxList;
	std::vector<std::optional<uint32_t>> prsIdxList;

	std::vector<VkQueueFamilyProperties> availableFamilyProps;
	uint32_t counter;

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfoList;
	VkDeviceQueueCreateInfo queueCreateInfo = {};
public:
	VulkanQueueIndex(): gpu(nullptr)
	{
		counter = 0;
		VkDeviceQueueCreateInfo queueCreateInfo = {};
	}

	VulkanQueueIndex(VkPhysicalDevice dev, VkSurfaceKHR suf) : gpu(dev), surface(suf)
	{
		counter = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(dev, &counter, nullptr);
		if (counter > 0)
		{
			availableFamilyProps.resize(counter);
			vkGetPhysicalDeviceQueueFamilyProperties(dev, &counter, availableFamilyProps.data());
		}
		VkDeviceQueueCreateInfo queueCreateInfo = {};
	}
	/*
	VK_QUEUE_COMPUTE_BIT = 0x00000002,
	VK_QUEUE_TRANSFER_BIT = 0x00000004,
	VK_QUEUE_SPARSE_BINDING_BIT = 0x00000008,
	VK_QUEUE_PROTECTED_BIT = 0x00000010,
	VK_QUEUE_FLAG_BITS_MAX_ENUM = 0x7FFFFFFF
*/
	bool QuerySupportedFamilies()
	{
		LOG_SCOPE_FUNCTION(INFO);
		uint32_t c = 0;
		for (auto const& e : availableFamilyProps)
		{
			LOG_F(WARNING, "Iterating over index counter %i", c);
			if (e.queueFlags & VK_QUEUE_GRAPHICS_BIT)
				gfxIdxList.push_back(c);
			if (e.queueFlags & VK_QUEUE_TRANSFER_BIT)
				tsfIdxList.push_back(c);
			if (e.queueFlags & VK_QUEUE_COMPUTE_BIT)
				cmpIdxList.push_back(c);

			VkBool32 b = VK_FALSE;

			vkGetPhysicalDeviceSurfaceSupportKHR(gpu, c, surface, &b);
			if (b) {
				prsIdxList.push_back(c);
			}
			c++;
		}

		return gfxIdxList[0].has_value() && prsIdxList[0].has_value();
	}

	void CreateQueueInfoList()
	{
		uint32_t size = 0;// = (uint32_t)(cmpIdxList.size() + prsIdxList.size + tsfIdxList.size() + gfxIdxList.size());
		std::vector<uint32_t> bigIdxList;
		for (auto e : cmpIdxList)
		{
			bigIdxList.push_back(e.value());
		}
		for (auto e : prsIdxList)
		{
			if(std::find(bigIdxList.begin(), bigIdxList.end(), e.value()) != bigIdxList.end())
				bigIdxList.push_back(e.value());
		}
		for (auto e : tsfIdxList)
		{
			if (std::find(bigIdxList.begin(), bigIdxList.end(), e.value()) != bigIdxList.end())
				bigIdxList.push_back(e.value());
		}
		for (auto e : gfxIdxList)
		{
			if (std::find(bigIdxList.begin(), bigIdxList.end(), e.value()) != bigIdxList.end())
				bigIdxList.push_back(e.value());
		}

		for (uint32_t i = 0; i < bigIdxList.size(); i++)
		{
			float prio = 1.0f;
			queueCreateInfo = {};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = bigIdxList[i];
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &prio;
			queueCreateInfoList.push_back(queueCreateInfo);
		}

	}

	std::vector<VkDeviceQueueCreateInfo> GetQueueCreateInfoList()
	{
		return queueCreateInfoList;
	}
};