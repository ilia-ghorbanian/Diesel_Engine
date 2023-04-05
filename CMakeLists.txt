cmake_minimum_required(VERSION 3.0.0)
project(VulkanEngine VERSION 0.1.0)

include(CTest)
enable_testing()

find_package(Vulkan REQUIRED)
find_package(SDL2 REQUIRED)



include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include)

include_directories( ${Vulkan_INCLUDE_DIRS})


include_directories( ${SDL2_INCLUDE_DIRS})
 



add_executable(VulkanEngine src/main.cpp)

target_link_libraries(VulkanEngine ${Vulkan_LIBS})
target_link_libraries(VulkanEngine ${SDL2_LIBRARIES})



set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
