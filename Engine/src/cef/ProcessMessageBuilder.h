#pragma once
#include "cef_v8.h"

class ProcessMessageBuilder {
    friend class BrowserMessageHandler;

public:
    ProcessMessageBuilder(const std::string& messageType, const std::string& name);

    CefRefPtr<CefProcessMessage> getMessage() const {
        return message;
    }

    void appendBool(const bool value) {
        arguments->SetBool(index++, value);
    }

    void appendInt(const int value) {
        arguments->SetInt(index++, value);
    }

    void appendDouble(const double value) {
        arguments->SetDouble(index++, value);
    }

    void appendString(const std::string& value) {
        arguments->SetString(index++, value);
    }

    void appendArguments(const CefRefPtr<CefListValue>& args);

private:
    CefRefPtr<CefProcessMessage> message;
    CefRefPtr<CefListValue> arguments;
    unsigned int index = 0;
};