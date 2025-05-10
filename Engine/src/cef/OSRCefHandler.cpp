#include "pch/enginepch.h"
#include "OSRCefHandler.h"

#include "include/base/cef_callback.h"
#include "include/wrapper/cef_closure_task.h"
#include "Application.h"

OSRCefHandler* instance = nullptr;

OSRCefHandler::OSRCefHandler(const Ref<Application>& app) : app(app) {
    instance = this;
}

OSRCefHandler::~OSRCefHandler() {
    instance = nullptr;
}

void OSRCefHandler::showMainWindow() {
    if (!CefCurrentlyOn(TID_UI)) {
        CefPostTask(TID_UI, base::BindOnce(&OSRCefHandler::showMainWindow, this));
        return;
    }

    // todo:
    // if (browser_list_.empty()) {
    // return;
    // }

    // auto main_browser = browser_list_.front();

    // if (auto browser_view = CefBrowserView::GetForBrowser(main_browser)) {
    // Show the window using the Views framework.
    // if (auto window = browser_view->GetWindow()) {
    // window->Show();
    // }
    // } else if (is_alloy_style_) {
    // PlatformShowWindow(main_browser);
    // }
}

void OSRCefHandler::closeAllBrowsers(bool force) {
    if (!CefCurrentlyOn(TID_UI)) {
        CefPostTask(TID_UI, base::BindOnce(&OSRCefHandler::closeAllBrowsers, this, force));
        return;
    }
    // TODO:
    //  if (browserList.empty()) {
    //  return;
    // }

    // auto it = browserList.begin();
    // for (; it != browserList.end(); ++it) {
    // (*it)->GetHost()->CloseBrowser(force);
    // }
}

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
    mouseEvent.x = getMouseCoordinate(event.getX());
    mouseEvent.y = getMouseCoordinate(event.getY());
    mouseEvent.modifiers = getMouseModifiers();
    this->host->SendMouseMoveEvent(mouseEvent, false);
}

void OSRCefHandler::sendMouseButtonPressedEvent(const MouseButtonPressedEvent& event) const {
    if (this->host == nullptr) {
        return;
    }

    CefMouseEvent mouseEvent;
    mouseEvent.x = getMouseCoordinate(event.getX());
    mouseEvent.y = getMouseCoordinate(event.getY());
    mouseEvent.modifiers = getMouseModifiers();
    this->host->SendMouseClickEvent(mouseEvent, getMouseButtonType(event.getButton()), false, 1);
}

void OSRCefHandler::sendMouseButtonReleasedEvent(const MouseButtonReleasedEvent& event) const {
    if (this->host == nullptr) {
        return;
    }

    CefMouseEvent mouseEvent;
    mouseEvent.x = getMouseCoordinate(event.getX());
    mouseEvent.y = getMouseCoordinate(event.getY());
    mouseEvent.modifiers = getMouseModifiers();
    this->host->SendMouseClickEvent(mouseEvent, getMouseButtonType(event.getButton()), true, 1);
}

void OSRCefHandler::sendMouseScrolledEvent(const MouseScrolledEvent& event) const {
    if (this->host == nullptr) {
        return;
    }

    CefMouseEvent mouseEvent;
    mouseEvent.x = getMouseCoordinate(event.getX());
    mouseEvent.y = getMouseCoordinate(event.getY());
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
    keyEvent.windows_key_code = event.getHostKey();
    keyEvent.native_key_code = event.getScancode();
    keyEvent.modifiers = getKeyModifiers(event);
    keyEvent.type = KEYEVENT_RAWKEYDOWN;
    this->host->SendKeyEvent(keyEvent);
}

void OSRCefHandler::sendKeyReleasedEvent(const KeyReleasedEvent& event) const {
    if (this->host == nullptr) {
        return;
    }

    CefKeyEvent keyEvent;
    keyEvent.windows_key_code = event.getHostKey();
    keyEvent.native_key_code = event.getScancode();
    keyEvent.modifiers = getKeyModifiers(event);
    keyEvent.type = KEYEVENT_KEYUP;
    this->host->SendKeyEvent(keyEvent);
}

void OSRCefHandler::sendCharTypedEvent(const CharTypedEvent& event) const {
    if (this->host == nullptr) {
        return;
    }

    CefKeyEvent keyEvent;
    keyEvent.character = static_cast<char16_t>(event.getChar());
    keyEvent.windows_key_code = static_cast<char16_t>(event.getChar());
    keyEvent.type = KEYEVENT_CHAR;
    this->host->SendKeyEvent(keyEvent);
}

void OSRCefHandler::updateFrameInfo(const double deltaTime) const {
    if (this->host == nullptr) {
        return;
    }
    DE_PROFILE_FUNCTION();
    const auto args = BrowserMessageHandler::createMessage("updateFrameInfo");
    args->appendDouble(deltaTime);
    this->host->GetBrowser()->GetMainFrame()->SendProcessMessage(PID_RENDERER, args->getMessage());
}

void OSRCefHandler::updateProfilingInfo() const {
    if (this->host == nullptr) {
        return;
    }
    DE_PROFILE_FUNCTION();
    const std::vector<DE::Profiling::ProfilerResult> timings = DE::Profiling::getProfiler().getTimings();
    const auto args = BrowserMessageHandler::createMessage("updateProfilingInfo");
    MessageList list;
    for (const auto& [name, duration] : timings) {
        MessageList timing;
        timing.appendString(name);
        timing.appendDouble(duration);
        list.appendList(timing);
    }
    args->appendList(list);
    this->host->GetBrowser()->GetMainFrame()->SendProcessMessage(PID_RENDERER, args->getMessage());
}

void OSRCefHandler::OnLoadStart(const CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, TransitionType transitionType) {
    this->host = browser->GetHost();
}

void OSRCefHandler::OnLoadError(CefRefPtr<CefBrowser> browser, const CefRefPtr<CefFrame> frame, const ErrorCode errorCode, const CefString& errorText, const CefString& failedUrl) {
    const std::string url = failedUrl.ToString();
    DE_ERROR("CEF Load error: {}, {}, {}", static_cast<int>(errorCode), errorText.ToString(), url);
    if (url.starts_with("data:")) {
        return;
    }
    frame->LoadURL(std::format("data:text/html,UI server error. <a href=\"javascript:window.location.href='{}'\">Reload</a>", url));
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
    rect.width = getCoordinate(app->getWindow()->getFramebufferWidth());
    rect.height = getCoordinate(app->getWindow()->getFramebufferHeight());
    if (rect.width <= 0 || rect.height <= 0) {
        rect.width = rect.height = 1;
    }
}

void OSRCefHandler::OnPaint(CefRefPtr<CefBrowser> browser, const PaintElementType type, const RectList& dirtyRects, const void* buffer, const int width, const int height) {
    this->updateTexture(buffer, width, height);
}

bool OSRCefHandler::OnProcessMessageReceived(const CefRefPtr<CefBrowser> browser, const CefRefPtr<CefFrame> frame, const CefProcessId sourceProcess,
                                             const CefRefPtr<CefProcessMessage> message) {
    return this->browserMessageHandler.processMessage(this, browser, frame, sourceProcess, message);
}

bool OSRCefHandler::DoClose(CefRefPtr<CefBrowser> browser) {
    std::cout << "DoClose\n";
    // todo: check browserList
    this->closing = true;
    return false;
}

OSRCefHandler* OSRCefHandler::getInstance() {
    return instance;
}

void OSRCefHandler::updateTexture(const void* buffer, const unsigned int width, const unsigned int height) const {
    // if (this->texture->getWidth() != width || this->texture->getHeight() != height) {
        // this->texture->resize(width, height);
    // }
    // this->texture->setRawData(buffer);
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

int OSRCefHandler::getMouseCoordinate(const float rawValue) const {
#ifdef DE_PLATFORM_MACOS
    return static_cast<int>(rawValue);
#else
    return getCoordinate(rawValue);
#endif
}

void OSRCefHandler::queueTaskForMainThread(const std::function<void()>& task) {
    std::lock_guard lock(this->taskQueueMutex);
    this->mainThreadTasks.push(task);
}

void OSRCefHandler::processMainThreadTasks() {
    std::queue<std::function<void()>> tasks;
    {
        std::lock_guard lock(this->taskQueueMutex);
        std::swap(tasks, this->mainThreadTasks);
    }

    while (!tasks.empty()) {
        const std::function<void()>& task = tasks.front();
        task();
        tasks.pop();
    }
}

uint32_t OSRCefHandler::getKeyModifiers(const KeyEvent& event) {
    uint32_t modifiers = 0;
    if (event.isShift()) {
        modifiers |= EVENTFLAG_SHIFT_DOWN;
    }
    if (event.isCtrl()) {
        modifiers |= EVENTFLAG_CONTROL_DOWN;
    }
    if (event.isAlt()) {
        modifiers |= EVENTFLAG_ALT_DOWN;
    }
    if (event.isSuper()) {
        modifiers |= EVENTFLAG_COMMAND_DOWN;
    }
    return modifiers;
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
