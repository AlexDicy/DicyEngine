#pragma once
#include "Application.h"
#include "OSRCefApp.h"

class OSRCefHandler : public CefClient, public CefRenderHandler, public CefLoadHandler {
public:
    explicit OSRCefHandler(const Ref<Application>& app) : app(app) {
        this->pixelBuffer.resize(static_cast<size_t>(app->getWindow()->getWidth()) * app->getWindow()->getHeight() * 4);
    }

    void setTexture(const Ref<Texture2D>& texture) {
        this->texture = texture;
    }

    //
    // CEF overrides
    //
    CefRefPtr<CefRenderHandler> GetRenderHandler() override {
        return this;
    }

    CefRefPtr<CefLoadHandler> GetLoadHandler() override {
        return this;
    }

    void OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, TransitionType transitionType) override;
    void OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode, const CefString& errorText, const CefString& failedUrl) override;

    void GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) override;
    void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& dirtyRects, const void* buffer, int width, int height) override;

private:
    void updateTexture() const;

    Ref<Application> app;
    Ref<Texture2D> texture;
    CefRefPtr<CefBrowserHost> host;
    std::vector<unsigned char> pixelBuffer;

    IMPLEMENT_REFCOUNTING(OSRCefHandler);
};
