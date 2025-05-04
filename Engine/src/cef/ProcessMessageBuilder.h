#pragma once
#include "cef_v8.h"

class MessageDictionary {
    friend class MessageList;

public:
    MessageDictionary();

    void setBool(const std::string& key, const bool value) const {
        dictionary->SetBool(key, value);
    }

    void setInt(const std::string& key, const int value) const {
        dictionary->SetInt(key, value);
    }

    void setDouble(const std::string& key, const double value) const {
        dictionary->SetDouble(key, value);
    }

    void setString(const std::string& key, const std::string& value) const {
        dictionary->SetString(key, value);
    }

    void setList(const std::string& key, const MessageList& value) const;

    void setDictionary(const std::string& key, const MessageDictionary& value) const {
        dictionary->SetDictionary(key, value.dictionary);
    }

protected:
    CefRefPtr<CefDictionaryValue> dictionary;
};

class MessageList {
    friend class MessageDictionary;
    friend class MessageArguments;

public:
    MessageList();

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

    void appendList(const MessageList& args) {
        arguments->SetList(index++, args.arguments);
    }

    void appendDictionary(const MessageDictionary& dictionary) {
        arguments->SetDictionary(index++, dictionary.dictionary);
    }

protected:
    explicit MessageList(bool initializeArguments);

    unsigned int index = 0;
    CefRefPtr<CefListValue> arguments;
};

class MessageArguments : public MessageList {
public:
    MessageArguments() : MessageList(true) {}

    void appendEachArgument(const MessageArguments& args);

protected:
    explicit MessageArguments(bool initializeArguments);
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