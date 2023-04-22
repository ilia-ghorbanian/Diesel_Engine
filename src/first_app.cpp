#include "first_app.hpp"
#include <iostream>

#include "simple_render_system.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <array>
#include <cassert>




namespace dsl {


std::unique_ptr<DslModel> createCircleModel(DslDevice& device, unsigned int numSides) {
  std::vector<DslModel::Vertex> uniqueVertices{};
  for (int i = 0; i < numSides; i++) {
    float angle = i * glm::two_pi<float>() / numSides;
    uniqueVertices.push_back({{glm::cos(angle), glm::sin(angle)}});
  }
  uniqueVertices.push_back({});  // adds center vertex at 0, 0
 
  std::vector<DslModel::Vertex> vertices{};
  for (int i = 0; i < numSides; i++) {
    vertices.push_back(uniqueVertices[i]);
    vertices.push_back(uniqueVertices[(i + 1) % numSides]);
    vertices.push_back(uniqueVertices[numSides]);
  }
  return std::make_unique<DslModel>(device, vertices);
}

    FirstApp::FirstApp(){
        loadGameObjects();
    }

    FirstApp::~FirstApp(){}

    void FirstApp::run() {
        SimpleRenderSystem simpleRenderSystem{dslDevice, dslRenderer.getSwapChainRenderPass()};

        bool running = true;

        while(running){
            SDL_Event event;
            
            while(SDL_PollEvent(&event)){

            if (event.type == SDL_QUIT){
                running = 0;
                }
            
            };
            if (auto commandBuffer = dslRenderer.beginFrame()){
                dslRenderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);
                dslRenderer.endSwapChainRenderPass(commandBuffer);
                dslRenderer.endFrame();
            }
            vkDeviceWaitIdle(dslDevice.device());
        }
    }




    // void FirstApp::sierpinski(
    //     std::vector<DslModel::Vertex> &vertices,
    //     int depth,
    //     glm::vec2 left,
    //     glm::vec2 right,
    //     glm::vec2 top) {
    //         if (depth <= 0) {
    //             vertices.push_back({top});
    //             vertices.push_back({right});
    //             vertices.push_back({left});
    //         }else {
    //             auto leftTop = 0.5f * (left + top);
    //             auto rightTop = 0.5f * (right + top);
    //             auto leftRight = 0.5f * (left + right);
    //             sierpinski(vertices, depth - 1, left, leftRight, leftTop);
    //             sierpinski(vertices, depth - 1, leftRight, right, rightTop);
    //             sierpinski(vertices, depth - 1, leftTop, rightTop, top);
    // };
    //     }


    void FirstApp::EventHandlerTest(){
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type){
                case SDL_EventType::SDL_WINDOWEVENT:
                    if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED){
                        std::cout << "test worked ig?" << std::endl;
                    };
            }
        }
    }







    void FirstApp::loadGameObjects() {

        std::vector<DslModel::Vertex>  vertices {
            {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
        };
        // sierpinski(vertices, 5, {-0.5f, 0.5f}, {0.5f, 0.5f}, {0.0f, -0.5f});

        auto dslModel = std::make_shared<DslModel>(dslDevice, vertices);
        
        std::shared_ptr<DslModel> circleModel = createCircleModel(dslDevice, 100);

        auto triangle = DslGameObject::createGameObject();
        auto circle = DslGameObject::createGameObject();
        triangle.model = dslModel;
        triangle.color = {0.1f, .8f, .1f};
        triangle.transform2d.translation.x = .2f;
        triangle.transform2d.scale = {2.f, .5f};
        triangle.transform2d.rotation = .25f * glm::two_pi<float>();

        circle.model = circleModel;
        circle.color = {1.f, .8f, .1f};
        circle.transform2d.translation.x = -.5f;
        circle.transform2d.scale = {.5f, .5f};
        triangle.transform2d.rotation = 0;
        
        gameObjects.push_back(std::move(triangle));
        gameObjects.push_back(std::move(circle));

    }



}