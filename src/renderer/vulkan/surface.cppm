
module;

#include <volk.h>
#include <renderer/vulkan/check.hpp>

export module fusion.renderer.vulkan.surface;

import fusion.renderer.vulkan.instance;

namespace fs::vk
{
export class Surface
{
	VkSurfaceKHR m_handle;

public:
	void destroy(const Instance &instance)
	{
		vkDestroySurfaceKHR(instance.handle(), m_handle, nullptr);
	}
	
	VkSurfaceKHR handle() const noexcept
	{
		return m_handle;
	}

	VkSurfaceKHR *addr() noexcept
	{
		return &m_handle;
	}
};
} // namespace fs::vk
