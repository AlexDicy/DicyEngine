#include "pch/enginepch.h"
#include "BrowserMessageHandler.h"
#include "RendererMessageHandler.h"

void BrowserMessageHandler::registerCallback(const std::string& name, const std::function<void(const Callback& callback)>& callback) {
    callbacks[name] = callback;
}

bool BrowserMessageHandler::processMessage(const CefRefPtr<CefBrowser>& browser, const CefRefPtr<CefFrame>& frame, CefProcessId sourceProcess,
                                           const CefRefPtr<CefProcessMessage>& message) {
    if (message->GetName() == RendererMessageHandler::callName) {
        const CefRefPtr<CefListValue> arguments = message->GetArgumentList();
        const std::string name = arguments->GetString(0);
        const int callId = arguments->GetInt(1);
        const CefRefPtr<CefListValue> args = arguments->GetList(2);

        const auto it = this->callbacks.find(name);
        if (it != this->callbacks.end()) {
            const Callback callback(name, callId, browser, args);
            it->second(callback);
            return true;
        }
        Callback(name, callId, browser, args).error("No callback registered for message: " + name);
    }
    return false;
}

Ref<ProcessMessageBuilder> BrowserMessageHandler::createMessage(const std::string& name) {
    return std::make_shared<ProcessMessageBuilder>(RendererMessageHandler::messageListenerName, name);
}

void Callback::success() const {
    this->sendMessage(this->getMessageBuilder(RendererMessageHandler::callName));
}

void Callback::success(const bool value) const {
    auto builder = this->getMessageBuilder(RendererMessageHandler::callName);
    builder.appendBool(value);
    this->sendMessage(builder);
}

void Callback::success(const CefRefPtr<CefListValue>& arguments) const {
    auto builder = this->getMessageBuilder(RendererMessageHandler::callName);
    builder.appendArguments(arguments);
    this->sendMessage(builder);
}

void Callback::error(const std::string& error) const {
    auto builder = this->getMessageBuilder(RendererMessageHandler::callErrorName);
    builder.appendString(error);
    this->sendMessage(builder);
}

int Callback::getInt(const int index) const {
    return args->GetInt(index);
}

ProcessMessageBuilder Callback::getMessageBuilder(const std::string& messageName) const {
    auto message = ProcessMessageBuilder(messageName, name);
    message.appendInt(callId);
    return message;
}

void Callback::sendMessage(const ProcessMessageBuilder& builder) const {
    this->browser->GetMainFrame()->SendProcessMessage(PID_RENDERER, builder.getMessage());
}
