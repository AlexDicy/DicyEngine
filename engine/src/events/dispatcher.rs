use std::any::{Any, TypeId};
use std::collections::HashMap;
use std::sync::{Mutex, MutexGuard};
use lazy_static::lazy_static;
use crate::events::event::Event;

pub struct EventDispatcher {
    handlers: HashMap<TypeId, Vec<Box<dyn Any + Send + Sync>>>,
}


impl EventDispatcher {
    fn new() -> EventDispatcher {
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

lazy_static! {
    pub static ref DISPATCHER: Mutex<EventDispatcher> = Mutex::new(EventDispatcher::new());
}

pub fn get_event_dispatcher() -> MutexGuard<'static, EventDispatcher> {
    DISPATCHER.lock().unwrap()
}
