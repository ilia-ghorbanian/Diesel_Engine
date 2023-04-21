#pragma once
#include "dsl_device.hpp"
#include "dsl_pipeline.hpp"
#include "dsl_window.hpp"
#include "dsl_swap_chain.hpp"
#include "dsl_model.hpp"

#include <memory>
#include <vector>

namespace dsl {

    class FirstApp {
        public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;
    

    FirstApp();
    ~FirstApp();

    FirstApp(const FirstApp &) = delete;
    FirstApp &operator=(const FirstApp &) = delete;
    
    void run();
    void EventHandlerTest(DslWindow *window);

        private:

        // void sierpinski(
        // std::vector<DslModel::Vertex> &vertices,
        // int depth,
        // glm::vec2 left,
        // glm::vec2 right,
        // glm::vec2 top); 

        void loadModels();
        void createPipelineLayout();
        void createPipeline();
        void createCommandBuffers();
        void drawFrame(DslWindow *dslWindow);
        void recreateSwapchain();
        void recordComandbuffer(int imgIndex);


        DslWindow dslWindow{WIDTH, HEIGHT, "Vulkan Stuff IG!?!"};
        DslDevice dslDevice{dslWindow};
        std::unique_ptr<DslSwapChain> dslSwapChain;
        //DslSwapChain dslSwapChain{dslDevice, dslWindow.getExtent()};
        std::unique_ptr<DslPipeline> dslPipeline;
        VkPipelineLayout pipelineLayout;
        std::vector<VkCommandBuffer> commandBuffers;
        std::unique_ptr<DslModel> dslModel;

        //DslPipeline dslPipeline{dslDevice, "../shaders/simple_shader.vert.spv", "../shaders/simple_shader.frag.spv", DslPipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)};
        
    };

}


