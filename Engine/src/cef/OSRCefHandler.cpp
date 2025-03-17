#include "pch/enginepch.h"
#include "OSRCefHandler.h"

#include "Application.h"

void OSRCefHandler::sendMouseMoveEvent(const MouseMovedEvent& event) const {
    if (this->host == nullptr) {
        return;
    }

    CefMouseEvent mouseEvent;
    mouseEvent.x = static_cast<int>(event.getX());
    mouseEvent.y = static_cast<int>(event.getY());
    mouseEvent.modifiers = getMouseModifiers();
    this->host->SendMouseMoveEvent(mouseEvent, false);
}

void OSRCefHandler::sendMouseButtonPressedEvent(const MouseButtonPressedEvent& event) const {
    if (this->host == nullptr) {
        return;
    }

    CefMouseEvent mouseEvent;
    mouseEvent.x = static_cast<int>(event.getX());
    mouseEvent.y = static_cast<int>(event.getY());
    mouseEvent.modifiers = getMouseModifiers();
    this->host->SendMouseClickEvent(mouseEvent, getMouseButtonType(event.getButton()), false, 1);
}

void OSRCefHandler::sendMouseButtonReleasedEvent(const MouseButtonReleasedEvent& event) const {
    if (this->host == nullptr) {
        return;
    }

    CefMouseEvent mouseEvent;
    mouseEvent.x = static_cast<int>(event.getX());
    mouseEvent.y = static_cast<int>(event.getY());
    mouseEvent.modifiers = getMouseModifiers();
    this->host->SendMouseClickEvent(mouseEvent, getMouseButtonType(event.getButton()), true, 1);
}

void OSRCefHandler::sendMouseScrolledEvent(const MouseScrolledEvent& event) const {
    if (this->host == nullptr) {
        return;
    }

    CefMouseEvent mouseEvent;
    mouseEvent.x = static_cast<int>(event.getX());
    mouseEvent.y = static_cast<int>(event.getY());
    this->host->SendMouseWheelEvent(mouseEvent, static_cast<int>(event.getOffsetX()), static_cast<int>(event.getOffsetY()));
}

void OSRCefHandler::sendKeyPressedEvent(const KeyPressedEvent& event) const {
    if (this->host == nullptr) {
        return;
    }

    CefKeyEvent keyEvent;
    keyEvent.windows_key_code = event.getScancode();
    keyEvent.native_key_code = event.getScancode();
    keyEvent.type = KEYEVENT_RAWKEYDOWN;
    // this->host->SendKeyEvent(keyEvent);
}

void OSRCefHandler::sendKeyReleasedEvent(const KeyReleasedEvent& event) const {
    if (this->host == nullptr) {
        return;
    }

    CefKeyEvent keyEvent;
    keyEvent.windows_key_code = event.getScancode();
    keyEvent.native_key_code = event.getScancode();
    keyEvent.type = KEYEVENT_KEYUP;
    // this->host->SendKeyEvent(keyEvent);
}

void OSRCefHandler::sendCharTypedEvent(const CharTypedEvent& event) const {
    if (this->host == nullptr) {
        return;
    }

    CefKeyEvent keyEvent;
    keyEvent.character = event.getChar();
    keyEvent.windows_key_code = event.getChar();
    keyEvent.type = KEYEVENT_CHAR;
    this->host->SendKeyEvent(keyEvent);
}

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

cef_mouse_button_type_t OSRCefHandler::getMouseButtonType(const InputCode code) {
    switch (code) { // NOLINT(clang-diagnostic-switch-enum)
        case InputCode::MOUSE_BUTTON_RIGHT:
            return MBT_RIGHT;
        case InputCode::MOUSE_BUTTON_MIDDLE:
            return MBT_MIDDLE;
        case InputCode::MOUSE_BUTTON_LEFT:
        default:
            return MBT_LEFT;
    }
}

uint32_t OSRCefHandler::getMouseModifiers() {
    uint32_t modifiers = 0;

    if (Input::isActionPressed("left_click")) {
        modifiers |= EVENTFLAG_LEFT_MOUSE_BUTTON;
    }
    if (Input::isActionPressed("middle_click")) {
        modifiers |= EVENTFLAG_MIDDLE_MOUSE_BUTTON;
    }
    if (Input::isActionPressed("right_click")) {
        modifiers |= EVENTFLAG_RIGHT_MOUSE_BUTTON;
    }
    return modifiers;
}
