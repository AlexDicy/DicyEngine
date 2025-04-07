#include "RendererMessageHandler.h"

#include <iostream>

bool RendererMessageHandler::Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retVal, CefString& exception) {
    if (name == "cefCall") {
        // call(functionName, onSuccess, onError, args...)
        if (arguments.size() > 2 && arguments[0]->IsString() && arguments[1]->IsFunction() && arguments[2]->IsFunction()) {
            const CefRefPtr<CefV8Context> context = CefV8Context::GetCurrentContext();
            CallPromise promise;
            promise.context = context;
            promise.resolve = arguments[1];
            promise.reject = arguments[2];
            const CefRefPtr<CefBrowser> browser = context->GetBrowser();
            const int browserId = browser->GetIdentifier();
            if (this->callId >= INT_MAX) {
                this->callId = INT_MIN;
            }
            const int callId = this->callId++;
            this->calls.insert(std::make_pair(std::make_pair(browserId, callId), promise));

            const CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create(callName);
            const CefRefPtr<CefListValue> list = message->GetArgumentList();
            const int size = static_cast<int>(arguments.size());
            list->SetSize(size - 2);
            list->SetString(0, arguments[0]->GetStringValue());
            list->SetInt(1, callId);
            for (int i = 2; i < size; i++) {
                if (arguments[i]->IsValid()) {
                    setListValue(list, i, arguments[i]);
                }
            }
            browser->GetMainFrame()->SendProcessMessage(PID_BROWSER, message);
        }
    } else if (name == "setMessageListener") {
        if (arguments.size() == 2 && arguments[0]->IsString() && arguments[1]->IsFunction()) {
            std::string messageName = arguments[0]->GetStringValue();
            CefRefPtr<CefV8Context> context = CefV8Context::GetCurrentContext();
            int browserId = context->GetBrowser()->GetIdentifier();
            this->callbacks.insert(std::make_pair(std::make_pair(messageName, browserId), std::make_pair(context, arguments[1])));
            return true;
        }
    }
    return false;
}

void RendererMessageHandler::releaseContext(const CefRefPtr<CefV8Context>& context) {
    // Remove any JavaScript callbacks registered for the context that has been released.
    if (!this->callbacks.empty()) {
        for (auto it = this->callbacks.begin(); it != this->callbacks.end();) {
            if (it->second.first->IsSame(context)) {
                this->callbacks.erase(it++);
            } else {
                ++it;
            }
        }
    }
    // Remove any pending calls for the context that has been released.
    if (!this->calls.empty()) {
        for (auto it = this->calls.begin(); it != this->calls.end();) {
            if (it->second.context->IsSame(context)) {
                this->calls.erase(it++);
            } else {
                ++it;
            }
        }
    }
}

void RendererMessageHandler::setListValue(const CefRefPtr<CefListValue>& list, const size_t index, const CefRefPtr<CefV8Value>& value) {
    if (value->IsArray()) {
        const CefRefPtr<CefListValue> newList = CefListValue::Create();
        list->SetList(index, newList);
    } else if (value->IsString()) {
        list->SetString(index, value->GetStringValue());
    } else if (value->IsBool()) {
        list->SetBool(index, value->GetBoolValue());
    } else if (value->IsInt()) {
        list->SetInt(index, value->GetIntValue());
    } else if (value->IsDouble()) {
        list->SetDouble(index, value->GetDoubleValue());
    }
}

void RendererMessageHandler::setList(const CefV8ValueList& source, const CefRefPtr<CefListValue>& target) {
    const size_t argLength = source.size();
    if (argLength == 0) {
        return;
    }

    target->SetSize(argLength);

    for (size_t i = 0; i < argLength; i++) {
        setListValue(target, i, source[i]);
    }
}

void RendererMessageHandler::setListValue(const CefRefPtr<CefV8Value>& list, const int index, const CefRefPtr<CefListValue>& value) {
    CefRefPtr<CefV8Value> newValue;

    switch (value->GetType(index)) {
        case VTYPE_LIST:
            {
                const CefRefPtr<CefListValue> subList = value->GetList(index);
                newValue = CefV8Value::CreateArray(static_cast<int>(subList->GetSize()));
                setList(subList, newValue);
            }
            break;
        case VTYPE_BOOL:
            newValue = CefV8Value::CreateBool(value->GetBool(index));
            break;
        case VTYPE_DOUBLE:
            newValue = CefV8Value::CreateDouble(value->GetDouble(index));
            break;
        case VTYPE_INT:
            newValue = CefV8Value::CreateInt(value->GetInt(index));
            break;
        case VTYPE_STRING:
            newValue = CefV8Value::CreateString(value->GetString(index));
            break;
        case VTYPE_INVALID:
        case VTYPE_NULL:
            newValue = CefV8Value::CreateNull();
            break;
        case VTYPE_BINARY:
        case VTYPE_DICTIONARY:
        case VTYPE_NUM_VALUES:
            newValue = CefV8Value::CreateString(CefString("[unrecognized type]"));
            break;
    }

    if (newValue.get()) {
        list->SetValue(index, newValue);
    } else {
        list->SetValue(index, CefV8Value::CreateNull());
    }
}

void RendererMessageHandler::setList(const CefRefPtr<CefListValue>& source, const CefRefPtr<CefV8Value>& target) {
    assert(target->IsArray());

    const int argLength = static_cast<int>(source->GetSize());
    if (argLength == 0) {
        return;
    }

    for (int i = 0; i < argLength; i++) {
        setListValue(target, i, source);
    }
}

bool RendererMessageHandler::processMessage(const CefRefPtr<CefBrowser>& browser, const CefRefPtr<CefFrame>& frame, CefProcessId sourceProcess,
                                            const CefRefPtr<CefProcessMessage>& message) {
    const CefString& messageName = message->GetName();
    if (messageName == callName) {
        return processMessageForCall(browser, message);
    }
    if (messageName == callErrorName) {
        return processMessageForCallError(browser, message);
    }
    if (messageName == messageListenerName) {
        return processMessageForListener(browser, message);
    }
    return false;
}

bool RendererMessageHandler::processMessageForCall(const CefRefPtr<CefBrowser>& browser, const CefRefPtr<CefProcessMessage>& message) {
    const CefRefPtr<CefListValue> messageArguments = message->GetArgumentList();
    const int callId = messageArguments->GetInt(1);

    const auto it = this->calls.find(std::make_pair(browser->GetIdentifier(), callId));
    if (it != this->calls.end()) {
        const CefRefPtr<CefV8Context> context = it->second.context;
        const CefRefPtr<CefV8Value> callback = it->second.resolve;

        context->Enter();
        CefV8ValueList callbackArguments;
        const int size = static_cast<int>(messageArguments->GetSize());
        const CefRefPtr<CefV8Value> args = CefV8Value::CreateArray(size);
        setList(messageArguments, args);
        callbackArguments.reserve(size - 2);
        for (int i = 2; i < size; i++) {
            callbackArguments.push_back(args->GetValue(i));
        }
        callback->ExecuteFunction(nullptr, callbackArguments);
        context->Exit();

        this->calls.erase(it);
        return true;
    }
    return false;
}

bool RendererMessageHandler::processMessageForCallError(const CefRefPtr<CefBrowser>& browser, const CefRefPtr<CefProcessMessage>& message) {
    const CefRefPtr<CefListValue> messageArguments = message->GetArgumentList();
    const int callId = messageArguments->GetInt(1);

    const auto it = this->calls.find(std::make_pair(browser->GetIdentifier(), callId));
    if (it != this->calls.end()) {
        const CefRefPtr<CefV8Context> context = it->second.context;
        const CefRefPtr<CefV8Value> callback = it->second.reject;

        context->Enter();
        CefV8ValueList arguments;
        arguments.push_back(CefV8Value::CreateString(messageArguments->GetString(2)));
        callback->ExecuteFunction(nullptr, arguments);
        context->Exit();

        this->calls.erase(it);
        return true;
    }
    return false;
}

bool RendererMessageHandler::processMessageForListener(const CefRefPtr<CefBrowser>& browser, const CefRefPtr<CefProcessMessage>& message) {
    const CefRefPtr<CefListValue> list = message->GetArgumentList();
    const CefString& listenerName = list->GetString(0);

    const auto it = this->callbacks.find(std::make_pair(listenerName.ToString(), browser->GetIdentifier()));
    if (it != this->callbacks.end()) {
        // Keep a local reference to the objects. The callback may remove itself from the callback map.
        const CefRefPtr<CefV8Context> context = it->second.first;
        const CefRefPtr<CefV8Value> callback = it->second.second;

        context->Enter();

        CefV8ValueList arguments;
        const int size = static_cast<int>(list->GetSize());
        const CefRefPtr<CefV8Value> args = CefV8Value::CreateArray(size);
        setList(list, args);
        arguments.reserve(list->GetSize() - 1);
        for (int i = 1; i < size; i++) {
            arguments.push_back(args->GetValue(i));
        }

        const CefRefPtr<CefV8Value> retVal = callback->ExecuteFunction(nullptr, arguments);
        if (retVal.get() && retVal->IsBool()) {
            return retVal->GetBoolValue();
        }

        context->Exit();
    }
    return false;
}
