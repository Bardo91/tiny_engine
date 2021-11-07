//
//
//
//
//

#ifndef TINYENGINE_CORE_ENGINE_H_
#define TINYENGINE_CORE_ENGINE_H_

#include <cstdint>
#include <core/mesh.h>
#include <core/matrix.h>
#include <mutex>

namespace te {
	namespace core {

		class SwapBuffer {
		public:
			void initialize(int width, int height) {
				width_ = width;
				height_ = height;
				back_ = new uint8_t[width_ * height_ * 4];
				front_ = new uint8_t[width_ * height_ * 4];
			}

			unsigned width() {
				return width_;
			}
			
			unsigned height() {
				return height_;
			}

			unsigned channels() {
				return channels_;
			}

			unsigned bufferSize() {
				return height_ * width_ * channels_;
			}

			uint8_t* frontBuffer() {
				return front_;
			}
			uint8_t* backBuffer() {
				return back_;
			}
			void swap() {
				guard_.lock();
				uint8_t* tmp = front_;
				front_ = back_;
				back_ = tmp;
				guard_.unlock();
			}

			void lock() { //  meh...
				guard_.lock();
			}

			void unlock() {
				guard_.unlock();
			}


		private:
			uint8_t* currentBuffer_;
			uint8_t* back_, *front_;
			int width_, height_, channels_=4;
			std::mutex guard_; 
		};

		class Engine {
		public:
			Engine(unsigned _width, unsigned _height);

			void setProjectionMatrixParameters(float _near, float _far, float _fov, float _aspectRatio);

			void draw(SwapBuffer &_buffer);

		private:
			Triangle projectTriangle(const Triangle& _t) const;
			Vec3 projectPoint(const Vec3& _t) const;
			
			void drawPixel(uint8_t* _buffer, unsigned _i, unsigned _j, uint8_t _r, uint8_t _b, uint8_t _g, uint8_t _a = 255);
			void drawLine(uint8_t* _buffer, Vec3 _p1, Vec3 _p2, uint8_t _r, uint8_t _b, uint8_t _g, uint8_t _a = 255);
			void drawTriangle(uint8_t* _buffer, Triangle _t, uint8_t _r, uint8_t _b, uint8_t _g, uint8_t _a = 255);
			void drawTriangleFilled(uint8_t* _buffer, Triangle _t, uint8_t _r, uint8_t _b, uint8_t _g, uint8_t _a=255);

		private:	// Don't like it too much, but I don't want to design the algorithm
			void ScanLine(int *contour, long x1, long y1, long x2, long y2);

		private:
			unsigned width_, height_;

			float near_, far_, fov_, aspectRatio_, fovRad_;

			Mat44 projMat_;

			Mesh mesh_;
			float timeCounter = 0.0f;
		
		};
	}
}

#endif