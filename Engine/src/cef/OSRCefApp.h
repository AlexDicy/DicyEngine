#pragma once
#include "cef_app.h"

class OSRCefApp : public CefApp, public CefBrowserProcessHandler, public CefRenderProcessHandler {
public:
    OSRCefApp();

    CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override {
        return this;
    }

    void OnBeforeCommandLineProcessing(const CefString& processType, CefRefPtr<CefCommandLine> commandLine) override;
    void OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) override;

private:
    IMPLEMENT_REFCOUNTING(OSRCefApp);
};
