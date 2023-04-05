#include <vulkan/vulkan.h>
#include "first_app.hpp"

#include <iostream>
#include <stdexcept>
#include <cstdlib>

int main() {

    dsl::FirstApp app{};

    try{
        app.run();
    }catch (const std::exception &e){

        std::cerr << e.what() << '\n';

        return EXIT_FAILURE;
        }

    return EXIT_SUCCESS;

}