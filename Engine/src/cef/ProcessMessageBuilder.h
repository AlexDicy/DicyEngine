#pragma once
#include "cef_v8.h"

class MessageArguments {
public:
    MessageArguments();

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

    void appendArguments(const MessageArguments& args) {
        arguments->SetList(index++, args.arguments);
    }

    void appendDictionary(const CefRefPtr<CefDictionaryValue>& dict) {
        arguments->SetDictionary(index++, dict);
    }

    void appendEachArgument(const MessageArguments& args);

protected:
    explicit MessageArguments(bool initializeArguments);

    unsigned int index = 0;
    CefRefPtr<CefListValue> arguments;
};

class ProcessMessageBuilder : public MessageArguments {
    friend class BrowserMessageHandler;

public:
    ProcessMessageBuilder(const std::string& messageType, const std::string& name);

    CefRefPtr<CefProcessMessage> getMessage() const {
        return message;
    }

private:
    CefRefPtr<CefProcessMessage> message;
};