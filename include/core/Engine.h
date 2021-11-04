//
//
//
//
//

#include <cstdint>
#include <vector>

namespace te {
	namespace core {

		struct Vec3 {
			float x, y, z;
		};

		struct Triangle {
			Vec3 p[3];
		};

		struct Mesh {
			std::vector<Triangle> tris;
		};

		class Engine {
		public:
			Engine(unsigned _width, unsigned _height);

			void setProjectionMatrixParameters(float _near, float _far, float _fov, float _aspectRatio);

			void draw(uint8_t *_buffer);

		private:

			void drawPixel(uint8_t* _buffer, unsigned _i, unsigned _j, uint8_t _r, uint8_t _b, uint8_t _g, uint8_t _a = 255);
			void drawLine(uint8_t* _buffer, Vec3 _p1, Vec3 _p2, uint8_t _r, uint8_t _b, uint8_t _g, uint8_t _a = 255);
			void drawTriangle(uint8_t* _buffer, Triangle _t, uint8_t _r, uint8_t _b, uint8_t _g, uint8_t _a = 255);
			void drawTriangleFilled(uint8_t* _buffer, Triangle _t, uint8_t _r, uint8_t _b, uint8_t _g, uint8_t _a);
		private:
			unsigned width_, height_;

			float near_, far_, fov_, aspectRatio_, fovRad_;

			float projMat_[4][4];

			Mesh cube_;
			float timeCounter = 0.0f;
		
		};
	}
}