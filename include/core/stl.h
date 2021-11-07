//
//
//
//
//
//
#ifndef TINIENGINE_CORE_STL_H_
#define TINIENGINE_CORE_STL_H_


#include <core/mesh.h>
#include <string>
#include <fstream>
#include <iostream>

namespace te {
	namespace core {
		Mesh loadStl(const std::string& _filepath) {
			std::ifstream file(_filepath, std::ios::in | std::ios::binary);
			
			Mesh mesh;
			if (file.is_open()) {
				char header[80];
				file.read(header, 80);
				std::cout << header << std::endl;

				uint32_t nTriangles;
				file.read((char*)&nTriangles, 4);
				mesh.tris.resize(nTriangles);
				for (auto &tri: mesh.tris) {
					file.read((char*)&tri.n_.x, 4);
					file.read((char*)&tri.n_.y, 4);
					file.read((char*)&tri.n_.z, 4);
					for (unsigned i = 0; i < 3; i++) {
						file.read((char*)&tri.p_[i].x, 4);
						file.read((char*)&tri.p_[i].y, 4);
						file.read((char*)&tri.p_[i].z, 4);
					}
					uint16_t attr;
					file.read((char*)&attr, 2);
				}
			}

			return mesh;
		}
	}
}

#endif