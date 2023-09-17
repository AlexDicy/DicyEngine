use engine::application::Application;
use engine::events::dispatcher::get_event_dispatcher;
use engine::events::event::{KeyPressedEvent, WindowResizeEvent};

fn main() {
    let mut app = Application::new();
    let event_dispatcher = get_event_dispatcher();
    event_dispatcher.register_handler(|event: &WindowResizeEvent| {
        println!("HANDLED: {} {}", event.width, event.height);
    });
    event_dispatcher.register_handler(|event: &KeyPressedEvent| {
        println!("Key: {} {}", event.key, event.repeat_count);
    });
    app.run();
}
