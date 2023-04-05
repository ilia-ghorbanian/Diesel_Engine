project(VulkanEngine)

add_library(dieselEngine dsl_window.cpp first_app.cpp)

target_include_directories(dieselEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)