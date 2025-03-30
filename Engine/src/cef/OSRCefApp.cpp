#include "OSRCefApp.h"

#include "FrameInfoHandler.h"

OSRCefApp::OSRCefApp() = default;

void OSRCefApp::OnBeforeCommandLineProcessing(const CefString& processType, const CefRefPtr<CefCommandLine> commandLine) {
    commandLine->AppendSwitch("disable-gpu-compositing");
}

void OSRCefApp::OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) {
    CefRefPtr<CefV8Value> object = context->GetGlobal();

    CefRefPtr<CefV8Handler> handler = new FrameInfoHandler();
    object->SetValue("register", CefV8Value::CreateFunction("register", handler), V8_PROPERTY_ATTRIBUTE_NONE);
}
