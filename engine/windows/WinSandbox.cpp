#include "pch/enginepch.h"
#include "Application.h"
#include "cef/OSRCefApp.h"
#include <iostream>
#include <filesystem>


int main(const int argc, char* argv[]) {
    const CefMainArgs mainArgs;
    const CefRefPtr<CefApp> osrApp = new OSRCefApp();

    int subExitCode = CefExecuteProcess(mainArgs, osrApp, nullptr);
    if (subExitCode >= 0) {
        DE_INFO("CEF Sub-process exited with code: {}", subExitCode);
        // The sub-process has completed so return here.
        return subExitCode;
    }

    const auto app = std::make_shared<Application>(argc, argv);
    app->initialize();
    app->run();
    return 0;
}
