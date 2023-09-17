use std::collections::HashMap;
use crate::events::event::EventType;
use crate::events::event::Event;

pub struct EventDispatcher {
    handlers: HashMap<EventType, Vec<fn(&dyn Event)>>,
}

impl EventDispatcher {
    pub fn new() -> EventDispatcher {
        EventDispatcher {
            handlers: HashMap::new(),
        }
    }

    pub fn register_handler(&mut self, event_type: EventType, handler: fn(&dyn Event)) {
        self.handlers.entry(event_type).or_insert(Vec::new()).push(handler);
    }

    pub fn dispatch(&self, event: &dyn Event) {
        if let Some(handlers) = self.handlers.get(&event.get_type()) {
            for handler in handlers {
                handler(event);
            }
        }
    }
}
