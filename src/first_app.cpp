#include "first_app.hpp"
#include <iostream>

#include <stdexcept>
#include <array>




namespace dsl {


    FirstApp::FirstApp(){
        loadModels();
        createPipelineLayout();
        createPipeline();
        createCommandBuffers();
    }

    FirstApp::~FirstApp(){
        vkDestroyPipelineLayout(dslDevice.device(), pipelineLayout, nullptr);
    }



    bool FirstApp::IsCursorOverWindowBorder(SDL_Window* window, int x, int y) {
    int windowWidth, windowHeight;
    int borderLeft, borderRight, borderTop, borderBottom;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
    SDL_GetWindowBordersSize(window, &borderTop, &borderLeft, &borderBottom, &borderRight);
    int left = borderLeft;
    int right = windowWidth + borderLeft;
    int top = borderTop;
    int bottom = windowHeight + borderTop;

    if (x >= left && x < right && y >= top && y < bottom) {
        return false;
    }

    return true;
}
    void FirstApp::run() {

        bool running = true;
        bool resizing = false;

        while(running){
            SDL_Event event;
            
            while(SDL_PollEvent(&event)){
                switch (event.type)
                {
                case SDL_QUIT:
                    running = false;
                    break;
                
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        SDL_CaptureMouse(SDL_TRUE);
                        int x = event.button.x;
                        int y = event.button.y;
                        if (IsCursorOverWindowBorder(dslWindow.getWindow(), x, y)) {
                            resizing = true;
                            std::cout << "on border AngelThump\n";
                            SDL_CaptureMouse(SDL_TRUE);
                        }
                    }
                
                
                
                    // if (event.button.button == SDL_BUTTON_LEFT) {
                    //     // Check if cursor is over window client area
                    //     int mouseX, mouseY;
                    //     Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);
                    //     int windowX, windowY, windowW, windowH;
                    //     SDL_GetWindowPosition(dslWindow.getWindow(), &windowX, &windowY);
                    //     SDL_GetWindowSize(dslWindow.getWindow(), &windowW, &windowH);
                    //     int borderWidth, borderHeight;
                        
                    //     SDL_GetWindowBordersSize(dslWindow.getWindow(), &borderWidth, &borderHeight, NULL, NULL);
                    //     std::cout << "here ig\n";
                    //     if ((mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) &&
                    //         (mouseX >= windowX && mouseX < (windowX + windowW - borderWidth)) &&
                    //         (mouseY >= windowY && mouseY < (windowY + windowH - borderHeight))) {
                    //         // Cursor is over window client area
                    //         resizing = true;
                    //         SDL_CaptureMouse(SDL_TRUE); // Capture mouse events
                    //         std::cout << "grabbing border?\n";
                    //     }
                    // }
                    break;

                case SDL_MOUSEBUTTONUP:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        // Check if we were resizing the window
                        if (resizing) {
                            resizing = false;
                            SDL_CaptureMouse(SDL_FALSE); // Release mouse events
                            SDL_Delay(16); // Delay to avoid potential race condition
                            
                        }
                    }

                case SDL_MOUSEMOTION:
                    if (resizing) {
                        // Handle window resize event
                        int width = event.motion.x;
                        int height = event.motion.y;
                        std::cout << "i'm mOOOOving OOOO\n";
                        // Update swap chain and rendering pipeline here
                    }
                
                default:
                    break;
                }
                if(!resizing){
                    drawFrame();
                }else{
                    // Resizing in progress, pause rendering temporarily
                    SDL_Delay(16);
                    vkDeviceWaitIdle(dslDevice.device());
                }

            //     if(event.type == SDL_WINDOWEVENT){
            //         if(event.window.event == SDL_WINDOWEVENT_RESIZED){

            //     std::cout << "_________________window resized IG AAAA _________________\n";
            //     std::cout << "_________________window resized IG AAAA _________________\n";
            //     std::cout << "_________________window resized IG AAAA _________________\n";
            //     std::cout << "_________________window resized IG AAAA _________________\n";
            //         }
            // }
            //     drawFrame();

            // if (event.type == SDL_QUIT){
            //     running = 0;
            //     }
            
            // };
            
        }
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
        auto pipelineConfig = DslPipeline::defaultPipelineConfigInfo(dslSwapChain.width(), dslSwapChain.height());
        pipelineConfig.renderPass = dslSwapChain.getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        dslPipeline = std::make_unique<DslPipeline>(dslDevice, "../shaders/simple_shader.vert.spv", "../shaders/simple_shader.frag.spv", pipelineConfig);
    }


    void FirstApp::createCommandBuffers(){

        commandBuffers.resize(dslSwapChain.imageCount());
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = dslDevice.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if (vkAllocateCommandBuffers(dslDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS){
            throw std::runtime_error("failed to allocate command buffers");
        }

        for (int i = 0; i < commandBuffers.size(); i++){
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            if(vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS){

                throw std::runtime_error("failed to begin recording command buffer");
            }

            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = dslSwapChain.getRenderPass();
            renderPassInfo.framebuffer = dslSwapChain.getFrameBuffer(i);

            renderPassInfo.renderArea.offset = {0,0};
            renderPassInfo.renderArea.extent = dslSwapChain.getSwapChainExtent();

            std::array<VkClearValue, 2> clearValues{};
            clearValues[0].color = {0.1f, 0.1f, 0.1f, 0.1f};
            clearValues[1].depthStencil = {1.0f, 0};


            renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            dslPipeline->bind(commandBuffers[i]);
            dslModel->bind(commandBuffers[i]);
            dslModel->draw(commandBuffers[i]);

            vkCmdEndRenderPass(commandBuffers[i]);
            if(vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS){
                throw std::runtime_error("failed to record command buffer");
            }
        }

    }


    void FirstApp::drawFrame(){

        uint32_t imageIndex;
        auto result = dslSwapChain.acquireNextImage(&imageIndex);

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR){
            throw std::runtime_error("failed to acquire swap chain image");
        }
        result = dslSwapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
        if (result !=VK_SUCCESS){
            if (result == VK_ERROR_OUT_OF_DATE_KHR){
                std::cout << "lol" << std::endl;
                EventHandlerTest();
            }
            throw std::runtime_error("failed to present swap chain image");
        }
    };

}