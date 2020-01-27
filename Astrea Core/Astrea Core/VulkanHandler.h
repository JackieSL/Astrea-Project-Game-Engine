#pragma once
#include <vector>
#include <loguru.hpp>
template<class T>
class VulkanHandler
{

private:
	std::vector<T>				availableList;
	std::vector<const char*>	requirementList;

	uint32_t					avExtCounter;
public:
	VulkanHandler<T>()
	{
		avExtCounter = 0;
	}

	virtual ~VulkanHandler<T>()
	{

	}

	virtual std::vector<const char*> GetList()
	{
		return requirementList;
	}

	virtual const char** GetData()
	{
		return nullptr;
	}

	virtual bool Validate(const char* requirement)
	{
		LOG_SCOPE_FUNCTION(INFO);
		LOG_F(WARNING, "You shouldn't be calling the super function of VulkanHandler!");

		if (avExtCounter > 0)
		{
			for (T e : availableList)
			{

			}
		}
		return false;

	}

	virtual uint32_t GetSize()
	{
		return 0;
	}

	virtual bool Push(const char* item)
	{

		if (Validate(item)) {
			//We only want requiredInstanceExtensions to have one of which, copies not allowed, but we still return true if they are already inside.
			if (std::find(requirementList.begin(), requirementList.end(), item) == requirementList.end())
				requirementList.push_back(item);
			return true;
		}
		return false;
	}


};