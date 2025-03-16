#include "pch/enginepch.h"
#include "OSRCefHandler.h"

#include "Application.h"

void OSRCefHandler::OnLoadStart(const CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, TransitionType transitionType) {
    this->host = browser->GetHost();
}

void OSRCefHandler::OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const ErrorCode errorCode, const CefString& errorText, const CefString& failedUrl) {
    DE_ERROR("CEF Load Error: {}, {}, {}", static_cast<int>(errorCode), errorText.ToString(), failedUrl.ToString());
}

void OSRCefHandler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) {
    rect = CefRect(0, 0, static_cast<int>(app->getWindow()->getWidth()), static_cast<int>(app->getWindow()->getHeight()));
}

void OSRCefHandler::OnPaint(CefRefPtr<CefBrowser> browser, const PaintElementType type, const RectList& dirtyRects, const void* buffer, const int width, const int height) {
    this->pixelBuffer.resize(static_cast<size_t>(width) * height * 4);
    std::memcpy(this->pixelBuffer.data(), buffer, this->pixelBuffer.size());
    this->updateTexture();
}

void OSRCefHandler::updateTexture() const {
    if (this->texture->getWidth() != app->getWindow()->getWidth() || this->texture->getHeight() != app->getWindow()->getHeight()) {
        this->texture->resize(app->getWindow()->getWidth(), app->getWindow()->getHeight());
    }
    this->texture->setRawData(this->pixelBuffer.data());
}
