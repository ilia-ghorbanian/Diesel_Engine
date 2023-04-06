#include "dsl_pipeline.hpp"

#include <fstream>
#include <stdexcept>
#include <iostream>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>


namespace dsl
{

    DslPipeline::DslPipeline(DslDevice &device ,const std::string &vertFilepath, const std::string &fragFilepath, const PipelineConfigInfo &configInfo) : dslDevice{device}{
        createGraphicsPipeline(vertFilepath, fragFilepath, configInfo);
    }

    std::vector<char> DslPipeline::readFile(const std::string &filepath) {
        //load file and move to the end immedietly
        std::ifstream file(filepath, std::ios::ate | std::ios::binary);


        //check if file was successfully opened
        if(!file.is_open()){
            throw std::runtime_error("Failed to open file: " + filepath);
        }
        //get the size of the file. we're already at the end of the file, so tellg helps us get the last position (which is the file size)
        size_t fileSize = static_cast<size_t> (file.tellg());

        //initilize a vector with the size of the file
        std::vector<char> buffer(fileSize);

        //seek start of the file to actually read the data
        file.seekg(0); 
        //read the data to "buffer.data"
        file.read(buffer.data(), fileSize);

        file.close();
        return buffer;
    }


    void DslPipeline::createGraphicsPipeline(const std::string &vertFilepath, const std::string &fragFilepath, const PipelineConfigInfo &configInfo){

        auto vertCode = readFile(vertFilepath);
        auto fragCode = readFile(fragFilepath);

        std::cout << "Vertex Shader Code Size: " << vertCode.size() << '\n';
        std::cout << "Fragment Shader Code Size: " << fragCode.size() << '\n';


    }

    void DslPipeline::createShaderModule(const std::vector<char> &code, VkShaderModule*shaderModule){
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        if(vkCreateShaderModule(dslDevice.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS){
            throw std::runtime_error("Failed to create shader module");
        }
    }
        

    PipelineConfigInfo DslPipeline::defaultPipelineConfigInfo(uint32_t width, uint32_t height){

        PipelineConfigInfo configInfo;

        return configInfo;

    }



} // namespace dsl
