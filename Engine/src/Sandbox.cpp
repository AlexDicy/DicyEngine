#include "pch/enginepch.h"
#include "Application.h"
#include "cef/OSRCefApp.h"
#include "cef/OSRCefHandler.h"

#if defined(__APPLE__)
#include "include/wrapper/cef_library_loader.h"
#endif

int main(const int argc, char* argv[]) {
    #if defined(__APPLE__)
    CefScopedLibraryLoader libraryLoader;
    if (!libraryLoader.LoadInMain()) {
        return 1;
    }
    #endif

    const CefMainArgs mainArgs;

    CefRefPtr<CefCommandLine> commandLine = CefCommandLine::CreateCommandLine();
    commandLine->InitFromArgv(argc, argv);

    const CefRefPtr<CefApp> osrApp = new OSRCefApp();

    #ifdef _WIN32
    int subExitCode = CefExecuteProcess(mainArgs, osrApp, nullptr);
    if (subExitCode >= 0) {
        DE_INFO("CEF Sub-process exited with code: {}", subExitCode);
        // The sub-process has completed so return here.
        return subExitCode;
    }
    #endif

    CefSettings settings;
    settings.no_sandbox = true;
    settings.windowless_rendering_enabled = true;

    // Initialize the CEF browser process. May return false if initialization
    // fails or if early exit is desired (for example, due to process singleton
    // relaunch behavior).
    if (!CefInitialize(mainArgs, settings, osrApp.get(), nullptr)) {
        int exitCode = CefGetExitCode();
        DE_INFO("CEF Browser process exited with code: {}", exitCode);
        return exitCode;
    }

    const auto app = std::make_shared<Application>();
    app->initialize();
    app->run();

    // Shut down CEF.
    CefShutdown();
    return 0;
}
