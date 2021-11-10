//
//
//
//
//

#ifndef TINYENGINE_CORE_CAMERA_H_
#define TINYENGINE_CORE_CAMERA_H_

#include <core/vector.h>
#include <core/matrix.h>

namespace te {
    namespace core{
        class Camera{
        public:
            void renderShape(uint16_t _width, uint16_t _height);
            void setProjectionMatrixParameters(float _near, float _far, float _fov, float _aspectRatio);
			Vec3 projectPoint(const Vec3& _t) const;
		private:
            float near_, far_, fov_, aspectRatio_, fovRad_;
            uint16_t width_, height_;
			Mat44 projMat_;
        };
    }
}

#endif