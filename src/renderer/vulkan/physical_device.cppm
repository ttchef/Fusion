
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

constexpr FsU32 GRAPHICS_QUEUE = 0;
constexpr FsU32 PRESENT_QUEUE = 0;

namespace fs::vk
{
FsU32 score_physical_device(VkPhysicalDevice dev)
{
	VkPhysicalDeviceProperties2 props{};
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
	}

	if (graphics_index != INVALID_GPU && present_index != INVALID_GPU)
	{
		get<GRAPHICS_QUEUE>(queue_indices) = graphics_index;
		get<PRESENT_QUEUE>(queue_indices) = present_index;
		return true;
	}

	return false;	
}

class Device;

export class Queue
{
	VkQueue m_handle;
	FsU32 m_index;
public:
	Queue() : m_handle(VK_NULL_HANDLE), m_index(0) {}
	Queue(FsU32 index) : m_handle(VK_NULL_HANDLE), m_index(index) {}

	FsU32 index() const
	{
		return m_index;
	}

	VkQueue handle() const
	{
		return m_handle;
	}

	VkQueue *addr()
	{
		return &m_handle;
	}
};
	
export class PhysicalDevice
{
	VkPhysicalDevice m_handle;

	Queue m_graphics_queue;
	Queue m_present_queue;
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

			m_graphics_queue = Queue(get<GRAPHICS_QUEUE>(queue_indices));
			m_present_queue = Queue(get<PRESENT_QUEUE>(queue_indices));
			
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

	VkPhysicalDevice handle() const noexcept
	{
		return m_handle;
	}

	Queue graphics_queue() const noexcept
	{
		return m_graphics_queue;
	}

	Queue present_queue() const noexcept
	{
		return m_present_queue;
	}
};
} // namespace fs::vk

