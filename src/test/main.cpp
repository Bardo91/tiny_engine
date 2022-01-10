//
//
//
//

#include <thread>
#include <os/WindowWindows.h>
#include <core/Engine.h>
#include <core/Entity.h>

using namespace te::os;
using namespace te::core;

int main(int _argc, char** _argv) {

    int width = 640;
    int height = 480;

    Engine engine(width, height);
    WindowWindows win(engine.buffer());

    std::shared_ptr<Scene> scene(new Scene);
    engine.setScene(scene);
    scene->setLight({0,0,0}, {0,0,1});
    Entity eevee;
    eevee.loadMeshFromFile(_argv[1]);
    scene->addEntity(eevee);

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