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



int DslWindow::event_watch(void* userdata, SDL_Event* event)
{
    if (event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_RESIZED) {
        printf("Window resized: %dx%d\n", event->window.data1, event->window.data2);
        SDL_Window* window = static_cast<SDL_Window*>(userdata);
        auto dslWindow = reinterpret_cast<DslWindow *>(SDL_GetWindowData(window, "windowUserData"));
                int w,h;
        SDL_GetWindowSize(dslWindow->getWindow(),&w,&h);
        printf("Window resized: %dx%d\n", w, h);
        dslWindow->framebufferResized = true;
        dslWindow->width = event->window.data1;
        dslWindow->height = event->window.data2;

        
        
        //framebufferResizeCallback(window ,event->window.data1, event->window.data1);

        // Recreate the swapchain and update the render targets here
    }
    return 0;
}

    void DslWindow::initWindow(){
        SDL_Init(SDL_INIT_EVERYTHING);


        SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);

        window = SDL_CreateWindow(
            windowName.c_str(),
            SDL_WINDOWPOS_UNDEFINED, // window position x (dont care)
            SDL_WINDOWPOS_UNDEFINED, // window position y (dont care)
            width,
            height,
            window_flags);
            SDL_SetWindowData(window, "windowUserData", this);
            SDL_AddEventWatch(event_watch, window);
            std::cout << "heh\n";

    }

    
    // void DslWindow::GetSDLExtensions(unsigned int *pCount, std::vector<const char*> * pNames){
    //     SDL_bool test = SDL_Vulkan_GetInstanceExtensions(window, pCount, NULL );
    //     //SDL_bool lol = SDL_Vulkan_GetInstanceExtensions(window, pCount, &extensions_cstr[0] );  
    // }



    
    void DslWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface){
        
        if (SDL_Vulkan_CreateSurface(window, instance, surface) != SDL_TRUE){
            throw std::runtime_error("Failed to create window surface");
        }
    }



    void DslWindow::framebufferResizeCallback(SDL_Window *window, int width, int height){;
        auto dslWindow = reinterpret_cast<DslWindow *> (SDL_GetWindowData(window, "windowUserData"));
        dslWindow->framebufferResized = true;
        dslWindow->width = width;
        dslWindow-> height = height;
    
    }


}