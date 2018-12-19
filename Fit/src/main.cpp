#include <application.hpp>
 #include "first_scene/first_scene.h"


int main()
{
    Application* app = new Application("Fit", 460, 580, false);
    Scene* scene = new RendertoTextureScene(app);

	app->setScene(scene);
    app->run();

    delete scene;
    delete app;

    return 0;
}
