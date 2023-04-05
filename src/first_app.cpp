#include "first_app.hpp"
#include <iostream>
namespace dsl {
    void FirstApp::run() {

        bool running = true;

        while(running){
            SDL_Event event;
            while(SDL_PollEvent(&event)){

            



            if (event.type == SDL_QUIT){
                running = 0;
                }
            };
        }

    }
}