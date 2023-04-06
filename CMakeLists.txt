cmake_minimum_required(VERSION 3.0.0)
project(VulkanEngine VERSION 0.1.0)

include(CTest)
enable_testing()

find_package(Vulkan REQUIRED)
find_package(SDL2 REQUIRED)




add_subdirectory(include/)
add_subdirectory(src/)

include_directories( ${Vulkan_INCLUDE_DIRS})


include_directories( ${SDL2_INCLUDE_DIRS})
 



add_executable(main app/main.cpp)



target_link_libraries(main SDL2 vulkan)
target_link_libraries(main dieselEngine)



set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
