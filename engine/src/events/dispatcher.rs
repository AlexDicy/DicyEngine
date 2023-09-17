use std::any::{Any, TypeId};
use std::collections::HashMap;
use crate::events::event::Event;

pub struct EventDispatcher {
    handlers: HashMap<TypeId, Vec<Box<dyn Any + Send + Sync>>>,
}


impl EventDispatcher {
    pub(crate) fn new() -> EventDispatcher {
        EventDispatcher {
            handlers: HashMap::new(),
        }
    }

    pub fn register_handler<T: 'static + Event>(&mut self, handler: fn(&T)) {
        self.handlers.entry(TypeId::of::<T>()).or_insert(Vec::new()).push(Box::new(handler));
    }

    pub fn dispatch<T: 'static + Event>(&self, event: &T) {
        if let Some(handlers) = self.handlers.get(&TypeId::of::<T>()) {
            for handler in handlers {
                let handler = handler.downcast_ref::<fn(&T)>().unwrap();
                handler(event);
            }
        }
    }
}

pub fn get_event_dispatcher() -> &'static mut EventDispatcher {
    static mut EVENT_DISPATCHER: *mut EventDispatcher = std::ptr::null_mut();
    unsafe {
        if EVENT_DISPATCHER.is_null() {
            EVENT_DISPATCHER = Box::into_raw(Box::new(EventDispatcher::new()));
        }
        &mut *EVENT_DISPATCHER
    }
}

