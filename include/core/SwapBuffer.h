//
//
//
//
//

#ifndef TINYENGINE_CORE_SWAPBUFFER_H_
#define TINYENGINE_CORE_SWAPBUFFER_H_

#include <cstdint>
#include <mutex>

namespace te {
    namespace core{
		class SwapBuffer {
		public:
			void initialize(int width, int height);

			unsigned width() const;
			unsigned height() const;
			unsigned channels() const;

			unsigned bufferSize() const;

			uint8_t* frontBuffer() const;
			uint8_t* backBuffer() const;

			void swap();

			void lock() const;
			void unlock() const;
		private:
			uint8_t* currentBuffer_;
			uint8_t* back_, *front_;
			int width_, height_, channels_=4;
			mutable std::mutex guard_; 
		};

    }
}

#endif