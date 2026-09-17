
module;

#include <volk.h>
#include <renderer/vulkan/check.hpp>

export module fusion.renderer.vulkan.instance;

import std;
import fusion.types;
import fusion.renderer.vulkan.utils;

namespace fs::vk
{
export enum class ApiVersion : FsU32
{
	ZERO = VK_API_VERSION_1_0,
	ONE = VK_API_VERSION_1_1,
	TWO = VK_API_VERSION_1_2,
	THREE = VK_API_VERSION_1_3,
	FOUR = VK_API_VERSION_1_4,
};
	
export struct InstanceDesc
{
	ApiVersion api_version;

	std::span<const char * const> layers;
	std::span<const char * const> extensions;
};
	
export class Instance
{
	VkInstance m_handle;
public:
	Instance(InstanceDesc desc)
	{
		if (volkInitialize() != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to init volk");
		}
		
		VkApplicationInfo app_info{};
		app_info.pApplicationName = "";
		app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		app_info.pEngineName = "";
		app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		app_info.apiVersion = static_cast<FsU32>(desc.api_version);

		VkInstanceCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		info.pApplicationInfo = &app_info;
		info.enabledExtensionCount = static_cast<FsU32>(desc.extensions.size());
		info.ppEnabledExtensionNames = desc.extensions.data();
		info.enabledLayerCount = static_cast<FsU32>(desc.layers.size());
		info.ppEnabledLayerNames = desc.layers.data();

		VK_CHECK(vkCreateInstance(&info, NULL, &m_handle));

		volkLoadInstance(m_handle);
	}
	~Instance()
	{
		vkDestroyInstance(m_handle, NULL);
	}
};
} // namespace fs::vk
