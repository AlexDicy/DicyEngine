use crate::events::event::Event;

pub struct EventDispatcher<T: Event> {
    handlers: Vec<fn(&T)>,
}

impl<T: Event> EventDispatcher<T> {
    pub fn new() -> EventDispatcher<T> {
        EventDispatcher {
            handlers: Vec::new(),
        }
    }

    pub fn register_handler(&mut self, handler: fn(&T)) {
        self.handlers.push(handler);
    }

    pub fn dispatch(&self, event: &T) {
        for handler in &self.handlers {
            handler(event);
        }
    }
}
