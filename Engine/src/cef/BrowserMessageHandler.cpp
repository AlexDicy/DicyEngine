#include "BrowserMessageHandler.h"

void BrowserMessageHandler::registerCallback(const std::string& name, const std::function<void(const CefRefPtr<CefListValue>&)>& callback) {
    callbacks[name] = callback;
}

bool BrowserMessageHandler::processMessage(const CefRefPtr<CefBrowser>& browser, const CefRefPtr<CefFrame>& frame, CefProcessId sourceProcess,
                                           const CefRefPtr<CefProcessMessage>& message) {
    const CefString& messageName = message->GetName();
    const auto it = this->callbacks.find(messageName.ToString());
    if (it != this->callbacks.end()) {
        it->second(message->GetArgumentList());
        return true;
    }
    return false;
}
