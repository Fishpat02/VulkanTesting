#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <stdexcept>
#include <cstdlib>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

class HelloTriangleApplication
{
public:
	void Run ( )
	{
		InitWindow ( );
		CreateInstance ( );
		InitVulkan ( );
		MainLoop ( );
		Cleanup ( );
	}

private:
	GLFWwindow* Window;
	VkInstance Instance;

	void InitWindow ( )
	{
		glfwInit ( );

		glfwWindowHint ( GLFW_CLIENT_API, GLFW_NO_API );
		glfwWindowHint ( GLFW_RESIZABLE, GLFW_FALSE );

		Window = glfwCreateWindow ( WIDTH, HEIGHT, "Vulkan", nullptr, nullptr );
	}

	void CreateInstance ( )
	{
		VkApplicationInfo AppInfo {};
		AppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		AppInfo.pApplicationName = "Hello Triangle";
		AppInfo.applicationVersion = VK_MAKE_VERSION ( 1, 0, 0 );
		AppInfo.pEngineName = "No Engine";
		AppInfo.engineVersion = VK_MAKE_VERSION ( 1, 0, 0 );
		AppInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo CreateInfo {};
		CreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		CreateInfo.pApplicationInfo = &AppInfo;

		uint32_t GLFWExtensionCount = 0;
		const char** GLFWExtensions = nullptr;

		vkEnumerateInstanceExtensionProperties ( nullptr, &GLFWExtensionCount, nullptr );
		std::vector<VkExtensionProperties> Extensions ( GLFWExtensionCount );
		vkEnumerateInstanceExtensionProperties ( nullptr, &GLFWExtensionCount, Extensions.data ( ) );

		std::cout << "available extensions (" << GLFWExtensionCount << "):\n";

		for ( const auto& Extension : Extensions )
		{
			std::cout << '\t' << Extension.extensionName << '\n';
		}

		GLFWExtensions = glfwGetRequiredInstanceExtensions ( &GLFWExtensionCount );

		CreateInfo.enabledExtensionCount = GLFWExtensionCount;
		CreateInfo.ppEnabledExtensionNames = GLFWExtensions;

		CreateInfo.enabledLayerCount = 0;

		if ( vkCreateInstance ( &CreateInfo, nullptr, &Instance ) != VK_SUCCESS )
		{
			throw std::runtime_error ( "failed to create instance!" );
		}

	}

	void InitVulkan ( )
	{

	}

	void MainLoop ( )
	{
		while ( !glfwWindowShouldClose ( Window ) )
		{
			glfwPollEvents ( );
		}
	}

	void Cleanup ( )
	{
		vkDestroyInstance ( Instance, nullptr );

		glfwDestroyWindow ( Window );

		glfwTerminate ( );

	}
};

int main ( )
{
	HelloTriangleApplication App{};

	try
	{
		App.Run ( );
	} catch ( const std::exception& e )
	{
		std::cerr << e.what ( ) << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
