#[derive(PartialEq, Eq, Hash, Clone, Copy, Debug)]
pub enum EventType {
    WindowResize
}

pub enum EventCategory {
    Application,
    Input,
    Keyboard,
    Mouse,
    MouseButton,
}

impl EventCategory {
    pub fn is_input(&self) -> bool {
        match self {
            EventCategory::Input => true,
            EventCategory::Keyboard => true,
            EventCategory::Mouse => true,
            EventCategory::MouseButton => true,
            _ => false,
        }
    }

    pub fn is_mouse(&self) -> bool {
        match self {
            EventCategory::Mouse => true,
            EventCategory::MouseButton => true,
            _ => false,
        }
    }
}

pub trait Event {
    fn get_type(&self) -> EventType;
    fn get_category(&self) -> EventCategory;
    fn is_handled(&self) -> bool;
    fn set_handled(&mut self, handled: bool);
}

pub struct WindowResizeEvent {
    pub width: u32,
    pub height: u32,
    pub handled: bool,
}

impl Event for WindowResizeEvent {
    fn get_type(&self) -> EventType {
        EventType::WindowResize
    }

    fn get_category(&self) -> EventCategory {
        EventCategory::Application
    }

    fn is_handled(&self) -> bool {
        self.handled
    }

    fn set_handled(&mut self, handled: bool) {
        self.handled = handled;
    }
}
