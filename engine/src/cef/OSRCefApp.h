﻿#pragma once
#include "RendererMessageHandler.h"
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
    void OnContextReleased(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) override;

    bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefProcessId sourceProcess, CefRefPtr<CefProcessMessage> message) override;

private:
    CefRefPtr<RendererMessageHandler> messageHandler = new RendererMessageHandler();

    IMPLEMENT_REFCOUNTING(OSRCefApp);
};
