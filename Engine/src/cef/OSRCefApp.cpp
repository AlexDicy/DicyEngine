#include "OSRCefApp.h"

#include "FrameInfoHandler.h"

OSRCefApp::OSRCefApp() = default;

void OSRCefApp::OnBeforeCommandLineProcessing(const CefString& processType, const CefRefPtr<CefCommandLine> commandLine) {
    commandLine->AppendSwitch("disable-gpu-compositing");
}

void OSRCefApp::OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefRefPtr<CefV8Context> context) {
    const CefRefPtr<CefV8Value> object = context->GetGlobal();
    object->SetValue("register", CefV8Value::CreateFunction("register", this->frameInfoHandler), V8_PROPERTY_ATTRIBUTE_NONE);
}
