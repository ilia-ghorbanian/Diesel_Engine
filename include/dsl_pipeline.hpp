#pragma once

#include "dsl_device.hpp"

#include <string>

#include <vector>
namespace dsl
{

    struct PipelineConfigInfo {
        VkViewport viewport;
        VkRect2D scissor;
        
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo colorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
        VkPipelineLayout pipelineLayout = nullptr;
        VkRenderPass renderPass = nullptr;
        uint32_t subpass = 0;
    };


    class DslPipeline {
        public:
            DslPipeline(DslDevice &device ,const std::string &vertFilepath, const std::string &fragFilepath, const PipelineConfigInfo &configInfo);


            ~DslPipeline();
            DslPipeline(const DslPipeline&) = delete;
            DslPipeline& operator=(const DslPipeline&) = delete;


            void bind(VkCommandBuffer commandBuffer);


            static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);

        private:
            static std::vector<char> readFile(const std::string &filepath);

            void createGraphicsPipeline(const std::string &vertFilepath, const std::string &fragFilepath, const PipelineConfigInfo &configInfo);
    
    
            void createShaderModule(const std::vector<char> &code, VkShaderModule*shaderModule);

            DslDevice &dslDevice;
            VkPipeline graphicsPipeline;
            VkShaderModule vertShaderModule;
            VkShaderModule fragShaderModule;
    };
} // namespace dsl
