#pragma once
#include "OSRCefApp.h"

OSRCefApp::OSRCefApp() = default;

void OSRCefApp::OnBeforeCommandLineProcessing(const CefString& processType, const CefRefPtr<CefCommandLine> commandLine) {
    commandLine->AppendSwitch("disable-gpu-compositing");
}
