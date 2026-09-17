
export module fusion.window;

import fusion.types;
import fusion.renderer.vulkan.surface;
import fusion.renderer.vulkan.instance;

import std;

namespace fs
{
export class Window
{
public:
	static std::unique_ptr<Window> create(FsU32 width, FsU32 height, const char *title);
	virtual ~Window() = default;
	virtual FsBool should_close() const = 0;
	virtual void poll_events() const = 0;
	virtual std::vector<const char *> get_platform_extensions() const = 0;
	virtual fs::vk::Surface create_surface(const fs::vk::Instance &instance) const = 0;
};
} // namespace fs
