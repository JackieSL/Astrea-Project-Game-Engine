#pragma once

#include <vector>
#include "VulkanHandler.h"
#ifndef VULKAN_H_
#include <vulkan/vulkan.h>
#endif


class VulkanInstanceLayerHandler : public VulkanHandler<VkLayerProperties>
{
private:
	std::vector<VkLayerProperties>	availableList;
	std::vector<const char*>		requirementList;

	uint32_t counter;
	uint32_t reqCounter;
public:

	VulkanInstanceLayerHandler()
	{

		vkEnumerateInstanceLayerProperties(&counter, nullptr);
		availableList.resize(counter);
		reqCounter = 1;
		if (counter > 0)
		{
			vkEnumerateInstanceLayerProperties(&counter, availableList.data());
		}
	}

	void EnableAllAvailableLayerExtensions()
	{
		requirementList.clear();
		requirementList.resize(counter);
		for (auto e : availableList)
		{
			requirementList.push_back(e.layerName);
		}
	}

	bool Validate(const char* requirement) override
	{
		LOG_SCOPE_FUNCTION(INFO);
		LOG_F(INFO, "Validating Layer %s(%i/%i available)", requirement, reqCounter,counter);
		if (counter > 0)
		{
			for (auto e : availableList)
			{
				if (strcmp(e.layerName, requirement) == 0)
					return true;
			}
		}
		return false;

	}

	uint32_t GetSize() override
	{
		return requirementList.size();
	}

	const char** GetData() override
	{
		return requirementList.data();
	}


    bool Push(const char* requirement) override
    {
		if (Validate(requirement)) {
			//We only want requiredInstanceExtensions to have one of which, copies not allowed, but we still return true if they are already inside.
			if (std::find(requirementList.begin(), requirementList.end(), requirement) == requirementList.end()) {
				requirementList.push_back(requirement);
				reqCounter++;
			}

			return true;
		}
		return false;
    }
};
