#include "dsl_renderer.hpp"
#include <iostream>


#include <stdexcept>
#include <array>




namespace dsl {



    DslRenderer::DslRenderer(DslWindow &window, DslDevice &device) : dslWindow{window}, dslDevice{device} {

        recreateSwapchain();
        createCommandBuffers();
    }

    DslRenderer::~DslRenderer(){
        freeCommandBuffers();
    }





    // void DslRenderer::sierpinski(
    //     std::vector<DslModel::Vertex> &vertices,
    //     int depth,
    //     glm::vec2 left,
    //     glm::vec2 right,
    //     glm::vec2 top) {
    //         if (depth <= 0) {
    //             vertices.push_back({top});
    //             vertices.push_back({right});
    //             vertices.push_back({left});
    //         }else {
    //             auto leftTop = 0.5f * (left + top);
    //             auto rightTop = 0.5f * (right + top);
    //             auto leftRight = 0.5f * (left + right);
    //             sierpinski(vertices, depth - 1, left, leftRight, leftTop);
    //             sierpinski(vertices, depth - 1, leftRight, right, rightTop);
    //             sierpinski(vertices, depth - 1, leftTop, rightTop, top);
    // };
    //     }


    void DslRenderer::EventHandlerTest(){
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type){
                case SDL_EventType::SDL_WINDOWEVENT:
                    if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED){
                        std::cout << "test worked ig?" << std::endl;
                    };
            }
        }
    }




    void DslRenderer::recreateSwapchain(){
        auto extent = dslWindow.getExtent();
        while (extent.width == 0 || extent.height == 0){
            extent = dslWindow.getExtent();
            SDL_Event event;
            SDL_WaitEvent(&event);
        }

        vkDeviceWaitIdle(dslDevice.device());
        if(dslSwapChain == nullptr){
        //dslSwapChain = nullptr;
        dslSwapChain = std::make_unique<DslSwapChain>(dslDevice, extent);
        }else{
            dslSwapChain = std::make_unique<DslSwapChain>(dslDevice, extent, std::move(dslSwapChain));

            if(dslSwapChain->imageCount() != commandBuffers.size()){
                freeCommandBuffers();
                createCommandBuffers();
            }
        }
        
        //TODO COME BACK TO
    }


    void DslRenderer::createCommandBuffers(){

        commandBuffers.resize(dslSwapChain->imageCount());
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = dslDevice.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if (vkAllocateCommandBuffers(dslDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS){
            throw std::runtime_error("failed to allocate command buffers");
        }
    }


    void DslRenderer::freeCommandBuffers(){
        vkFreeCommandBuffers(dslDevice.device(), dslDevice.getCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
        commandBuffers.clear();
    }




    VkCommandBuffer DslRenderer::beginFrame(){
        assert(!isFrameStarted && "Cant call begin frame while already in progress");
        
        auto result = dslSwapChain->acquireNextImage(&currentImageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR){
            recreateSwapchain();
            return nullptr;
        }

    
        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR){
            throw std::runtime_error("failed to acquire swap chain image");
        }
        isFrameStarted = true;

        auto commandBuffer = getCurrentCommandBuffer();

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if(vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS){

            throw std::runtime_error("failed to begin recording command buffer");
        }
        return commandBuffer;
    };


    void DslRenderer::endFrame(){
        assert(isFrameStarted && "Cant call endFrame while is not in progress");
        auto commandBuffer = getCurrentCommandBuffer();
        if(vkEndCommandBuffer(commandBuffer) != VK_SUCCESS){
                throw std::runtime_error("failed to record command buffer");
            }

        auto result = dslSwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || dslWindow.wasWindowResized()){
            EventHandlerTest();
            dslWindow.resetWindowResizedFlag();
            recreateSwapchain();
            //return;
        }
        else if (result !=VK_SUCCESS){
            if (result == VK_ERROR_OUT_OF_DATE_KHR){
                std::cout << "lol" << std::endl;
                
            }
            int w,h;
            SDL_GetWindowSize(dslWindow.getWindow(), &w, &h);
            throw std::runtime_error("failed to present swap chain image");
        }

        isFrameStarted = false;
    };


    void DslRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer){
        assert(isFrameStarted && "Cant call beginSwapChainRenderPass while is not in progress");
        assert(commandBuffer == getCurrentCommandBuffer() && "Cant begin RenderPass on commandBuffer from a different frame");

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = dslSwapChain->getRenderPass();
        renderPassInfo.framebuffer = dslSwapChain->getFrameBuffer(currentImageIndex);

        renderPassInfo.renderArea.offset = {0,0};
        renderPassInfo.renderArea.extent = dslSwapChain->getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0};


        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);


        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(dslSwapChain->getSwapChainExtent().width);
        viewport.height = static_cast<float>(dslSwapChain->getSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor{{0,0}, dslSwapChain->getSwapChainExtent()};
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
    }


    void DslRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer){
        assert(isFrameStarted && "Cant call endSwapChainRenderPass while is not in progress");
        assert(commandBuffer == getCurrentCommandBuffer() && "Cant end RenderPass on commandBuffer from a different frame");
        vkCmdEndRenderPass(commandBuffer);
    };









}