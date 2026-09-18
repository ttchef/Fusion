
module;

#include <volk.h>
#include <renderer/vulkan/check.hpp>

export module fusion.renderer.vulkan.device;

import fusion.renderer.vulkan.physical_device;
import fusion.renderer.vulkan.utils;
import fusion.types;
import std;

namespace fs::vk
{
export class Device
{
	VkDevice m_handle;
public:
	Device(PhysicalDevice &physical_device)
	{
		FsF32 priority = 1.0f;

		VkDeviceQueueCreateInfo queue_infos[2];
		FsU32 queue_count = 0;
		queue_infos[queue_count++] = {
			.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
			.queueFamilyIndex = physical_device.graphics_queue().index(),
			.queueCount = 1,
			.pQueuePriorities = &priority,	
		};

		if (physical_device.graphics_queue().index() != physical_device.present_queue().index())
		{
			queue_infos[queue_count++] = {
				.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
				.queueFamilyIndex = physical_device.present_queue().index(),
				.queueCount = 1,
				.pQueuePriorities = &priority,	
			};
		}

		std::vector<const char *> device_extensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME,	
		};

		VkDeviceCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		info.queueCreateInfoCount = queue_count;
		info.pQueueCreateInfos = queue_infos;
		info.enabledExtensionCount = device_extensions.size();
		info.ppEnabledExtensionNames =  device_extensions.data();

		VK_CHECK(vkCreateDevice(physical_device.handle(), &info, nullptr, &m_handle));

		vkGetDeviceQueue(m_handle, physical_device.graphics_queue().index(), 0, physical_device.graphics_queue().addr());
		vkGetDeviceQueue(m_handle, physical_device.present_queue().index(), 0, physical_device.present_queue().addr());
	}

	void destroy()
	{
		vkDestroyDevice(m_handle, nullptr);	
	}

	VkDevice handle() const noexcept
	{
		return m_handle;
	}
};
} // namespace fs::vk
