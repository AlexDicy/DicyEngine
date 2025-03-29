#pragma once
#include "cef_v8.h"

class FrameInfoHandler : public CefV8Handler {
public:
    FrameInfoHandler() = default;

    bool Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retVal, CefString& exception) override;

    IMPLEMENT_REFCOUNTING(FrameInfoHandler);
};
