#pragma once
#include "OSRCefApp.h"

class OSRCefHandler : public CefClient, public CefRenderHandler, public CefLoadHandler {
public:
    CefRefPtr<CefLoadHandler> GetLoadHandler() override {
        return this;
    }

    void OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, TransitionType transitionType) override;
    void OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode, const CefString& errorText, const CefString& failedUrl) override;

    void GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) override;
    void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& dirtyRects, const void* buffer, int width, int height) override;

private:
    void updateTexture();

    Ref<Application> app;
    CefRefPtr<CefBrowserHost> host;
    std::vector<unsigned char> pixelBuffer;
    std::vector<unsigned char> popupPixelBuffer;

    IMPLEMENT_REFCOUNTING(OSRCefHandler);
};
