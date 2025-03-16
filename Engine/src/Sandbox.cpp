#include "pch/enginepch.h"
#include "Application.h"
#include "cef/OSRCefApp.h"
#include "cef/OSRCefHandler.h"

int main() {
    const CefMainArgs mainArgs;
    CefRefPtr<CefApp> osrApp = new OSRCefApp();

    // CEF applications have multiple sub-processes (render, GPU, etc.) that share
    // the same executable. This function checks the command-line and, if this is
    // a sub-process, executes the appropriate logic.
    int exitCode = CefExecuteProcess(mainArgs, osrApp, nullptr);
    if (exitCode >= 0) {
        DE_INFO("CEF Sub-process exited with code: {}", exitCode);
        // The sub-process has completed so return here.
        return exitCode;
    }

    CefSettings settings;
    settings.no_sandbox = true;
    // settings.multi_threaded_message_loop = false;
    settings.windowless_rendering_enabled = true;

    // Initialize the CEF browser process. May return false if initialization
    // fails or if early exit is desired (for example, due to process singleton
    // relaunch behavior).
    if (!CefInitialize(mainArgs, settings, osrApp.get(), nullptr)) {
        exitCode = CefGetExitCode();
        DE_INFO("CEF Browser process exited with code: {}", exitCode);
        return exitCode;
    }

    // Instruct CEF to not render to a window.
    CefWindowInfo windowInfo;
    windowInfo.SetAsWindowless(nullptr);

    CefBrowserSettings browserSettings;

    const auto app = std::make_shared<Application>();
    app->initialize();

    CefRefPtr handler(new OSRCefHandler(app));
    // Start up the browser instance.
    CefBrowserHost::CreateBrowser(windowInfo, handler, "https://www.google.com", browserSettings, nullptr, nullptr);

    app->run();

    // Shut down CEF.
    CefShutdown();
    return 0;
}
