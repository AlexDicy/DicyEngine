#pragma once
#include "cef_v8.h"
#include <functional>

#include "utils/Common.h"


class ProcessMessageBuilder;

class BrowserMessageHandler {
    friend class OSRCefHandler;

public:
    void registerCallback(const std::string& name, const std::function<void(const CefRefPtr<CefListValue>&)>& callback);

protected:
    BrowserMessageHandler() = default;

    bool processMessage(const CefRefPtr<CefBrowser>& browser, const CefRefPtr<CefFrame>& frame, CefProcessId sourceProcess, const CefRefPtr<CefProcessMessage>& message);

    static Ref<ProcessMessageBuilder> createMessage(const std::string& name);

private:
    std::unordered_map<std::string, std::function<void(const CefRefPtr<CefListValue>&)>> callbacks;
};

class ProcessMessageBuilder {
    friend class BrowserMessageHandler;
public:
    ProcessMessageBuilder(const std::string& messageType, const std::string& name) : message(CefProcessMessage::Create(messageType)) {
        arguments = message->GetArgumentList();
        arguments->SetString(index++, name);
    }

    CefRefPtr<CefProcessMessage> getMessage() const {
        return message;
    }

    void appendDouble(const double value) {
        arguments->SetDouble(index++, value);
    }

private:
    CefRefPtr<CefProcessMessage> message;
    CefRefPtr<CefListValue> arguments;
    unsigned int index = 0;
};
