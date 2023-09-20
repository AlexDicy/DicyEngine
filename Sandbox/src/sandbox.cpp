#include "application.h"
#include "events/dispatcher.h"

int main() {
    const auto app = new Application();

    const auto event_dispatcher = EventDispatcher::get();
    event_dispatcher->register_global_handler<WindowResizeEvent>([](const WindowResizeEvent &event) {
        DE_TRACE("WindowResizeEvent: {0} {1}", event.get_width(), event.get_height());
    });
    event_dispatcher->register_global_handler<WindowCloseEvent>([](const WindowCloseEvent &) {
        DE_TRACE("WindowCloseEvent");
    });
    event_dispatcher->register_global_handler<KeyPressedEvent>([](const KeyPressedEvent &event) {
        DE_TRACE("KeyPressedEvent: {0} {1}", event.get_key(), event.get_repeat_count());
    });
    event_dispatcher->register_global_handler<KeyReleasedEvent>([](const KeyReleasedEvent &event) {
        DE_TRACE("KeyReleasedEvent: {0}", event.get_key());
    });
    event_dispatcher->register_global_handler<MouseButtonPressedEvent>([](const MouseButtonPressedEvent &event) {
        DE_TRACE("MouseButtonPressedEvent: {0}", event.get_button());
    });
    event_dispatcher->register_global_handler<MouseButtonReleasedEvent>([](const MouseButtonReleasedEvent &event) {
        DE_TRACE("MouseButtonReleasedEvent: {0}", event.get_button());
    });
    event_dispatcher->register_global_handler<MouseMovedEvent>([](const MouseMovedEvent &event) {
        DE_TRACE("MouseMovedEvent: {0} {1}", event.get_x(), event.get_y());
    });
    event_dispatcher->register_global_handler<MouseScrolledEvent>([](const MouseScrolledEvent &event) {
        DE_TRACE("MouseScrolledEvent: {0} {1}", event.get_offset_x(), event.get_offset_y());
    });

    app->run();
    delete app;
    return 0;
}
