//
//
//
//
//

#include <core/Camera.h>

namespace te {
    namespace core{
        void Camera::renderShape(uint16_t _width, uint16_t _height){
            width_ = _width;
            height_ = _height;
        }

        void Camera::setProjectionMatrixParameters(float _near, float _far, float _fov, float _aspectRatio){
            near_ = _near;
			far_ = _far;
			fov_ = _fov;
			aspectRatio_ = _aspectRatio;
			
			fovRad_ = 1.0f / tanf(fov_ * 0.5f / 180.0f * 3.14159f);
			
			projMat_.data_[0][0] = aspectRatio_ * fovRad_;
			projMat_.data_[1][1] = fovRad_;
			projMat_.data_[2][2] = far_/(far_-near_);
			projMat_.data_[3][2] = (-far_*near_)/(far_-near_);
			projMat_.data_[2][3] = 1.0f;
			projMat_.data_[3][3] = 0.0f;
        }

        Vec3 Camera::projectPoint(const Vec3& _t) const{
            Vec3 point = _t * projMat_;
			point.x += 1.0f; 
			point.y += 1.0f;
			point.x *= 0.5f * width_;
			point.y *= 0.5f * height_;
			return point;
        }
            
    }
}
