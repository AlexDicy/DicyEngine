#pragma once
#include "ProcessMessageBuilder.h"

#include <cef_v8.h>
#include <functional>

class Callback;

class BrowserMessageHandler {
    friend class OSRCefHandler;

public:
    void registerCallback(const std::string& name, const std::function<void(const Callback& callback)>& callback);

protected:
    BrowserMessageHandler() = default;

    bool processMessage(const CefRefPtr<CefBrowser>& browser, const CefRefPtr<CefFrame>& frame, CefProcessId sourceProcess, const CefRefPtr<CefProcessMessage>& message);

    static Ref<ProcessMessageBuilder> createMessage(const std::string& name);

private:
    std::unordered_map<std::string, std::function<void(const Callback& callback)>> callbacks;
};

class Callback {
public:
    Callback(std::string name, const int callId, const CefRefPtr<CefBrowser>& browser, const CefRefPtr<CefListValue> args) :
        name(std::move(name)), callId(callId), browser(browser), args(args) {}

    void success() const;
    void success(bool value) const;
    void success(const CefRefPtr<CefListValue>& arguments) const;
    void error(const std::string& error) const;

    int getInt(int index) const;

private:
    ProcessMessageBuilder getMessageBuilder(const std::string& messageName) const;
    void sendMessage(const ProcessMessageBuilder& builder) const;

    std::string name;
    int callId = 0;
    CefRefPtr<CefBrowser> browser;
    CefRefPtr<CefListValue> args;
};
