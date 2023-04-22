project(VulkanEngine)


add_library(dieselEngine SHARED dsl_window.cpp first_app.cpp dsl_pipeline.cpp dsl_device.cpp dsl_swap_chain.cpp dsl_model.cpp dsl_renderer.cpp)
target_link_libraries(dieselEngine PRIVATE ${Vulkan_LIBS})


target_include_directories(dieselEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)