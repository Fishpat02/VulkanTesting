cmake_minimum_required(VERSION 3.24.3)

project(VulkanTest)

# glfw
add_subdirectory(Lib/glfw)

include_directories(Lib/glfw/include)

# glm
add_subdirectory(Lib/glm)

include_directories(Lib/glm/glm)

# vulkan
find_package(Vulkan REQUIRED)

set(VULKAN_DIR ${Vulkan_INCLUDE_DIR})
STRING(REGEX REPLACE "/Include" "" VULKAN_DIR ${VULKAN_DIR})

if(WIN32)
    set(VULKAN_LIBS "vulkan-1")
else()
    set(VULKAN_LIBS "vulkan")
endif()

include_directories(AFTER ${VULKAN_DIR}/Include)
link_directories(${VULKAN_DIR}/Bin;${VULKAN_DIR}/Lib;)

# Add source files for compilation
add_subdirectory(Source)

# Link the libs
target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} glm)
target_link_libraries(${PROJECT_NAME} ${VULKAN_LIBS})