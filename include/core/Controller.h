//
//
//
//
//

#ifndef TINYENGINE_CORE_CAMERA_H_
#define TINYENGINE_CORE_CAMERA_H_

#include <cstdint>
#include <functional>

namespace te {
    namespace core{
        struct KeyEvent{
            enum class Type {KeyDown, KeyUp};
            Type type;
            uint8_t keyNumber_;
        };
        
        class Controller{
        public:
            void setKeyboardCallback(std::function<void(const KeyEvent&)> _callback){
                callback_ = _callback;
            }

            void flush(const KeyEvent&_event){
                callback_(_event);
            }

        protected:
            std::function<void(const KeyEvent&)> callback_;

        };
    }
}

#endif