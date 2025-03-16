#pragma once
#include "cef_app.h"

class OSRCefApp : public CefApp, public CefBrowserProcessHandler {
public:
    OSRCefApp();

    void OnBeforeCommandLineProcessing(const CefString& processType, CefRefPtr<CefCommandLine> commandLine) override;

private:
    IMPLEMENT_REFCOUNTING(OSRCefApp);
};
