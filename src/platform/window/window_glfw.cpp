
module;

#include <GLFW/glfw3.h>

module fusion.window;

namespace fs
{
class WindowGLFW : public Window
{
	GLFWwindow *m_window;
public:
	WindowGLFW(FsU32 width, FsU32 height, const char *title)
	{
		// NOTE: Move out in a sec
		if (!glfwInit())
		{
			// idk
		}
		m_window = glfwCreateWindow(width, height, title, NULL, NULL);
		if (!m_window)
		{
			// idk
		}

		if (!glfwVulkanSupported())
		{
			throw std::runtime_error("Vulkan isnt supported");
		}
	}
	~WindowGLFW()
	{
		if (m_window)
		{
			glfwDestroyWindow(m_window);
		}
	}
	FsBool should_close() const override
	{
		return glfwWindowShouldClose(m_window);
	}
	void poll_events() const override
	{
		glfwPollEvents();
	}
	std::vector<const char *> get_platform_extensions() const override
	{
		FsU32 count;
		const char **extensions = glfwGetRequiredInstanceExtensions(&count);

		if (!extensions)
		{
			throw std::runtime_error("Failed to retrive required vulkan instance extensions");
		}

		return std::vector<const char *>(extensions, extensions + count);
	}
};

std::unique_ptr<Window> Window::create(FsU32 width, FsU32 height, const char *title)
{
	return std::make_unique<WindowGLFW>(width, height, title);
}
} // namespace fs
