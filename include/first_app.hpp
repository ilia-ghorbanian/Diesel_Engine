#pragma once

#include "dsl_window.hpp"

namespace dsl {

    class FirstApp {
        public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        void run();

        private:
        DslWindow dslWindow{WIDTH, HEIGHT, "Vulkan Stuff IG!?!"};
    };

}