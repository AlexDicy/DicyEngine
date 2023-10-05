#include "application.h"

int main() {
    const auto app = new Application();
    app->run();
    delete app;
    return 0;
}
