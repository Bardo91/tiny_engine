//
//
//
//
//

#ifndef TINYENGINE_CORE_ENTITY_H_
#define TINYENGINE_CORE_ENTITY_H_

#include <string>

#include <core/mesh.h>

namespace te {
    namespace core{
        class Entity{
        public:
            void loadMeshFromFile(const std::string &_file);

            Mesh mesh_;
        };
    }
}

#endif