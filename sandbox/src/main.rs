use engine::events::dispatcher::EventDispatcher;
use engine::events::event::{WindowCloseEvent, WindowResizeEvent};
use engine::log;

fn main() {
    let app = engine::application::Application::new();
    //app.run();
    let mut event_dispatcher1 = EventDispatcher::new();
    event_dispatcher1.register_handler(|event: &WindowResizeEvent| {
        log!("WindowResizeEvent::register_handler() called, width: {}, height: {}", event.width, event.height);
    });
    let mut event_dispatcher2 = EventDispatcher::new();
    event_dispatcher2.register_handler(|_event: &WindowCloseEvent| {
        log!("WindowCloseEvent::register_handler() called");
    });
    event_dispatcher1.dispatch(&WindowResizeEvent {
        width: 0,
        height: 0,
        handled: false,
    });
    event_dispatcher1.dispatch(&WindowResizeEvent {
        width: 9999,
        height: 9999,
        handled: false,
    });
    event_dispatcher2.dispatch(&WindowCloseEvent {
        handled: false,
    });
}
