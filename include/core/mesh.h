//
//
//
//
//
//

#include <core/vector.h>
#include <vector>
#include <cassert>
#pragma once

namespace te {
	namespace core {
		struct Triangle {
			Triangle() {};
			Triangle(const Vec3 &_p1, const Vec3& _p2, const Vec3& _p3) {
				p_[0] = _p1;
				p_[1] = _p2;
				p_[2] = _p3;

				n_ = (_p2 - _p1).cross(_p3 - _p1);
				n_ = n_/n_.norm();

				assert(fabs((_p2 - _p1).dot(n_)) <0.001);
				assert(fabs((_p3 - _p1).dot(n_)) <0.001);

			}

			Triangle& transform(const Mat44 &_mat) {
				p_[0] = p_[0] * _mat;
				p_[1] = p_[1] * _mat;
				p_[2] = p_[2] * _mat;
				return *this;
			}

			Triangle operator*(const Mat44 & _mat) const {
				return {
					p_[0] * _mat,
					p_[1] * _mat,
					p_[2] * _mat
				};
			}

			Vec3 p_[3];
			Vec3 n_;
		};

		struct Mesh {
			std::vector<Triangle> tris;
		};
	}
}