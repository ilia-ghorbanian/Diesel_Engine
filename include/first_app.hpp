#pragma once

#include "dsl_window.hpp"
#include "dsl_pipeline.hpp"
#include "dsl_device.hpp"


namespace dsl {

    class FirstApp {
        public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

    void run();

        private:
        DslWindow dslWindow{WIDTH, HEIGHT, "Vulkan Stuff IG!?!"};
        DslDevice dslDevice{dslWindow};
        DslPipeline dslPipeline{dslDevice, "../shaders/simple_shader.vert.spv", "../shaders/simple_shader.frag.spv", DslPipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)};
        
    };

}


