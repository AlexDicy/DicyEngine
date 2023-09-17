use engine::application::Application;
use engine::events::dispatcher::get_event_dispatcher;
use engine::events::event::{KeyPressedEvent, KeyReleasedEvent, MouseButtonPressedEvent, MouseButtonReleasedEvent, MouseMovedEvent, MouseScrolledEvent, WindowCloseEvent, WindowResizeEvent};
use engine::log;

fn main() {
    let mut app = Application::new();
    let event_dispatcher = get_event_dispatcher();
    event_dispatcher.register_handler(|event: &WindowResizeEvent| { log!("WindowResizeEvent: {} {}", event.width, event.height); });
    event_dispatcher.register_handler(|event: &WindowCloseEvent| { log!("WindowCloseEvent"); });
    event_dispatcher.register_handler(|event: &KeyPressedEvent| { log!("KeyPressedEvent: {} {}", event.key, event.repeat_count); });
    event_dispatcher.register_handler(|event: &KeyReleasedEvent| { log!("KeyReleasedEvent: {}", event.key); });
    event_dispatcher.register_handler(|event: &MouseButtonPressedEvent| { log!("MouseButtonPressedEvent: {}", event.button); });
    event_dispatcher.register_handler(|event: &MouseButtonReleasedEvent| { log!("MouseButtonReleasedEvent: {}", event.button); });
    event_dispatcher.register_handler(|event: &MouseMovedEvent| { log!("MouseMovedEvent: {} {}", event.x, event.y); });
    event_dispatcher.register_handler(|event: &MouseScrolledEvent| { log!("MouseScrolledEvent: {} {}", event.offset_x, event.offset_y); });

    app.run();
}
