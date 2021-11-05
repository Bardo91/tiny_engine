//
//
//
//
//
//

#include <string>
#pragma once

namespace te {
	namespace core {
		struct Mat44 {
			Mat44() {
				memset(data_, 0, sizeof(float) * 16);
			}

			float data_[4][4];

			static Mat44 rotX(float _angle) {
				Mat44 rot;
				rot.data_[0][0] = 1.0f;
				rot.data_[1][1] = cosf(_angle);
				rot.data_[1][2] = sinf(_angle);
				rot.data_[2][1] = -sinf(_angle);
				rot.data_[2][2] = cosf(_angle);
				rot.data_[3][3] = 1.0f;
				return rot;
			}
			
			static Mat44 rotY(float _angle) {
				Mat44 rot;
				rot.data_[0][0] = cosf(_angle);
				rot.data_[0][2] = sinf(_angle);
				rot.data_[2][0] = -sinf(_angle);
				rot.data_[2][2] = cosf(_angle);
				rot.data_[1][1] = 1.0f;
				rot.data_[3][3] = 1.0f;
				return rot;
			}

			static Mat44 rotZ(float _angle) {
				Mat44 rot;
				rot.data_[0][0] = cosf(_angle);
				rot.data_[0][1] = sinf(_angle);
				rot.data_[1][0] = -sinf(_angle);
				rot.data_[1][1] = cosf(_angle);
				rot.data_[2][2] = 1.0f;
				rot.data_[3][3] = 1.0f;
				return rot;
			}


		};


		
	}
}