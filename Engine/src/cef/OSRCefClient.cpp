#include "pch/enginepch.h"
#include "OSRCefClient.h"

#include "OSRCefApp.h"
#include "OSRCefHandler.h"

#include <cef_app.h>

void OSRCefClient::start() {
    const CefMainArgs mainArgs;
    CefRefPtr<OSRCefApp> app;

    // CEF applications have multiple sub-processes (render, GPU, etc.) that share
    // the same executable. This function checks the command-line and, if this is
    // a sub-process, executes the appropriate logic.
    const int exitCode = CefExecuteProcess(mainArgs, app, nullptr);
    if (exitCode >= 0) {
        // The sub-process has completed so return here.
        return;
    }

    CefSettings settings;
    settings.no_sandbox = true;
    settings.multi_threaded_message_loop = false;
    settings.windowless_rendering_enabled = true;

    // Initialize the CEF browser process. May return false if initialization
    // fails or if early exit is desired (for example, due to process singleton
    // relaunch behavior).
    if (!CefInitialize(mainArgs, settings, app.get(), nullptr)) {
        DE_INFO("CEF Browser process exited with code: {}", CefGetExitCode());
        return;
    }

    // Instruct CEF to not render to a window.
    CefWindowInfo windowInfo;
    windowInfo.SetAsWindowless(nullptr);

    CefBrowserSettings browserSettings;
    this->handler = CefRefPtr(new OSRCefHandler());
    // Start up the browser instance.
    CefBrowserHost::CreateBrowser(windowInfo, handler, "https://www.google.com", browserSettings, nullptr, nullptr);
}
