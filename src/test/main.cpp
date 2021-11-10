

#include <os/WindowWindows.h>
#include <core/Engine.h>

using namespace te::os;
using namespace te::core;

#include <thread>

int main(int _argc, char** _argv) {

    int width = 640;
    int height = 480;

    Engine engine(width, height);
    WindowWindows win(engine.buffer());

    if (SUCCEEDED(win.Initialize())) {
        std::thread t([&] {
            while (true) {
                    engine.render();
                }
            }
        );

        win.RunMessageLoop();
    }

    return 0;
}