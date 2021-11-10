//
//
//
//
//

#ifndef TINYENGINE_CORE_SCENE_H_
#define TINYENGINE_CORE_SCENE_H_

#include <core/vector.h>
#include <core/Entity.h>
#include <vector>

namespace te {
    namespace core{

        struct Light{
            Vec3 origin;
            Vec3 direction;
        };

        class Scene{
        public:
            void addEntity(Entity &_entity) { entities_.push_back(_entity);}
            void setLight(Vec3 _origin, Vec3 _dir){light_ = {_origin, _dir};};

            inline const Light &light() const{ return light_; };

            inline const std::vector<Entity> &entities() const { return entities_; };

		private:
            std::vector<Entity> entities_;

            Light light_;
        };
    }
}

#endif