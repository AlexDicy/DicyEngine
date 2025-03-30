#pragma once
#include "FrameInfoHandler.h"
#include "cef_app.h"

class OSRCefApp : public CefApp, public CefBrowserProcessHandler, public CefRenderProcessHandler {
public:
    OSRCefApp();

    CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override {
        return this;
    }

    CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override {
        return this;
    }

    void OnBeforeCommandLineProcessing(const CefString& processType, CefRefPtr<CefCommandLine> commandLine) override;
    void OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) override;

    CefRefPtr<FrameInfoHandler> getFrameInfoHandler() const {
        return frameInfoHandler;
    }

private:
    CefRefPtr<FrameInfoHandler> frameInfoHandler = new FrameInfoHandler();

    IMPLEMENT_REFCOUNTING(OSRCefApp);
};
