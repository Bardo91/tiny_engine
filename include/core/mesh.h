//
//
//
//
//
//

#include <core/vector.h>
#include <vector>
#pragma once

namespace te {
	namespace core {
		struct Triangle {
			Vec3 p[3];
		};

		struct Mesh {
			std::vector<Triangle> tris;
		};
	}
}