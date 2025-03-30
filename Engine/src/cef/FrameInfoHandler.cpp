#include "pch/enginepch.h"
#include "FrameInfoHandler.h"

bool FrameInfoHandler::Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retVal, CefString& exception) {
    if (name == "register") {
        if (arguments.size() == 1 && arguments[0]->IsFunction()) {
            callback = arguments[0];
            callbackContext = CefV8Context::GetCurrentContext();
            return true;
        }
    }
    return false;
}

void FrameInfoHandler::updateFrameInfo(const double deltaTime) const {
    if (!callback || !callbackContext) {
        return;
    }

    CefV8ValueList arguments;
    arguments.push_back(CefV8Value::CreateDouble(deltaTime));
    this->callback->ExecuteFunctionWithContext(this->callbackContext, nullptr, arguments);
}
