//
//
//
//

#ifndef TINYENGINE_CORE_CAMERA_H_
#define TINYENGINE_CORE_CAMERA_H_

#include <core/Entity.h>
#include <core/stl.h>

namespace te {
    namespace core{
        void Entity::loadMeshFromFile(const std::string &_file){
			mesh_ = loadStl("C:/Users/Alejandro/Desktop/tiny_engine/samples/eevee.stl");
			mesh_.scale(0.03);
        }
    }
}

#endif