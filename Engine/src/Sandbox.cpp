#include "pch/enginepch.h"
// #include "Application.h"

#include "test.h"


int main() {
    // const auto app = std::make_shared<Application>();
    // app->run();
    // return 0;
    Logger::init();
    auto window = Window::create("DicyEngine", 1920, 1080);
    test t;

    int frameCount = 0;
    while (frameCount++ < 1000) {
        t.execute();
        window->update();
    }
}
