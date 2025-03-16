#pragma once
#include "cef_client.h"
#include "utils/Logging.h"

class RenderHandler : public CefRenderHandler {
public:
    void GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) override {
        rect = CefRect(0, 0, 1920, 1080);
    }

    void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& dirtyRects, const void* buffer, int width, int height) override {
        DE_INFO("OnPaint");
    }

    IMPLEMENT_REFCOUNTING(RenderHandler);
};
