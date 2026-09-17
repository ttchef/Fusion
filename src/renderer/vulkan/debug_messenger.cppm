
module;

#include <volk.h>
#include <renderer/vulkan/check.hpp>

export module fusion.renderer.vulkan.debug_messenger;

import std;
import fusion.renderer.vulkan.instance;
import fusion.renderer.vulkan.utils;

namespace fs::vk
{

static VKAPI_ATTR VkBool32 VKAPI_CALL
debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT      message_severity,
               VkDebugUtilsMessageTypeFlagsEXT             message_type,
               const VkDebugUtilsMessengerCallbackDataEXT *callback_data, void *user_data)
{
    (void)message_severity;
    (void)message_type;
    (void)user_data;

    std::cerr << "[VULKAN] Debug Messenger: %s" << callback_data->pMessage;

    return VK_FALSE;
}
	
export class DebugMessenger
{
	VkDebugUtilsMessengerEXT m_handle;
public:
	DebugMessenger(const Instance &instance)
	{
		VkDebugUtilsMessengerCreateInfoEXT info;
		info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
		                        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
		            		VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
		                	VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		info.pfnUserCallback = debug_callback;

		VK_CHECK(vkCreateDebugUtilsMessengerEXT(instance.handle(), &info, nullptr, &m_handle));
	}

	void destroy(const Instance &instance)
	{
		vkDestroyDebugUtilsMessengerEXT(instance.handle(), m_handle, nullptr);
	}
};
} // namespace fs::vk
