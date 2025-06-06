﻿#pragma once
#include "Application.h"
#include "BrowserMessageHandler.h"
#include "OSRCefApp.h"

#include <queue>

class OSRCefHandler : public CefClient, public CefRenderHandler, public CefLoadHandler, public CefLifeSpanHandler {
public:
    friend class BrowserMessageHandler;

    explicit OSRCefHandler(const Ref<Application>& app);
    ~OSRCefHandler() override;

    void setTexture(const Ref<Texture2D>& texture) {
        this->texture = texture;
    }

    const CefRefPtr<CefBrowserHost>& getHost() const {
        return this->host;
    }

    bool isClosing() const {
        return this->closing;
    }

    void showMainWindow();
    void closeAllBrowsers(bool force);

    void sendWindowResizeEvent(const WindowResizeEvent& event) const;
    void sendMouseMoveEvent(const MouseMovedEvent& event) const;
    void sendMouseButtonPressedEvent(const MouseButtonPressedEvent& event) const;
    void sendMouseButtonReleasedEvent(const MouseButtonReleasedEvent& event) const;
    void sendMouseScrolledEvent(const MouseScrolledEvent& event) const;
    void sendKeyPressedEvent(const KeyPressedEvent& event) const;
    void sendKeyReleasedEvent(const KeyReleasedEvent& event) const;
    void sendCharTypedEvent(const CharTypedEvent& event) const;

    void registerCallback(const std::string& name, const std::function<void(const Callback&)>& callback) {
        this->browserMessageHandler.registerCallback(name, callback);
    }

    void updateFrameInfo(double deltaTime) const;
    void updateProfilingInfo() const;

    void processMainThreadTasks();

#ifdef DE_PLATFORM_WINDOWS
    void updateTextureIfNeeded();
#endif

    //
    // CEF overrides
    //
    CefRefPtr<CefRenderHandler> GetRenderHandler() override {
        return this;
    }

    CefRefPtr<CefLoadHandler> GetLoadHandler() override {
        return this;
    }

    CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override {
        return this;
    }

    bool GetScreenPoint(CefRefPtr<CefBrowser> browser, const int viewX, const int viewY, int& screenX, int& screenY) override {
        screenX = viewX;
        screenY = viewY;
        return true;
    }

    void OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, TransitionType transitionType) override;
    void OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode, const CefString& errorText, const CefString& failedUrl) override;

    bool GetScreenInfo(CefRefPtr<CefBrowser> browser, CefScreenInfo& screenInfo) override;
    void GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) override;
    void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& dirtyRects, const void* buffer, int width, int height) override;

    bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefProcessId sourceProcess, CefRefPtr<CefProcessMessage> message) override;

    bool DoClose(CefRefPtr<CefBrowser> browser) override;

    static OSRCefHandler* getInstance();

private:
    int getCoordinate(unsigned int rawValue) const;
    int getCoordinate(int rawValue) const;
    int getCoordinate(float rawValue) const;
    int getMouseCoordinate(float rawValue) const;

    void queueTaskForMainThread(const std::function<void()>& task);

    static uint32_t getKeyModifiers(const KeyEvent& event);
    static cef_mouse_button_type_t getMouseButtonType(InputCode code);
    static uint32_t getMouseModifiers();

    Ref<Application> app;
#ifdef DE_PLATFORM_WINDOWS
    struct {
        uint8_t* buffer = nullptr;
        unsigned int bufferSize = 0;
        unsigned int width = 0;
        unsigned int height = 0;
        bool needsUpdate = false;
    } textureInfo;
    std::mutex textureInfoMutex;
#endif
    Ref<Texture2D> texture;
    CefRefPtr<CefBrowserHost> host;
    BrowserMessageHandler browserMessageHandler = BrowserMessageHandler();
    bool closing = false;

    std::queue<std::function<void()>> mainThreadTasks;
    std::mutex taskQueueMutex;

    IMPLEMENT_REFCOUNTING(OSRCefHandler);
};
