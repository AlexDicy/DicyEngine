#pragma once
#include "cef_v8.h"

#include <unordered_map>

class RendererMessageHandler : public CefV8Handler {
    friend class OSRCefApp;

public:
    static inline const std::string messageListenerName = "__messageListener";
    static inline const std::string callName = "__call";
    static inline const std::string callErrorName = "__callError";

protected:
    RendererMessageHandler() = default;

    bool Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retVal, CefString& exception) override;
    bool processMessage(const CefRefPtr<CefBrowser>& browser, const CefRefPtr<CefFrame>& frame, CefProcessId sourceProcess, const CefRefPtr<CefProcessMessage>& message);
    void releaseContext(const CefRefPtr<CefV8Context>& context);

private:
    static void setListValue(const CefRefPtr<CefListValue>& list, size_t index, const CefRefPtr<CefV8Value>& value);
    static void setListValue(const CefRefPtr<CefV8Value>& list, int index, const CefRefPtr<CefListValue>& value);
    static void setList(const CefV8ValueList& source, const CefRefPtr<CefListValue>& target);
    static void setList(const CefRefPtr<CefListValue>& source, const CefRefPtr<CefV8Value>& target);
    static CefRefPtr<CefV8Value> getV8Value(const CefRefPtr<CefValue>& value);

    bool processMessageForCall(const CefRefPtr<CefBrowser>& browser, const CefRefPtr<CefProcessMessage>& message);
    bool processMessageForCallError(const CefRefPtr<CefBrowser>& browser, const CefRefPtr<CefProcessMessage>& message);
    bool processMessageForListener(const CefRefPtr<CefBrowser>& browser, const CefRefPtr<CefProcessMessage>& message);

    struct CallPromise {
        CefRefPtr<CefV8Context> context;
        CefRefPtr<CefV8Value> resolve;
        CefRefPtr<CefV8Value> reject;
    };

    struct PairHash {
        template <typename T1, typename T2>
        std::size_t operator()(const std::pair<T1, T2>& p) const {
            uintmax_t hash = std::hash<T1>{}(p.first);
            hash <<= sizeof(uintmax_t) * 4;
            hash ^= std::hash<T2>{}(p.second);
            return std::hash<uintmax_t>{}(hash);
        }
    };

    std::unordered_map<std::pair<std::string, int>, std::pair<CefRefPtr<CefV8Context>, CefRefPtr<CefV8Value>>, PairHash> callbacks;
    std::unordered_map<std::pair<int, int>, CallPromise, PairHash> calls;
    int callId = 0;

    IMPLEMENT_REFCOUNTING(RendererMessageHandler);
};
