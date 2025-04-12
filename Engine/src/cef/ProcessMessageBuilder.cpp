#include "pch/enginepch.h"
#include "ProcessMessageBuilder.h"

MessageArguments::MessageArguments() : MessageArguments(true) {}

MessageArguments::MessageArguments(const bool initializeArguments) {
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

ProcessMessageBuilder::ProcessMessageBuilder(const std::string& messageType, const std::string& name) : MessageArguments(false), message(CefProcessMessage::Create(messageType)) {
    this->arguments = message->GetArgumentList();
    this->arguments->SetString(this->index++, name);
}
