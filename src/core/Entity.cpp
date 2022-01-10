//
//
//
//

#include <core/Entity.h>
#include <core/stl.h>

namespace te {
    namespace core{
        void Entity::loadMeshFromFile(const std::string &_file){
			mesh_ = loadStl(_file);
			mesh_.scale(0.03);
        }
    }
}
