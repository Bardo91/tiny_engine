//
//
//
//
//

#ifndef TINYENGINE_CORE_ENGINE_H_
#define TINYENGINE_CORE_ENGINE_H_

#include <cstdint>
#include <mutex>
#include <memory>

#include <core/Camera.h>
#include <core/mesh.h>
#include <core/matrix.h>
#include <core/SwapBuffer.h>
#include <core/Scene.h>


namespace te {
	namespace core {
		class Engine {
		public:
			Engine(unsigned _width, unsigned _height);

			void setProjectionMatrixParameters(float _near, float _far, float _fov, float _aspectRatio);

			void render();

			const SwapBuffer &buffer(){return buffer_;};

			void setScene(std::shared_ptr<Scene> _scene){ scene_ = _scene;};

		private:
			Triangle projectTriangle(const Triangle& _t) const;
			
			void drawPixel(uint8_t* _buffer, unsigned _i, unsigned _j, uint8_t _r, uint8_t _b, uint8_t _g, uint8_t _a = 255);
			void drawLine(uint8_t* _buffer, Vec3 _p1, Vec3 _p2, uint8_t _r, uint8_t _b, uint8_t _g, uint8_t _a = 255);
			void drawTriangle(uint8_t* _buffer, Triangle _t, uint8_t _r, uint8_t _b, uint8_t _g, uint8_t _a = 255);
			void drawTriangleFilled(uint8_t* _buffer, Triangle _t, uint8_t _r, uint8_t _b, uint8_t _g, uint8_t _a=255);

		private:	// Don't like it too much, but I don't want to design the algorithm
			void ScanLine(int *contour, long x1, long y1, long x2, long y2);

		private:
			unsigned width_, height_;

			float timeCounter = 0.0f;

			float* zBuffer_;
			SwapBuffer buffer_;

			Camera cam_;		

			std::shared_ptr<Scene> scene_;
		};
	}
}

#endif