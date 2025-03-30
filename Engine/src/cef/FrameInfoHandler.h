#pragma once
#include "cef_v8.h"

class FrameInfoHandler : public CefV8Handler {
public:
    FrameInfoHandler() = default;

    bool Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retVal, CefString& exception) override;

    void updateFrameInfo(double deltaTime) const;

private:
    CefRefPtr<CefV8Value> callback;
    CefRefPtr<CefV8Context> callbackContext;

    IMPLEMENT_REFCOUNTING(FrameInfoHandler);
};
