#include "dsl_pipeline.hpp"

#include <fstream>
#include <stdexcept>
#include <iostream>

namespace dsl
{

    DslPipeline::DslPipeline(const std::string &vertFilepath, const std::string &fragFilepath){
        createGraphicsPipeline(vertFilepath, fragFilepath);
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


    void DslPipeline::createGraphicsPipeline(const std::string &vertFilepath, const std::string &fragFilepath){

        auto vertCode = readFile(vertFilepath);
        auto fragCode = readFile(fragFilepath);

        std::cout << "Vertex Shader Code Size: " << vertCode.size() << '\n';
        std::cout << "Fragment Shader Code Size: " << fragCode.size() << '\n';


    }
} // namespace dsl
