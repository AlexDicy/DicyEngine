#include "application.h"
#include "events/dispatcher.h"

int main() {
    const auto app = new Application();
    app->run();
    delete app;
    return 0;
}
