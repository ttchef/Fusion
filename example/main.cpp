
import fusion;
import std;

int main()
{
	try
	{
		std::unique_ptr<fs::Window> window = fs::Window::create(100, 100, "Wsp");

		std::vector<const char *> instance_extensions = window->get_platform_extensions();
		std::vector<const char *> instance_layers{};

		// Add debug messenger
#if defined(DEBUG)
		instance_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		instance_layers.push_back("VK_LAYER_KHRONOS_validation");
#endif

		fs::vk::InstanceDesc instance_desc = {
			.api_version = fs::vk::ApiVersion::THREE,
			.extensions = instance_extensions,
		};
		fs::vk::Instance instance{instance_desc};

		while (!window->should_close())
		{
			window->poll_events();		
		}
	} catch (std::runtime_error &e)
	{
		std::cerr << "Error: " << e.what() << std::endl;		
	}
	catch (...)
	{
		std::cerr << "Unkown error" << std::endl;
	}

	return 0;
}
