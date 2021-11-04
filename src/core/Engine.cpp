//
//
//
//
//

#include <core/Engine.h>
#include<math.h>

namespace te {
	namespace core {

		void multiplyMatrixVector(const Vec3 &_i, Vec3 &_o, float _m[4][4]) {
			_o.x = _i.x * _m[0][0] + _i.y * _m[1][0] + _i.z * _m[2][0] + _m[3][0];
			_o.y = _i.x * _m[0][1] + _i.y * _m[1][1] + _i.z * _m[2][1] + _m[3][1];
			_o.z = _i.x * _m[0][2] + _i.y * _m[1][2] + _i.z * _m[2][2] + _m[3][2];
			float w = _i.x * _m[0][3] + _i.y * _m[1][3] + _i.z * _m[2][3] + _m[3][3];

			if (w != 0.0f)
			{
				_o.x /= w; _o.y /= w; _o.z /= w;
			}
		}

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
			memset(projMat_, 0, sizeof(float)*16);
			near_ = _near;
			far_ = _far;
			fov_ = _fov;
			aspectRatio_ = _aspectRatio;
			
			fovRad_ = 1.0f / tanf(fov_ * 0.5f / 180.0f * 3.14159f);
			
			projMat_[0][0] = aspectRatio_ * fovRad_;
			projMat_[1][1] = fovRad_;
			projMat_[2][2] = far_/(far_-near_);
			projMat_[3][2] = (-far_*near_)/(far_-near_);
			projMat_[2][3] = 1.0f;
			projMat_[3][3] = 0.0f;

		}

		void Engine::draw(uint8_t* _buffer) {

			memset(_buffer, 0, width_ * height_ * 4);

			float rotZ[4][4];
			memset(rotZ, 0, sizeof(float) * 16);
			rotZ[0][0] = cosf(timeCounter);
			rotZ[0][1] = sinf(timeCounter);
			rotZ[1][0] = -sinf(timeCounter);
			rotZ[1][1] = cosf(timeCounter);
			rotZ[2][2] = 1.0f;
			rotZ[3][3] = 1.0f;

			float rotX[4][4];
			memset(rotX, 0, sizeof(float) * 16);
			rotX[0][0] = 1.0f;
			rotX[1][1] = cosf(timeCounter*0.5f);
			rotX[1][2] = sinf(timeCounter * 0.5f);
			rotX[2][1] = -sinf(timeCounter * 0.5f);
			rotX[2][2] = cosf(timeCounter * 0.5f);
			rotX[3][3] = 1.0f;



			for (const auto &tri : cube_.tris) {
				timeCounter += 0.00001;
				Triangle triRotZ, triRotX;

				multiplyMatrixVector(tri.p[0], triRotZ.p[0], rotZ);
				multiplyMatrixVector(tri.p[1], triRotZ.p[1], rotZ);
				multiplyMatrixVector(tri.p[2], triRotZ.p[2], rotZ);

				multiplyMatrixVector(triRotZ.p[0], triRotX.p[0], rotX);
				multiplyMatrixVector(triRotZ.p[1], triRotX.p[1], rotX);
				multiplyMatrixVector(triRotZ.p[2], triRotX.p[2], rotX);

				triRotX.p[0].z += 3.0f;
				triRotX.p[1].z += 3.0f;
				triRotX.p[2].z += 3.0f;


				Triangle triProj;
				
				multiplyMatrixVector(triRotX.p[0], triProj.p[0], projMat_);
				multiplyMatrixVector(triRotX.p[1], triProj.p[1], projMat_);
				multiplyMatrixVector(triRotX.p[2], triProj.p[2], projMat_);


				triProj.p[0].x += 1.0f; triProj.p[0].y += 1.0f;
				triProj.p[1].x += 1.0f; triProj.p[1].y += 1.0f;
				triProj.p[2].x += 1.0f; triProj.p[2].y += 1.0f;

				triProj.p[0].x *= 0.5f * width_; 
				triProj.p[0].y *= 0.5f * height_;
				triProj.p[1].x *= 0.5f * width_; 
				triProj.p[1].y *= 0.5f * height_;
				triProj.p[2].x *= 0.5f * width_; 
				triProj.p[2].y *= 0.5f * height_;


				drawTriangle(_buffer, triProj, 255, 0, 0);
			}
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
			float mod = sqrt(dx * dx + dy * dy);
			dx /= mod;
			dy /= mod;

			if (dx > dy) { // Line is "more" horizontal
				if (dx > 0) {
					for (unsigned x = _p1.x; x < _p2.x; x++) {
						unsigned y = _p1.y + dy * (x - _p1.x);
						drawPixel(_buffer, y, x, _r, _g, _b, _a);
					}
				} else {
					for (unsigned x = _p1.x; x > _p2.x; x--) {
						unsigned y = _p1.y + dy * (x - _p1.x);
						drawPixel(_buffer, y, x, _r, _g, _b, _a);
					}
				}
			} else { // "Line is "more" vertical
				if (dy > 0) {
					for (unsigned y = _p1.y; y < _p2.y; y++) {
						unsigned x = _p1.x + dx * (y - _p1.y);
						drawPixel(_buffer, y, x, _r, _g, _b, _a);
					}
				}
				else {
					for (unsigned y = _p1.y; y > _p2.y; y--) {
						unsigned x = _p1.x + dx * (y - _p1.y);
						drawPixel(_buffer, y, x, _r, _g, _b, _a);
					}
				}
			}
		}

		void Engine::drawTriangle(uint8_t* _buffer, Triangle _t, uint8_t _r, uint8_t _b, uint8_t _g, uint8_t _a) {
			drawLine(_buffer, _t.p[0], _t.p[1], _r, _g, _b, _a);
			drawLine(_buffer, _t.p[1], _t.p[2], _r, _g, _b, _a);
			drawLine(_buffer, _t.p[2], _t.p[0], _r, _g, _b, _a);
		}
	}
}