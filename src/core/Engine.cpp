//
//
//
//
//

#include <core/Engine.h>
#include <math.h>
#include <algorithm>

namespace te {
	namespace core {
		Engine::Engine(unsigned _width, unsigned _height) {
			width_ = _width;
			height_ = _height;
			zBuffer_ = new float[width_ * height_];
			buffer_.initialize(width_, height_);

			//mesh_.tris = {
			//	// SOUTH
			//	{{ 0.0f, 0.0f, 0.0f},    {0.0f, 1.0f, 0.0f},    {1.0f, 1.0f, 0.0f} },
			//	{{ 0.0f, 0.0f, 0.0f},    {1.0f, 1.0f, 0.0f},    {1.0f, 0.0f, 0.0f} },

			//	// EAST                                                      
			//	{{ 1.0f, 0.0f, 0.0f},    {1.0f, 1.0f, 0.0f},    {1.0f, 1.0f, 1.0f} },
			//	{{ 1.0f, 0.0f, 0.0f},    {1.0f, 1.0f, 1.0f},    {1.0f, 0.0f, 1.0f} },

			//	// NORTH                                                     
			//	{{ 1.0f, 0.0f, 1.0f},    {1.0f, 1.0f, 1.0f},    {0.0f, 1.0f, 1.0f} },
			//	{{ 1.0f, 0.0f, 1.0f},    {0.0f, 1.0f, 1.0f},    {0.0f, 0.0f, 1.0f} },

			//	// WEST                                                      
			//	{{ 0.0f, 0.0f, 1.0f},    {0.0f, 1.0f, 1.0f},    {0.0f, 1.0f, 0.0f} },
			//	{{ 0.0f, 0.0f, 1.0f},    {0.0f, 1.0f, 0.0f},    {0.0f, 0.0f, 0.0f} },

			//	// TOP                                                       
			//	{{ 0.0f, 1.0f, 0.0f},    {0.0f, 1.0f, 1.0f},    {1.0f, 1.0f, 1.0f} },
			//	{{ 0.0f, 1.0f, 0.0f},    {1.0f, 1.0f, 1.0f},    {1.0f, 1.0f, 0.0f} },
			//	
			//	// BOTTOM                                                    
			//	{{ 1.0f, 0.0f, 1.0f},    {0.0f, 0.0f, 1.0f},    {0.0f, 0.0f, 0.0f} },
			//	{{ 1.0f, 0.0f, 1.0f},    {0.0f, 0.0f, 0.0f},    {1.0f, 0.0f, 0.0f} },
			//};

			//mesh_.tris = {
			//	//{{0,0,0},{0,1,0},{1,0,0}},
			//	//{{0,0,0},{0,1,0},{0,0,1}},
			//	//{{0,0,0},{0,0,1},{1,0,0}},
			//	{{1,0,0},{0,0,1},{0,1,0}}
			//};

			cam_.setProjectionMatrixParameters(0.1f, 1000.0f, 90.0f, float(height_)/ width_);
			cam_.renderShape(width_, height_);

		}

		void Engine::render() {
			uint8_t* buffer = buffer_.backBuffer();

			memset(buffer, 0, width_ * height_ * 4);
			std::fill(zBuffer_, zBuffer_ + width_ * height_, std::numeric_limits<float>::max());

			Mat44 rotZ = Mat44::rotZ(timeCounter);
			Mat44 rotX = Mat44::rotX(timeCounter*0.5f);
			
			for(auto &entity:scene_->entities()){
				#pragma omp parallel for
				for (auto tri : entity.mesh_.tris) {
					timeCounter += 0.00001;

					tri.transform(rotZ).transform(rotX);
					tri.p_[0].z += 3.0f;
					tri.p_[1].z += 3.0f;
					tri.p_[2].z += 3.0f;

					// Check if triangle should be drawn
					Vec3 camera = { 0.0f, 0.0f, 0.0f };

					if (tri.n_.dot(tri.p_[0] - camera) < 0) {
						Triangle triProj = projectTriangle(tri);

						int light = 0;
						float valLight = tri.n_.dot(scene_->light().direction);
						if (valLight > -1 && valLight < -0.5) {
							light = 255;
						}else if (valLight > -0.5 && valLight < 0) {
							light = 175;
						}else if (valLight > 0 && valLight < 0.5) {
							light = 100;
						} else {
							light = 50;
						}

						drawTriangleFilled(buffer, triProj, light, light, light);
					}
				}
			}

			buffer_.swap();
		}

		Triangle Engine::projectTriangle(const Triangle& _t) const {
			Triangle triProj;
			for(unsigned i=0; i < 3; i++){
				triProj.p_[i] = cam_.projectPoint(_t.p_ [i]);
			}
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
			int x0 = _p1.x;
			int y0 = _p1.y;
			int x1 = _p2.x;
			int y1 = _p2.y;

			int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
			int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
			int err = dx + dy, e2; /* error value e_xy */

			for (;;) {  /* loop */
				drawPixel(_buffer, x0, y0, _r, _g, _b, _a);
				if (x0 == x1 && y0 == y1) break;
				e2 = 2 * err;
				if (e2 >= dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
				if (e2 <= dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
			}
		}

		void Engine::drawTriangle(uint8_t* _buffer, Triangle _t, uint8_t _r, uint8_t _b, uint8_t _g, uint8_t _a) {
			drawLine(_buffer, _t.p_[0], _t.p_[1], _r, _g, _b, _a);
			drawLine(_buffer, _t.p_[1], _t.p_[2], _r, _g, _b, _a);
			drawLine(_buffer, _t.p_[2], _t.p_[0], _r, _g, _b, _a);
		}

		void Engine::ScanLine(int* contour, long x1, long y1, long x2, long y2) {
			long sx, sy, dx1, dy1, dx2, dy2, x, y, m, n, k, cnt;

			sx = x2 - x1;
			sy = y2 - y1;

			if (sx > 0) dx1 = 1;
			else if (sx < 0) dx1 = -1;
			else dx1 = 0;

			if (sy > 0) dy1 = 1;
			else if (sy < 0) dy1 = -1;
			else dy1 = 0;

			m = fabs(sx);
			n = fabs(sy);
			dx2 = dx1;
			dy2 = 0;

			if (m < n)
			{
				m = fabs(sy);
				n = fabs(sx);
				dx2 = 0;
				dy2 = dy1;
			}

			x = x1; y = y1;
			cnt = m + 1;
			k = n / 2;

			while (cnt--)
			{
				if ((y >= 0) && (y < height_))
				{
					if (x < contour[y*2]) contour[y*2] = x;
					if (x > contour[y*2+1]) contour[y*2+1] = x;
				}

				k += n;
				if (k < m)
				{
					x += dx2;
					y += dy2;
				}
				else
				{
					k -= m;
					x += dx1;
					y += dy1;
				}
			}
		}

		void Engine::drawTriangleFilled(uint8_t* _buffer, Triangle _t, uint8_t _r, uint8_t _b, uint8_t _g, uint8_t _a) {

			int* contour = new int[height_ * 2];
			for (unsigned i = 0; i < height_; i++) {
				contour[i * 2 + 0] = width_;
				contour[i * 2 + 1] = 0;
			}

			ScanLine(contour, _t.p_[0].x, _t.p_[0].y, _t.p_[1].x, _t.p_[1].y);
			ScanLine(contour, _t.p_[1].x, _t.p_[1].y, _t.p_[2].x, _t.p_[2].y);
			ScanLine(contour, _t.p_[2].x, _t.p_[2].y, _t.p_[0].x, _t.p_[0].y);

			float midVal = (_t.p_[0].z + _t.p_[1].z + _t.p_[2].z) / 3;

			for (unsigned i = 0; i < height_; i++) {
				for (unsigned x = contour[i * 2]; x <= contour[i * 2 + 1]; x++) {
					if (contour[i * 2 + 1] >= contour[i * 2]) {
						if (zBuffer_[i * width_ + x] > midVal) {
							zBuffer_[i * width_ + x] = midVal;
							drawPixel(_buffer, x, i, _r, _g, _b, _a);
						}
					}
				}
			}

			delete[] contour;
		}
	}
}