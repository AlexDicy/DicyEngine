﻿#pragma once
#include "Application.h"
#include "OSRCefApp.h"

class OSRCefHandler : public CefClient, public CefRenderHandler, public CefLoadHandler {
public:
    explicit OSRCefHandler(const Ref<Application>& app) : app(app) {}

    void setTexture(const Ref<Texture2D>& texture) {
        this->texture = texture;
    }

    const CefRefPtr<CefBrowserHost>& getHost() const {
        return this->host;
    }

    void sendWindowResizeEvent(const WindowResizeEvent& event) const;
    void sendMouseMoveEvent(const MouseMovedEvent& event) const;
    void sendMouseButtonPressedEvent(const MouseButtonPressedEvent& event) const;
    void sendMouseButtonReleasedEvent(const MouseButtonReleasedEvent& event) const;
    void sendMouseScrolledEvent(const MouseScrolledEvent& event) const;
    void sendKeyPressedEvent(const KeyPressedEvent& event) const;
    void sendKeyReleasedEvent(const KeyReleasedEvent& event) const;
    void sendCharTypedEvent(const CharTypedEvent& event) const;

    //
    // CEF overrides
    //
    CefRefPtr<CefRenderHandler> GetRenderHandler() override {
        return this;
    }

    CefRefPtr<CefLoadHandler> GetLoadHandler() override {
        return this;
    }

    bool GetScreenPoint(CefRefPtr<CefBrowser> browser, const int viewX, const int viewY, int& screenX, int& screenY) override {
        screenX = viewX;
        screenY = viewY;
        return true;
    }

    void OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, TransitionType transitionType) override;
    void OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode, const CefString& errorText, const CefString& failedUrl) override;

    void GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) override;
    void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& dirtyRects, const void* buffer, int width, int height) override;

private:
    void updateTexture(const void* buffer, unsigned int width, unsigned int height) const;

    static cef_mouse_button_type_t getMouseButtonType(InputCode code);
    static uint32_t getMouseModifiers();

    Ref<Application> app;
    Ref<Texture2D> texture;
    CefRefPtr<CefBrowserHost> host;

    IMPLEMENT_REFCOUNTING(OSRCefHandler);
};
