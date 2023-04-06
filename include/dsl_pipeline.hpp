#pragma once

#include "dsl_device.hpp"

#include <vector>
#include <string>
namespace dsl
{

    struct PipelineConfigInfo {};
    class DslPipeline {
        public:
            DslPipeline(DslDevice &device ,const std::string &vertFilepath, const std::string &fragFilepath, const PipelineConfigInfo &configInfo);


            ~DslPipeline(){};
            DslPipeline(const DslPipeline&) = delete;
            void operator=(const DslPipeline&) = delete;

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
