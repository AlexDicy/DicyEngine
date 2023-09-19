#include "pch/enginepch.h"
#include "common.h"

class DE_API Application {
public:
    Application() {
        logger::init();
        this->running = true;
    }

    void run() {
        while (this->running) {
            DE_TRACE("Running");
        }
    }
private:
    bool running;
};
