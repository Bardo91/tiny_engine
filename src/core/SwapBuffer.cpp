//
//
//
//
//

#include <core/SwapBuffer.h>

namespace te {
    namespace core{
        void SwapBuffer::initialize(int width, int height) {
            width_ = width;
            height_ = height;
            back_ = new uint8_t[width_ * height_ * 4];
            front_ = new uint8_t[width_ * height_ * 4];
        }

        unsigned SwapBuffer::width() const {
            return width_;
        }
        
        unsigned SwapBuffer::height() const {
            return height_;
        }

        unsigned SwapBuffer::channels() const {
            return channels_;
        }

        unsigned SwapBuffer::bufferSize() const {
            return height_ * width_ * channels_;
        }

        uint8_t* SwapBuffer::frontBuffer() const {
            return front_;
        }
        uint8_t* SwapBuffer::backBuffer() const {
            return back_;
        }
        void SwapBuffer::swap() {
            guard_.lock();
            uint8_t* tmp = front_;
            front_ = back_;
            back_ = tmp;
            guard_.unlock();
        }

        void SwapBuffer::lock() const { //  meh...
            guard_.lock();
        }

        void SwapBuffer::unlock() const {
            guard_.unlock();
        }
    }
}
