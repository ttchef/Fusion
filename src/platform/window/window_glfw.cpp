
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

		glfwMakeContextCurrent(m_window);
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
};

std::unique_ptr<Window> Window::create(FsU32 width, FsU32 height, const char *title)
{
	return std::make_unique<WindowGLFW>(width, height, title);
}
} // namespace fs
