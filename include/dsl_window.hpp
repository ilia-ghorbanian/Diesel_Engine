#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <vector>

#include <string> 

//#define SDL_WINDOW_VULKAN

namespace dsl{

class DslWindow {
    public:
    
    DslWindow(int w, int h, std::string name);
    ~DslWindow();

    DslWindow(const DslWindow &) = delete;
    DslWindow &operator=(const DslWindow &) = delete;
    auto getWindow(){return window;};

    // void GetSDLExtensions(unsigned int *pCount, std::vector<const char*> * pNames); //DEPRECATED
    void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);


    private:
    
    void initWindow();

    const int width;
    const int height;

    std::string windowName;
    SDL_Window *window;


    
    };

}