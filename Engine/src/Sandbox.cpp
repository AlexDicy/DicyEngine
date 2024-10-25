#include "pch/enginepch.h"
#include "Application.h"

int main() {
    const auto app = std::make_shared<Application>();
    app->run();
    return 0;
}
