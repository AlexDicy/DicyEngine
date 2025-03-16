#pragma once
#include "cef_app.h"

// Implement application-level callbacks for the browser process.
class OSRCefApp : public CefApp, public CefBrowserProcessHandler {
public:
    OSRCefApp();

private:
    IMPLEMENT_REFCOUNTING(OSRCefApp);
};
