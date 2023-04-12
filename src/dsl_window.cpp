#include "dsl_window.hpp"
#include <stdexcept>
#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>

namespace dsl{

    DslWindow::DslWindow(int w, int h, std::string name) : width{w}, height{h}, windowName{name} {
        initWindow();
    }



    DslWindow::~DslWindow(){

        SDL_DestroyWindow(window);
        SDL_Quit();
    }





    void DslWindow::initWindow(){
        SDL_Init(SDL_INIT_VIDEO);


        SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_VULKAN);

        window = SDL_CreateWindow(
            windowName.c_str(),
            SDL_WINDOWPOS_UNDEFINED, // window position x (dont care)
            SDL_WINDOWPOS_UNDEFINED, // window position y (dont care)
            width,
            height,
            window_flags);

    }

    
    // void DslWindow::GetSDLExtensions(unsigned int *pCount, std::vector<const char*> * pNames){

    //     SDL_bool test = SDL_Vulkan_GetInstanceExtensions(window, pCount, NULL );


    //     //SDL_bool lol = SDL_Vulkan_GetInstanceExtensions(window, pCount, &extensions_cstr[0] );


        
    // }



    
    void DslWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface){
        if (SDL_Vulkan_CreateSurface(window, instance, surface) != VK_SUCCESS){
            throw std::runtime_error("Failed to create window surface");
        }
        else{
            int i = 0;
        }
    }





}