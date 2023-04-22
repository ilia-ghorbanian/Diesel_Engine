#pragma once

#include "dsl_model.hpp"
#include <memory>




namespace dsl{


    struct Transform2dComponent{

        glm::vec2 translation{};
        glm::vec2 scale{1.f, 1.f};
        float rotation;

        glm::mat2 mat2() {
            
            const float s = glm::sin(rotation);
            const float c = glm::cos(rotation);
            glm::mat2 rotMatrix{{c,s}, {-s, c}};
            
            glm::mat2 scaleMat{{scale.x, .0f}, {.0f, scale.y}};
            return rotMatrix * scaleMat;
        }
    };

    class DslGameObject {

        public:
        using id_t = unsigned int;

        static DslGameObject createGameObject(){
            static id_t currentId = 0;
            return DslGameObject{currentId++};
        }



        DslGameObject(const DslGameObject &) = delete;
        DslGameObject& operator=(const DslGameObject &) = delete;
        DslGameObject(DslGameObject &&) = default;
        DslGameObject &operator=(DslGameObject &&) = default;


        const id_t getId(){return id;}

        std::shared_ptr<DslModel> model{};
        glm::vec3 color{};
        Transform2dComponent transform2d{};

        private:
        DslGameObject(id_t objId) : id{objId}{};

        id_t id;


    };




}