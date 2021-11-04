

#include <os/WindowWindows.h>
using namespace te::os;

int main(int _argc, char** _argv) {
    WindowWindows app;

    if (SUCCEEDED(app.Initialize())) {
        app.RunMessageLoop();
    }

    return 0;
}