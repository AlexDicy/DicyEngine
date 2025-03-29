#include "pch/enginepch.h"
#include "FrameInfoHandler.h"

bool FrameInfoHandler::Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retVal, CefString& exception) {
    if (name == "register") {
        if (arguments.size() == 1 && arguments[0]->IsFunction()) {
            // callback_func_ = arguments[0];
            // callback_context_ = CefV8Context::GetCurrentContext();
            return true;
        }
    }
    return false;
}
