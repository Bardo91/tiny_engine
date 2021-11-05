//
//
//
//
//

#include <cstdint>
#include <core/mesh.h>
#include <core/matrix.h>

namespace te {
	namespace core {
		class Engine {
		public:
			Engine(unsigned _width, unsigned _height);

			void setProjectionMatrixParameters(float _near, float _far, float _fov, float _aspectRatio);

			void draw(uint8_t *_buffer);

		private:
			Triangle projectTriangle(Triangle _t) const;

			void drawPixel(uint8_t* _buffer, unsigned _i, unsigned _j, uint8_t _r, uint8_t _b, uint8_t _g, uint8_t _a = 255);
			void drawLine(uint8_t* _buffer, Vec3 _p1, Vec3 _p2, uint8_t _r, uint8_t _b, uint8_t _g, uint8_t _a = 255);
			void drawTriangle(uint8_t* _buffer, Triangle _t, uint8_t _r, uint8_t _b, uint8_t _g, uint8_t _a = 255);
			void drawTriangleFilled(uint8_t* _buffer, Triangle _t, uint8_t _r, uint8_t _b, uint8_t _g, uint8_t _a);
		private:
			unsigned width_, height_;

			float near_, far_, fov_, aspectRatio_, fovRad_;

			Mat44 projMat_;

			Mesh cube_;
			float timeCounter = 0.0f;
		
		};
	}
}