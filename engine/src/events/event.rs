pub trait Event {
    fn is_handled(&self) -> bool;
    fn set_handled(&mut self, handled: bool);
}

pub struct WindowResizeEvent {
    pub width: u32,
    pub height: u32,
    pub handled: bool,
}

impl Event for WindowResizeEvent {
    fn is_handled(&self) -> bool {
        self.handled
    }

    fn set_handled(&mut self, handled: bool) {
        self.handled = handled;
    }
}

pub struct WindowCloseEvent {
    pub handled: bool,
}

impl Event for WindowCloseEvent {
    fn is_handled(&self) -> bool {
        self.handled
    }

    fn set_handled(&mut self, handled: bool) {
        self.handled = handled;
    }
}

pub struct KeyPressedEvent {
    pub key: u32,
    pub repeat_count: u32,
    pub handled: bool,
}

impl Event for KeyPressedEvent {
    fn is_handled(&self) -> bool {
        self.handled
    }

    fn set_handled(&mut self, handled: bool) {
        self.handled = handled;
    }
}

pub struct KeyReleasedEvent {
    pub key: u32,
    pub handled: bool,
}

impl Event for KeyReleasedEvent {
    fn is_handled(&self) -> bool {
        self.handled
    }

    fn set_handled(&mut self, handled: bool) {
        self.handled = handled;
    }
}
