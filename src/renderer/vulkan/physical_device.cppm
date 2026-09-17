
module;

#include <volk.h>
#include <renderer/vulkan/check.hpp>

export module fusion.renderer.vulkan.physical_device;

import fusion.renderer.vulkan.instance;
import fusion.renderer.vulkan.utils;
import fusion.renderer.vulkan.surface;
import fusion.types;
import std;

constexpr FsI32 INVALID_GPU = -1;
constexpr FsI32 INVALID_QUEUE = -1;

namespace fs::vk
{
FsU32 score_physical_device(VkPhysicalDevice dev)
{
	VkPhysicalDeviceProperties2 props;
	props.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
	
	vkGetPhysicalDeviceProperties2(dev, &props);
	FsU32 score = 0;
	
	// Device type
	switch (props.properties.deviceType)
	{
	case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU: 
	{
		score += 100;
	} break;
	case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU: 
	{
		score += 50;
	} break;
	case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU: 
	{
		score += 25;
	} break;
	case VK_PHYSICAL_DEVICE_TYPE_CPU: 
	{
		score += 1;
	} break;
	default: break;
	}

	return score;
}

bool is_physical_device_supported(VkPhysicalDevice dev, VkSurfaceKHR surface, std::tuple<FsU32, FsU32> &queue_indices)
{
	FsU32 queue_count;
	vkGetPhysicalDeviceQueueFamilyProperties2(dev, &queue_count, nullptr);

	std::vector<VkQueueFamilyProperties2> props{queue_count};
	vkGetPhysicalDeviceQueueFamilyProperties2(dev, &queue_count, props.data());

	FsI32 graphics_index = INVALID_QUEUE;
	FsI32 present_index = INVALID_QUEUE;
	
	for (FsUsize i = 0; i < props.size(); i++)
	{
		const VkQueueFamilyProperties &prop = props[i].queueFamilyProperties;

		if (prop.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			graphics_index = i;
		}

		VkBool32 present_support = VK_FALSE;
		vkGetPhysicalDeviceSurfaceSupportKHR(dev, i, surface, &present_support);

		if (present_support)
		{
			present_index = i;
			if (graphics_index != INVALID_QUEUE)
			{
				break;
			}
		}

		if (graphics_index != INVALID_GPU && present_index != INVALID_GPU)
		{
			
		}
	}

	return true;	
}

export class Queue
{
	VkQueue handle;
	FsU32 index;
};
	
export class PhysicalDevice
{
	VkPhysicalDevice m_handle;
	Queue graphics_queue;

public:
	PhysicalDevice(const Instance &instance, const Surface &surface)
	{
		FsU32 device_count;
		VK_CHECK(vkEnumeratePhysicalDevices(instance.handle(), &device_count, nullptr));

		std::vector<VkPhysicalDevice> devices{device_count};
		VK_CHECK(vkEnumeratePhysicalDevices(instance.handle(), &device_count, devices.data()));

		FsI32 gpu_to_pick = INVALID_GPU;
		FsU32 highscore = 0;

		for (FsUsize i = 0; i < devices.size(); i++)
		{
			VkPhysicalDevice &dev = devices[i];

			std::tuple<FsU32, FsU32> queue_indices;
			if (!is_physical_device_supported(dev, surface.handle(), queue_indices))
			{
				continue;
			}
			
			FsU32 score = score_physical_device(dev);

			if (highscore < score)
			{
				gpu_to_pick = i;
				highscore = score;	
			}
		}

		if (gpu_to_pick == INVALID_GPU)
		{
			throw std::runtime_error("No viable gpu found");	
		}

		m_handle = devices.at(gpu_to_pick);
	}
};
} // namespace fs::vk

