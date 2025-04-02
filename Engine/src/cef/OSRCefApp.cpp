#include "OSRCefApp.h"

#include "CefMessageHandler.h"

OSRCefApp::OSRCefApp() = default;

void OSRCefApp::OnBeforeCommandLineProcessing(const CefString& processType, const CefRefPtr<CefCommandLine> commandLine) {
    commandLine->AppendSwitch("disable-gpu-compositing");
    commandLine->AppendSwitchWithValue("disable-features",
                                       "WebBluetooth,"
#ifdef DE_PLATFORM_WINDOWS
                                       "EnableWindowsGamingInputDataFetcher,"
#endif
                                       "HardwareMediaKeyHandling");
#ifdef DE_PLATFORM_MACOS
    commandLine->AppendSwitch("use-mock-keychain");
    commandLine->AppendSwitch("disable-gpu"); // TODO: test without
#endif
}

void OSRCefApp::OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefRefPtr<CefV8Context> context) {
    const CefRefPtr<CefV8Value> object = context->GetGlobal();
    object->SetValue("setMessageListener", CefV8Value::CreateFunction("setMessageListener", this->messageHandler), V8_PROPERTY_ATTRIBUTE_NONE);
}

void OSRCefApp::OnContextReleased(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefRefPtr<CefV8Context> context) {
    this->messageHandler->releaseContext(context);
}

bool OSRCefApp::OnProcessMessageReceived(const CefRefPtr<CefBrowser> browser, const CefRefPtr<CefFrame> frame, const CefProcessId sourceProcess,
                                         const CefRefPtr<CefProcessMessage> message) {
    return this->messageHandler->processMessage(browser, frame, sourceProcess, message);
}
