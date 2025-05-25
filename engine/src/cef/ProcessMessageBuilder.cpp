#include "pch/enginepch.h"
#include "ProcessMessageBuilder.h"

MessageDictionary::MessageDictionary() : dictionary(CefDictionaryValue::Create()) {}

void MessageDictionary::setList(const std::string& key, const MessageList& value) const {
    dictionary->SetList(key, value.arguments);
}

MessageList::MessageList() : MessageList(true) {}

MessageList::MessageList(const bool initializeArguments) {
    if (initializeArguments) {
        this->arguments = CefListValue::Create();
    }
}

void MessageArguments::appendEachArgument(const MessageArguments& args) {
    const int size = static_cast<int>(args.arguments->GetSize());
    this->arguments->SetSize(size + this->index);
    for (int i = 0; i < size; i++) {
        this->arguments->SetValue(this->index++, args.arguments->GetValue(i));
    }
}

MessageArguments::MessageArguments(const bool initializeArguments) : MessageList(initializeArguments) {}

ProcessMessageBuilder::ProcessMessageBuilder(const std::string& messageType, const std::string& name) : MessageArguments(false), message(CefProcessMessage::Create(messageType)) {
    this->arguments = message->GetArgumentList();
    this->arguments->SetString(this->index++, name);
}
