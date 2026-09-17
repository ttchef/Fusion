
export module fusion.window;

import fusion.types;

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

	// TEMP: For testing
	virtual void draw() const = 0;
};
} // namespace fs
