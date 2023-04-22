#pragma once
#include "dsl_device.hpp"
#include "dsl_window.hpp"
#include "dsl_swap_chain.hpp"
//#include "dsl_model.hpp"

#include <memory>
#include <vector>
#include <cassert>

namespace dsl {

    class DslRenderer {
        public:
    

    DslRenderer(DslWindow &window, DslDevice &device);
    ~DslRenderer();

    DslRenderer(const DslRenderer &) = delete;
    DslRenderer &operator=(const DslRenderer &) = delete;

    VkRenderPass getSwapChainRenderPass() const { return dslSwapChain->getRenderPass();}
    bool isFrameInProgress() const {return isFrameStarted;}

    VkCommandBuffer getCurrentCommandBuffer() const{
        assert(isFrameStarted && "Cannot get command buffer when frame is not in progress");
        return commandBuffers[currentFrameIndex];
    }


    int getFrameIndex() const {
        assert(isFrameStarted && "Cannot get frame index when frame is not in progress");
        return currentFrameIndex;
    }
    VkCommandBuffer beginFrame();
    void endFrame();
    void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
    void endSwapChainRenderPass(VkCommandBuffer commandBuffer);



    
    void EventHandlerTest();

        private:

        // void sierpinski(
        // std::vector<DslModel::Vertex> &vertices,
        // int depth,
        // glm::vec2 left,
        // glm::vec2 right,
        // glm::vec2 top); 


        void createCommandBuffers();
        void freeCommandBuffers();
        void recreateSwapchain();



        DslWindow &dslWindow;
        DslDevice &dslDevice;
        // DslSwapChain dslSwapChain{dslDevice, dslWindow.getExtent()};
        std::unique_ptr<DslSwapChain> dslSwapChain;
        std::vector<VkCommandBuffer> commandBuffers;

        uint32_t currentImageIndex;
        int currentFrameIndex;
        bool isFrameStarted = false;

        //DslPipeline dslPipeline{dslDevice, "../shaders/simple_shader.vert.spv", "../shaders/simple_shader.frag.spv", DslPipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)};
        
    };

}


