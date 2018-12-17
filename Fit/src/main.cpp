#include <application.hpp>
 #include "first_scene/first_scene.h"


int main()
{
    Application* app = new Application("Frame Buffers", 640, 480, false);
    Scene* scene = new RendertoTextureScene(app);

	app->setScene(scene);
    app->run();

    delete scene;
    delete app;

    return 0;
}
