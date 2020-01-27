#pragma once
#include <vulkan/vulkan.h>
#include "VulkanHandler.h"
#include <loguru.hpp>




class VulkanExtensionHandler : public VulkanHandler<VkExtensionProperties>
{

private:
	std::vector<VkExtensionProperties>      availableInstanceExtensions;
	std::vector<const char*>				requiredInstanceExtensions;

	uint32_t enabledExtensionCount;
	uint32_t avExtCounter;



public:
	VulkanExtensionHandler()
	{
		avExtCounter = 0;
		enabledExtensionCount = 1;
		vkEnumerateInstanceExtensionProperties(NULL, &avExtCounter, nullptr);
		availableInstanceExtensions.resize(avExtCounter);
		if (avExtCounter > 0) {
			vkEnumerateInstanceExtensionProperties(NULL, &avExtCounter, availableInstanceExtensions.data());
		}
	}

	~VulkanExtensionHandler()
	{

	}

	

	void EnableAllAvailableInstanceExtensions()
	{
		LOG_SCOPE_FUNCTION(INFO);
		requiredInstanceExtensions.clear();
		requiredInstanceExtensions.resize(avExtCounter);

		for (auto e : availableInstanceExtensions)
		{
			requiredInstanceExtensions.push_back(e.extensionName);
		}
		enabledExtensionCount = requiredInstanceExtensions.size();
	}


	bool Push(const char* requirement) override
	{
		if (Validate(requirement)) {
			//We only want requiredInstanceExtensions to have one of which, copies not allowed, but we still return true if they are already inside.
			if (std::find(requiredInstanceExtensions.begin(), requiredInstanceExtensions.end(), requirement) == requiredInstanceExtensions.end())
			{
				requiredInstanceExtensions.push_back(requirement);
				enabledExtensionCount++;
			}
			return true;
		}
		return false;
	}

	uint32_t GetSize() override
	{
		return requiredInstanceExtensions.size();
	}

	const char** GetData() override
	{
		return requiredInstanceExtensions.data();
	}

	bool Validate(const char* requirement) override
	{
		LOG_SCOPE_FUNCTION(INFO);
		LOG_F(INFO, "Available Extensions: (%i/%i) Validating %s...", enabledExtensionCount, avExtCounter, requirement);

		if (avExtCounter > 0)
		{
			for (VkExtensionProperties e : availableInstanceExtensions)
			{
				if (strcmp(e.extensionName, requirement) == 0) 
					return true;
			}
		}
		return false;
	}

};

