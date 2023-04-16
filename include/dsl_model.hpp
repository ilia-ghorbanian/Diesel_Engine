#pragma once
#include "dsl_device.hpp"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>


namespace dsl{
    class DslModel {
        public:


        struct Vertex{
            glm::vec2 position;
            glm::vec3 color;

            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
        };

        DslModel(DslDevice &device_, const std::vector<Vertex> &vertices);
        ~DslModel();

        DslModel(const DslModel &) = delete;
        DslModel &operator=(const DslModel &) = delete;


        void bind(VkCommandBuffer commandBuffer);
        void draw(VkCommandBuffer commandBuffer);

        
        private:
        void createVertexBuffers(const std::vector<Vertex> &vertices);

        DslDevice &dslDevice;
        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        uint32_t vertexCount;


    };
}