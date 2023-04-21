#include "first_app.hpp"
#include <iostream>

#include <stdexcept>
#include <array>




namespace dsl {


    FirstApp::FirstApp(){
        loadModels();
        createPipelineLayout();
        recreateSwapchain();
        createCommandBuffers();
    }

    FirstApp::~FirstApp(){
        vkDestroyPipelineLayout(dslDevice.device(), pipelineLayout, nullptr);
    }

    void FirstApp::run() {

        bool running = true;

        while(running){
            SDL_Event event;
            
            while(SDL_PollEvent(&event)){

            if (event.type == SDL_QUIT){
                running = 0;
                }
            
            };
            drawFrame();
            vkDeviceWaitIdle(dslDevice.device());
        }
    }




    // void FirstApp::sierpinski(
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


    void FirstApp::EventHandlerTest(){
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







    void FirstApp::loadModels() {

        std::vector<DslModel::Vertex>  vertices {
            {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
        };
        // sierpinski(vertices, 5, {-0.5f, 0.5f}, {0.5f, 0.5f}, {0.0f, -0.5f});

        dslModel = std::make_unique<DslModel>(dslDevice, vertices);

    }



    void FirstApp::createPipelineLayout(){

        VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
        pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutCreateInfo.setLayoutCount = 0;
        pipelineLayoutCreateInfo.pSetLayouts = nullptr;
        pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
        pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;

        if(vkCreatePipelineLayout(dslDevice.device(), &pipelineLayoutCreateInfo, nullptr, &pipelineLayout) != VK_SUCCESS){
            throw std::runtime_error("failed to create pipeline layout");
        }
    }


    void FirstApp::createPipeline(){
        auto pipelineConfig = DslPipeline::defaultPipelineConfigInfo(dslSwapChain->width(), dslSwapChain->height());
        pipelineConfig.renderPass = dslSwapChain->getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        dslPipeline = std::make_unique<DslPipeline>(dslDevice, "../shaders/simple_shader.vert.spv", "../shaders/simple_shader.frag.spv", pipelineConfig);
    }

    void FirstApp::recreateSwapchain(){
        auto extent = dslWindow.getExtent();
        while (extent.width == 0 || extent.height == 0){
            extent = dslWindow.getExtent();
            SDL_Event event;
            SDL_WaitEvent(&event);
        }

        vkDeviceWaitIdle(dslDevice.device());
        dslSwapChain = nullptr;
        dslSwapChain = std::make_unique<DslSwapChain>(dslDevice, extent);
        createPipeline();
    }


    void FirstApp::createCommandBuffers(){

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

    void FirstApp::recordCommandbuffer(int imageIndex){
                    VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            if(vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS){

                throw std::runtime_error("failed to begin recording command buffer");
            }

            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = dslSwapChain->getRenderPass();
            renderPassInfo.framebuffer = dslSwapChain->getFrameBuffer(imageIndex);

            renderPassInfo.renderArea.offset = {0,0};
            renderPassInfo.renderArea.extent = dslSwapChain->getSwapChainExtent();

            std::array<VkClearValue, 2> clearValues{};
            clearValues[0].color = {0.1f, 0.1f, 0.1f, 0.1f};
            clearValues[1].depthStencil = {1.0f, 0};


            renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            dslPipeline->bind(commandBuffers[imageIndex]);
            dslModel->bind(commandBuffers[imageIndex]);
            dslModel->draw(commandBuffers[imageIndex]);

            vkCmdEndRenderPass(commandBuffers[imageIndex]);
            if(vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS){
                throw std::runtime_error("failed to record command buffer");
            }
    }

    void FirstApp::drawFrame(){

        uint32_t imageIndex;
        auto result = dslSwapChain->acquireNextImage(&imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR){
            recreateSwapchain();
            return;
        }

    
        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR){
            throw std::runtime_error("failed to acquire swap chain image");
        }
        recordCommandbuffer(imageIndex);
        result = dslSwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || dslWindow.wasWindowResized()){
            dslWindow.resetWindowResizedFlag();
            recreateSwapchain();
            return;
        }
        if (result !=VK_SUCCESS){
            if (result == VK_ERROR_OUT_OF_DATE_KHR){
                std::cout << "lol" << std::endl;
                EventHandlerTest();
            }
            throw std::runtime_error("failed to present swap chain image");
        }
    };

}