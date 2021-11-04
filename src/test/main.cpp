

#include <os/WindowWindows.h>
#include <core/Engine.h>

using namespace te::os;
using namespace te::core;

#include <thread>

int main(int _argc, char** _argv) {
    WindowWindows app;

    int width = 640;
    int height = 480;

    if (SUCCEEDED(app.Initialize(width, height))) {
        std::thread t([&] {
            Engine engine(width, height);
            engine.setProjectionMatrixParameters(0.1f, 1000.0f, 90.0f, float(height)/ width);
            uint8_t* data = app.getBuffer();
            while (true) {
                    //#pragma omp parallel for collapse(2)
                    //for (int Y = 0; Y < height; Y++) {
                    //    for (int X = 0; X < width; X++)
                    //    {
                    //        int idx = (Y * width + X) * 4;
                    //        if (Y == X) {
                    //            data[idx + 0] = 255;
                    //            data[idx + 1] = 255;
                    //            data[idx + 2] = 255;
                    //            data[idx + 3] = 255;
                    //        }
                    //        else {
                    //            data[idx + 0] = 0;
                    //            data[idx + 1] = 0;
                    //            data[idx + 2] = 0;
                    //            data[idx + 3] = 255;
                    //        }
                    //        /*data[idx + 0] = uint8_t(float(rand()) / RAND_MAX * 255);
                    //        data[idx + 1] = uint8_t(float(rand()) / RAND_MAX * 255);
                    //        data[idx + 2] = uint8_t(float(rand()) / RAND_MAX * 255);
                    //        data[idx + 3] = 255;*/
                    //    }
                    //}
                    engine.draw(data);
                }
            }
        );

        app.RunMessageLoop();
    }

    return 0;
}