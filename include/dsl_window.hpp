#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <vector>
#include <vulkan/vulkan.h>
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

    VkExtent2D getExtent() {return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; }
    bool wasWindowResized() {return framebufferResized;}
    void resetWindowResized() {framebufferResized = false;}

    // void GetSDLExtensions(unsigned int *pCount, std::vector<const char*> * pNames); //DEPRECATED
    void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);


    private:

    static void framebufferResizeCallback(SDL_Window *window, int width, int height);
    void initWindow();

    int width;
    int height;
    bool framebufferResized = false;

    std::string windowName;
    SDL_Window *window;


    
    };

}