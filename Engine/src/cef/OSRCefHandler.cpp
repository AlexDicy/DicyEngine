#include "pch/enginepch.h"
#include "OSRCefHandler.h"

#include "Application.h"

void OSRCefHandler::sendWindowResizeEvent(const WindowResizeEvent&) const {
    if (this->host == nullptr) {
        return;
    }

    this->host->WasResized();
}

void OSRCefHandler::sendMouseMoveEvent(const MouseMovedEvent& event) const {
    if (this->host == nullptr) {
        return;
    }

    CefMouseEvent mouseEvent;
    mouseEvent.x = getCoordinate(event.getX());
    mouseEvent.y = getCoordinate(event.getY());
    mouseEvent.modifiers = getMouseModifiers();
    this->host->SendMouseMoveEvent(mouseEvent, false);
}

void OSRCefHandler::sendMouseButtonPressedEvent(const MouseButtonPressedEvent& event) const {
    if (this->host == nullptr) {
        return;
    }

    CefMouseEvent mouseEvent;
    mouseEvent.x = getCoordinate(event.getX());
    mouseEvent.y = getCoordinate(event.getY());
    mouseEvent.modifiers = getMouseModifiers();
    this->host->SendMouseClickEvent(mouseEvent, getMouseButtonType(event.getButton()), false, 1);
}

void OSRCefHandler::sendMouseButtonReleasedEvent(const MouseButtonReleasedEvent& event) const {
    if (this->host == nullptr) {
        return;
    }

    CefMouseEvent mouseEvent;
    mouseEvent.x = getCoordinate(event.getX());
    mouseEvent.y = getCoordinate(event.getY());
    mouseEvent.modifiers = getMouseModifiers();
    this->host->SendMouseClickEvent(mouseEvent, getMouseButtonType(event.getButton()), true, 1);
}

void OSRCefHandler::sendMouseScrolledEvent(const MouseScrolledEvent& event) const {
    if (this->host == nullptr) {
        return;
    }

    CefMouseEvent mouseEvent;
    mouseEvent.x = getCoordinate(event.getX());
    mouseEvent.y = getCoordinate(event.getY());
    constexpr int defaultScrollDelta = 120;
    const int deltaX = static_cast<int>(event.getOffsetX() * defaultScrollDelta);
    const int deltaY = static_cast<int>(event.getOffsetY() * defaultScrollDelta);
    this->host->SendMouseWheelEvent(mouseEvent, deltaX, deltaY);
}

void OSRCefHandler::sendKeyPressedEvent(const KeyPressedEvent& event) const {
    if (this->host == nullptr) {
        return;
    }

    CefKeyEvent keyEvent;
    keyEvent.windows_key_code = event.getScancode();
    keyEvent.native_key_code = event.getScancode();
    keyEvent.type = KEYEVENT_RAWKEYDOWN;
    this->host->SendKeyEvent(keyEvent);
}

void OSRCefHandler::sendKeyReleasedEvent(const KeyReleasedEvent& event) const {
    if (this->host == nullptr) {
        return;
    }

    CefKeyEvent keyEvent;
    keyEvent.windows_key_code = event.getScancode();
    keyEvent.native_key_code = event.getScancode();
    keyEvent.type = KEYEVENT_KEYUP;
    this->host->SendKeyEvent(keyEvent);
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

void OSRCefHandler::updateFrameInfo(const double deltaTime) const {
    if (this->host == nullptr) {
        return;
    }

    const auto message = CefProcessMessage::Create(CefString("updateFrameInfo"));
    const auto args = message->GetArgumentList();
    args->SetDouble(0, deltaTime);

    this->host->GetBrowser()->GetMainFrame()->SendProcessMessage(PID_RENDERER, message);
}

void OSRCefHandler::OnLoadStart(const CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, TransitionType transitionType) {
    this->host = browser->GetHost();
}

void OSRCefHandler::OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const ErrorCode errorCode, const CefString& errorText, const CefString& failedUrl) {
    DE_ERROR("CEF Load error: {}, {}, {}", static_cast<int>(errorCode), errorText.ToString(), failedUrl.ToString());
    frame->LoadURL("data:text/html,UI server not loaded");
}

bool OSRCefHandler::GetScreenInfo(const CefRefPtr<CefBrowser> browser, CefScreenInfo& screenInfo) {
    CefRect viewRect;
    GetViewRect(browser, viewRect);

    screenInfo.device_scale_factor = app->getWindow()->getScalingFactor();
    screenInfo.rect = viewRect;
    screenInfo.available_rect = viewRect;
    const int refreshRate = app->getWindow()->getMonitorRefreshRate();
    if (this->host && refreshRate) {
        this->host->SetWindowlessFrameRate(refreshRate);
    }
    return true;
}

void OSRCefHandler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) {
    rect.x = 0;
    rect.y = 0;
    rect.width = getCoordinate(app->getWindow()->getWidth());
    rect.height = getCoordinate(app->getWindow()->getHeight());
    if (rect.width <= 0 || rect.height <= 0) {
        rect.width = rect.height = 1;
    }
}

void OSRCefHandler::OnPaint(CefRefPtr<CefBrowser> browser, const PaintElementType type, const RectList& dirtyRects, const void* buffer, const int width, const int height) {
    this->updateTexture(buffer, width, height);
}

void OSRCefHandler::updateTexture(const void* buffer, const unsigned int width, const unsigned int height) const {
    if (this->texture->getWidth() != width || this->texture->getHeight() != height) {
        this->texture->resize(width, height);
    }
    this->texture->setRawData(buffer);
}

int OSRCefHandler::getCoordinate(const unsigned int rawValue) const {
    const float scaleFactor = app->getWindow()->getScalingFactor();
    const float scaledValue = std::floor(static_cast<float>(rawValue) / scaleFactor);
    return static_cast<int>(scaledValue);
}

int OSRCefHandler::getCoordinate(const int rawValue) const {
    return getCoordinate(static_cast<unsigned int>(rawValue));
}

int OSRCefHandler::getCoordinate(const float rawValue) const {
    return getCoordinate(static_cast<unsigned int>(rawValue));
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
