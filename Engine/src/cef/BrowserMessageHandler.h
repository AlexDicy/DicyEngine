#pragma once
#include "cef_v8.h"

class BrowserMessageHandler {
    friend class OSRCefHandler;

public:
    void registerCallback(const std::string& name, const std::function<void(const CefRefPtr<CefListValue>&)>& callback);

protected:
    BrowserMessageHandler() = default;

    bool processMessage(const CefRefPtr<CefBrowser>& browser, const CefRefPtr<CefFrame>& frame, CefProcessId sourceProcess, const CefRefPtr<CefProcessMessage>& message);

private:
    std::unordered_map<std::string, std::function<void(const CefRefPtr<CefListValue>&)>> callbacks;
};
