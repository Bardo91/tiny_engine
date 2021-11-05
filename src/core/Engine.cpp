//
//
//
//
//

#include <core/Engine.h>
#include<math.h>

namespace te {
	namespace core {
		Engine::Engine(unsigned _width, unsigned _height) {
			width_ = _width;
			height_ = _height;

			cube_.tris = {
				{0.0f, 0.0f, 0.0f,		0.0f, 1.0f, 0.0f,		1.0f, 1.0f, 0.0f, },
				{0.0f, 0.0f, 0.0f,		1.0f, 1.0f, 0.0f,		1.0f, 0.0f, 0.0f, },

				{1.0f, 0.0f, 0.0f,		1.0f, 1.0f, 0.0f,		1.0f, 1.0f, 1.0f, },
				{1.0f, 0.0f, 0.0f,		1.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f, },

				{1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 1.0f, },
				{1.0f, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f, },

				{0.0f, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f,		0.0f, 1.0f, 0.0f, },
				{0.0f, 0.0f, 1.0f,		0.0f, 1.0f, 0.0f,		0.0f, 0.0f, 0.0f, },

				{0.0f, 1.0f, 0.0f,		0.0f, 1.0f, 1.0f,		1.0f, 1.0f, 1.0f, },
				{0.0f, 1.0f, 0.0f,		1.0f, 1.0f, 1.0f,		1.0f, 1.0f, 0.0f, },

				{1.0f, 0.0f, 1.0f,		0.0f, 0.0f, 1.0f,		0.0f, 0.0f, 0.0f, },
				{1.0f, 0.0f, 1.0f,		0.0f, 0.0f, 0.0f,		1.0f, 0.0f, 0.0f, },
			};
		}

		void Engine::setProjectionMatrixParameters(float _near, float _far, float _fov, float _aspectRatio) {
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

		void Engine::draw(uint8_t* _buffer) {
			memset(_buffer, 0, width_ * height_ * 4);

			Mat44 rotZ = Mat44::rotZ(timeCounter);
			Mat44 rotX = Mat44::rotX(timeCounter*0.5f);;
			
			for (auto tri : cube_.tris) {
				timeCounter += 0.00001;

				tri.p[0] = tri.p[0] * rotZ * rotX;
				tri.p[1] = tri.p[1] * rotZ * rotX;
				tri.p[2] = tri.p[2] * rotZ * rotX;
				
				tri.p[0].z += 3.0f;
				tri.p[1].z += 3.0f;
				tri.p[2].z += 3.0f;

				Triangle triProj = projectTriangle(tri);

				drawTriangle(_buffer, triProj, 255, 0, 0);
			}
		}

		Triangle Engine::projectTriangle(Triangle _t) const {
			Triangle triProj;
			triProj.p[0] = _t.p[0] * projMat_;
			triProj.p[1] = _t.p[1] * projMat_;
			triProj.p[2] = _t.p[2] * projMat_;


			triProj.p[0].x += 1.0f; triProj.p[0].y += 1.0f;
			triProj.p[1].x += 1.0f; triProj.p[1].y += 1.0f;
			triProj.p[2].x += 1.0f; triProj.p[2].y += 1.0f;

			triProj.p[0].x *= 0.5f * width_;
			triProj.p[0].y *= 0.5f * height_;
			triProj.p[1].x *= 0.5f * width_;
			triProj.p[1].y *= 0.5f * height_;
			triProj.p[2].x *= 0.5f * width_;
			triProj.p[2].y *= 0.5f * height_;

			return triProj;
		}

		void Engine::drawPixel(uint8_t *_buffer, unsigned _x, unsigned _y, uint8_t _r, uint8_t _b, uint8_t _g, uint8_t _a) {
			if (_x > 0 && _x < width_ && _y > 0 && _y < height_) {
				_buffer[(_y * width_ + _x)*4 + 0] = _r;
				_buffer[(_y * width_ + _x)*4 + 1] = _g;
				_buffer[(_y * width_ + _x)*4 + 2] = _b;
				_buffer[(_y * width_ + _x)*4 + 3] = _a;
			}
		}

		void Engine::drawLine(uint8_t* _buffer, Vec3 _p1, Vec3 _p2, uint8_t _r, uint8_t _b, uint8_t _g, uint8_t _a){
			float dx = _p2.x - _p1.x;
			float dy = _p2.y - _p1.y;

			if (dx > dy) { // Line is "more" horizontal
				float m = dy / dx;
				float sign = dx > 0 ? 1 : -1;
				if (dx > 0) {
					for (unsigned i = 0; i < fabs(dx); i++) {
						unsigned x = _p1.x + sign*i;
						unsigned y = _p1.y + m*sign*i;
						drawPixel(_buffer, y, x, _r, _g, _b, _a);
					}
				} 
			} else { // "Line is "more" vertical
				float m = dx / dy;
				float sign = dy > 0 ? 1 : -1;
				if (dx > 0) {
					for (unsigned i = 0; i < fabs(dy); i++) {
						unsigned y = _p1.y + sign * i;
						unsigned x = _p1.x + m * sign * i;
						drawPixel(_buffer, y, x, _r, _g, _b, _a);
					}
				}
			}

			/*float dx = _p2.x - _p1.x;
			float dy = _p2.y - _p1.y;

			float m = dy / dx;
			float b = _p1.y - (m * _p1.x);

			int j = 0;
			for (size_t i = 0; i < dx; i++) {
				drawPixel(_buffer, _p1.x + i, round(m * (_p1.x + i) + b), _r, _g, _b, _a);
			}*/

		}

		void Engine::drawTriangle(uint8_t* _buffer, Triangle _t, uint8_t _r, uint8_t _b, uint8_t _g, uint8_t _a) {
			drawLine(_buffer, _t.p[0], _t.p[1], _r, _g, _b, _a);
			drawLine(_buffer, _t.p[1], _t.p[2], _r, _g, _b, _a);
			drawLine(_buffer, _t.p[2], _t.p[0], _r, _g, _b, _a);
		}

		void Engine::drawTriangleFilled(uint8_t* _buffer, Triangle _t, uint8_t _r, uint8_t _b, uint8_t _g, uint8_t _a) {
			drawLine(_buffer, _t.p[0], _t.p[1], _r, _g, _b, _a);
			drawLine(_buffer, _t.p[1], _t.p[2], _r, _g, _b, _a);
			drawLine(_buffer, _t.p[2], _t.p[0], _r, _g, _b, _a);
		}
	}
}