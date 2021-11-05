//
//
//
//
//
//
//

#include <core/matrix.h>
#include <cmath>
#pragma once

namespace te {
	namespace core {
		struct Vec3 {
			float x, y, z;

			Vec3 operator+(const Vec3& _other) const {
				Vec3 res;
				res.x = x + _other.x;
				res.y = y + _other.y;
				res.z = z + _other.z;
				return res;
			}

			Vec3 operator-(const Vec3& _other) const {
				Vec3 res;
				res.x = x - _other.x;
				res.y = y - _other.y;
				res.z = z - _other.z;
				return res;
			}

			Vec3 operator/(const float& _scalar) const {
				Vec3 res;
				res.x = x / _scalar;
				res.y = y / _scalar;
				res.z = z / _scalar;
				return res;
			}

			float norm() const {
				return sqrt(x * x + y * y + z * z);
			}

			float dot(const Vec3& _other) const {
				return x * _other.x + y * _other.y + z * _other.z;
			}

			Vec3 cross(const Vec3& _other) const {
				return{
					y * _other.z - z * _other.y,
					z* _other.x  - x* _other.z,
					x* _other.y - y* _other.x
				};
			}

			Vec3 operator*(const Mat44& _mat) const{
				Vec3 out;

				out.x = x * _mat.data_[0][0] + y * _mat.data_[1][0] + z * _mat.data_[2][0] + _mat.data_[3][0];
				out.y = x * _mat.data_[0][1] + y * _mat.data_[1][1] + z * _mat.data_[2][1] + _mat.data_[3][1];
				out.z = x * _mat.data_[0][2] + y * _mat.data_[1][2] + z * _mat.data_[2][2] + _mat.data_[3][2];
				float w = x * _mat.data_[0][3] + y * _mat.data_[1][3] + z * _mat.data_[2][3] + _mat.data_[3][3];

				if (w != 0.0f)
				{
					out.x /= w; out.y /= w; out.z /= w;
				}
				return out;
			}
		};
	}
}