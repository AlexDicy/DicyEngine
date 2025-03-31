#pragma once
#include "cef_v8.h"

class CefMessageHandler : public CefV8Handler {
    friend class OSRCefApp;

protected:
    CefMessageHandler() = default;

    bool Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retVal, CefString& exception) override;
    bool processMessage(const CefRefPtr<CefBrowser>& browser, const CefRefPtr<CefFrame>& frame, CefProcessId sourceProcess, const CefRefPtr<CefProcessMessage>& message);
    void releaseContext(const CefRefPtr<CefV8Context>& context);

private:
    static void setListValue(const CefRefPtr<CefListValue>& list, int index, const CefRefPtr<CefV8Value>& value);
    static void setListValue(const CefRefPtr<CefV8Value>& list, int index, const CefRefPtr<CefListValue>& value);
    static void setList(const CefRefPtr<CefV8Value>& source, const CefRefPtr<CefListValue>& target);
    static void setList(const CefRefPtr<CefListValue>& source, const CefRefPtr<CefV8Value>& target);

    std::map<std::pair<std::string, int>, std::pair<CefRefPtr<CefV8Context>, CefRefPtr<CefV8Value>>> callbacks;

    IMPLEMENT_REFCOUNTING(CefMessageHandler);
};
