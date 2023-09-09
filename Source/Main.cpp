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

#ifndef _DEBUG
const bool EnableValidationLayers = false;
#else
const bool EnableValidationLayers = true;
#endif

VkResult CreateDebugUtilsMessengerEXT (
    VkInstance                                Instance,
    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks*              pAllocator,
    VkDebugUtilsMessengerEXT*                 pDebugMessenger )
{
  auto Func = ( PFN_vkCreateDebugUtilsMessengerEXT ) vkGetInstanceProcAddr (
      Instance,
      "vkCreateDebugUtilsMessengerEXT" );
  if ( Func != nullptr )
    return Func ( Instance, pCreateInfo, pAllocator, pDebugMessenger );
  else
    return VK_ERROR_EXTENSION_NOT_PRESENT;
}

void DestroyDebugUtilsMessengerEXT (
    VkInstance                   Instance,
    VkDebugUtilsMessengerEXT     DebugMessenger,
    const VkAllocationCallbacks* pAllocator )
{
  auto Func = ( PFN_vkDestroyDebugUtilsMessengerEXT ) vkGetInstanceProcAddr (
      Instance,
      "vkDestroyDebugUtilsMessengerEXT" );
  if ( Func != nullptr ) Func ( Instance, DebugMessenger, pAllocator );
}

class HelloTriangleApplication
{
 public:
  void Run ( )
  {
    InitWindow ( );
    CheckValidationLayerSupport ( );
    GetRequiredExtensions ( );
    InitVulkan ( );
    MainLoop ( );
    Cleanup ( );
  }

 private:
  GLFWwindow*              pWindow;
  VkInstance               Instance;
  VkDebugUtilsMessengerEXT DebugMessenger;

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

    VkDebugUtilsMessengerCreateInfoEXT DebugCreateInfo { };

    if ( EnableValidationLayers )
      {
        CreateInfo.enabledLayerCount =
            static_cast< uint32_t > ( ValidationLayers.size ( ) );
        CreateInfo.ppEnabledLayerNames = ValidationLayers.data ( );

        PopulateDebugMessengerCreateInfo ( DebugCreateInfo );
        CreateInfo.pNext =
            ( VkDebugUtilsMessengerCreateInfoEXT* ) &DebugCreateInfo;
      }
    else
      {
        CreateInfo.enabledLayerCount = 0;

        CreateInfo.pNext = nullptr;
      }

    auto Extensions = GetRequiredExtensions ( );
    CreateInfo.enabledExtensionCount =
        static_cast< uint32_t > ( Extensions.size ( ) );
    CreateInfo.ppEnabledExtensionNames = Extensions.data ( );

    uint32_t GLFWExtensionCount = 0;

    vkEnumerateInstanceExtensionProperties (
        nullptr,
        &GLFWExtensionCount,
        nullptr );
    std::vector< VkExtensionProperties > GLFWExtensions ( GLFWExtensionCount );
    vkEnumerateInstanceExtensionProperties (
        nullptr,
        &GLFWExtensionCount,
        GLFWExtensions.data ( ) );

    std::cout << "available extensions (" << GLFWExtensionCount << "):\n";

    for ( const auto& Extension : GLFWExtensions )
      {
        std::cout << '\t' << Extension.extensionName << '\n';
      }

    if ( vkCreateInstance ( &CreateInfo, nullptr, &Instance ) != VK_SUCCESS )
      {
        throw std::runtime_error ( "failed to create instance!" );
      }
  }

  bool CheckValidationLayerSupport ( )
  {
    uint32_t LayerCount = 0;
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

  std::vector< const char* > GetRequiredExtensions ( )
  {
    uint32_t     GLFWExtensionCount = 0;
    const char** ppGLFWExtensions   = nullptr;

    ppGLFWExtensions =
        glfwGetRequiredInstanceExtensions ( &GLFWExtensionCount );

    std::vector< const char* > Extentions (
        ppGLFWExtensions,
        ppGLFWExtensions + GLFWExtensionCount );

    if ( EnableValidationLayers )
      {
        Extentions.push_back ( VK_EXT_DEBUG_UTILS_EXTENSION_NAME );
      }

    return Extentions;
  }

  void InitVulkan ( )
  {
    CreateInstance ( );
    SetupDebugMessenger ( );
  }

  void PopulateDebugMessengerCreateInfo (
      VkDebugUtilsMessengerCreateInfoEXT& CreateInfo )
  {
    CreateInfo       = { };
    CreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    CreateInfo.messageSeverity =
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    CreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    CreateInfo.pfnUserCallback = DebugCallback;
    CreateInfo.pUserData       = nullptr;
  }

  void SetupDebugMessenger ( )
  {
    if ( !EnableValidationLayers ) return;

    VkDebugUtilsMessengerCreateInfoEXT CreateInfo;
    PopulateDebugMessengerCreateInfo ( CreateInfo );

    if ( CreateDebugUtilsMessengerEXT (
             Instance,
             &CreateInfo,
             nullptr,
             &DebugMessenger ) != VK_SUCCESS )
      {
        throw std::runtime_error ( "failed to set up debug messenger!" );
      }
  }

  void MainLoop ( )
  {
    while ( !glfwWindowShouldClose ( pWindow ) ) { glfwPollEvents ( ); }
  }

  void Cleanup ( )
  {
    if ( EnableValidationLayers )
      DestroyDebugUtilsMessengerEXT ( Instance, DebugMessenger, nullptr );

    vkDestroyInstance ( Instance, nullptr );

    glfwDestroyWindow ( pWindow );

    glfwTerminate ( );
  }

  static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback (
      VkDebugUtilsMessageSeverityFlagBitsEXT      MessageSeverity,
      VkDebugUtilsMessageTypeFlagsEXT             MessageType,
      const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
      void*                                       pUserData )
  {
    std::cerr << "Validation layer: " << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
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
