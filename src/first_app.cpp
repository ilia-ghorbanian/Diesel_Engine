#include "first_app.hpp"
#include <iostream>

#include <stdexcept>



namespace dsl {


    FirstApp::FirstApp(){
        createPipelineLayout();
        createPipeline();
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
        }
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

    }


    void FirstApp::drawFrame(){};

}