#pragma once
#include <internal/cef_ptr.h>


class OSRCefHandler;

class OSRCefClient {
public:
    OSRCefClient() = default;

    void start();

private:
    CefRefPtr<OSRCefHandler> handler;
};
