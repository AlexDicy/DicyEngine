#include "pch/enginepch.h"
#include "application.h"

int main() {
    const auto app = std::make_shared<Application>();
    app->run();
    return 0;
}
