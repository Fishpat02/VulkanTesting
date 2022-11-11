#include <vulkan/vulkan_core.h>

#include <cstdint>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <vector>

const uint32_t WIDTH  = 800;
const uint32_t HEIGHT = 600;

const std::vector< const char* > ValidationLayers = {
  "VK_LAYER_KHRONOS_validation"
};

#ifndef NDBEBUG
const bool EnableValidationLayers = false;
#else
const bool EnableValidationLayers = true;
#endif

class HelloTriangleApplication
{
 public:
  void Run ( )
  {
    InitWindow ( );
    CreateInstance ( );
    CheckValidationLayerSupport ( );
    InitVulkan ( );
    MainLoop ( );
    Cleanup ( );
  }

 private:
  GLFWwindow* pWindow;
  VkInstance  Instance;

  void InitWindow ( )
  {
    glfwInit ( );

    glfwWindowHint ( GLFW_CLIENT_API, GLFW_NO_API );
    glfwWindowHint ( GLFW_RESIZABLE, GLFW_FALSE );

    pWindow = glfwCreateWindow ( WIDTH, HEIGHT, "Vulkan", nullptr, nullptr );
  }

  void CreateInstance ( )
  {
    if ( EnableValidationLayers && !CheckValidationLayerSupport ( ) )
      {
        throw std::runtime_error (
            "Validation layers requested, but not available!" );
      }

    VkApplicationInfo AppInfo { };
    AppInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    AppInfo.pApplicationName   = "Hello Triangle";
    AppInfo.applicationVersion = VK_MAKE_VERSION ( 1, 0, 0 );
    AppInfo.pEngineName        = "No Engine";
    AppInfo.engineVersion      = VK_MAKE_VERSION ( 1, 0, 0 );
    AppInfo.apiVersion         = VK_API_VERSION_1_0;

    VkInstanceCreateInfo CreateInfo { };
    CreateInfo.sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    CreateInfo.pApplicationInfo = &AppInfo;

    if ( EnableValidationLayers )
      {
        CreateInfo.enabledLayerCount =
            static_cast< uint32_t > ( ValidationLayers.size ( ) );
        CreateInfo.ppEnabledLayerNames = ValidationLayers.data ( );
      }
    else { CreateInfo.enabledLayerCount = 0; }

    uint32_t     GLFWExtensionCount = 0;
    const char** ppGLFWExtensions   = nullptr;

    vkEnumerateInstanceExtensionProperties (
        nullptr,
        &GLFWExtensionCount,
        nullptr );
    std::vector< VkExtensionProperties > Extensions ( GLFWExtensionCount );
    vkEnumerateInstanceExtensionProperties (
        nullptr,
        &GLFWExtensionCount,
        Extensions.data ( ) );

    std::cout << "available extensions (" << GLFWExtensionCount << "):\n";

    for ( const auto& Extension : Extensions )
      {
        std::cout << '\t' << Extension.extensionName << '\n';
      }

    ppGLFWExtensions =
        glfwGetRequiredInstanceExtensions ( &GLFWExtensionCount );

    CreateInfo.enabledExtensionCount   = GLFWExtensionCount;
    CreateInfo.ppEnabledExtensionNames = ppGLFWExtensions;

    CreateInfo.enabledLayerCount = 0;

    if ( vkCreateInstance ( &CreateInfo, nullptr, &Instance ) != VK_SUCCESS )
      {
        throw std::runtime_error ( "failed to create instance!" );
      }
  }

  bool CheckValidationLayerSupport ( )
  {
    uint32_t LayerCount;
    vkEnumerateInstanceLayerProperties ( &LayerCount, nullptr );

    std::vector< VkLayerProperties > AvailableLayers ( LayerCount );
    vkEnumerateInstanceLayerProperties (
        &LayerCount,
        AvailableLayers.data ( ) );

    for ( const char* LayerName : ValidationLayers )
      {
        bool LayerFound = false;

        for ( const auto& LayerProperties : AvailableLayers )
          {
            if ( strcmp ( LayerName, LayerProperties.layerName ) == 0 )
              {
                LayerFound = true;
                break;
              }
          }

        if ( !LayerFound ) { return false; }
      }

    return true;
  }

  void InitVulkan ( ) {}

  void MainLoop ( )
  {
    while ( !glfwWindowShouldClose ( pWindow ) ) { glfwPollEvents ( ); }
  }

  void Cleanup ( )
  {
    vkDestroyInstance ( Instance, nullptr );

    glfwDestroyWindow ( pWindow );

    glfwTerminate ( );
  }
};

int main ( )
{
  HelloTriangleApplication App { };

  try
    {
      App.Run ( );
    }
  catch ( const std::exception& e )
    {
      std::cerr << e.what ( ) << std::endl;
      return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
