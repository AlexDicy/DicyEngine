#include "pch/enginepch.h"
#include "ProcessMessageBuilder.h"

ProcessMessageBuilder::ProcessMessageBuilder(const std::string& messageType, const std::string& name) : message(CefProcessMessage::Create(messageType)) {
    arguments = message->GetArgumentList();
    arguments->SetString(index++, name);
}

void ProcessMessageBuilder::appendArguments(const CefRefPtr<CefListValue>& args) {
    const int size = static_cast<int>(args->GetSize());
    arguments->SetSize(size + index);
    for (int i = 0; i < size; i++) {
        arguments->SetValue(index++, args->GetValue(i));
    }
}
