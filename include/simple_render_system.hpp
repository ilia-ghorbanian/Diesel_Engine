#pragma once

#include "dsl_device.hpp"
#include "dsl_pipeline.hpp"
//#include "dsl_model.hpp"
#include "dsl_game_object.hpp"

#include <memory>
#include <vector>

namespace dsl {

    class SimpleRenderSystem {
        public:

    

    SimpleRenderSystem(DslDevice &device, VkRenderPass renderPass);
    ~SimpleRenderSystem();

    SimpleRenderSystem(const SimpleRenderSystem &) = delete;
    SimpleRenderSystem &operator=(const SimpleRenderSystem &) = delete;

    void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<DslGameObject> &gameObjects);
    

        private:

        // void sierpinski(
        // std::vector<DslModel::Vertex> &vertices,
        // int depth,
        // glm::vec2 left,
        // glm::vec2 right,
        // glm::vec2 top); 

        void createPipelineLayout();
        void createPipeline(VkRenderPass renderPass);
        


        DslDevice &dslDevice;
        // DslSwapChain dslSwapChain{dslDevice, dslWindow.getExtent()};

        std::unique_ptr<DslPipeline> dslPipeline;
        VkPipelineLayout pipelineLayout;

        //DslPipeline dslPipeline{dslDevice, "../shaders/simple_shader.vert.spv", "../shaders/simple_shader.frag.spv", DslPipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)};
        
    };

}


