pub trait Event {
    fn is_handled(&self) -> bool;
    fn set_handled(&mut self, handled: bool);
}

macro_rules! event {
    (
        $(#[$attr:meta])*
        $name:ident {
            $(
            $(#[$field_attr:meta])*
            $field:ident: $type:ty
            ),* $(,)?
        }
    ) => {
        $(#[$attr])*
        pub struct $name {
            $(
            $(#[$field_attr])*
            pub $field: $type,
            )*
        }

        impl Event for $name {
            fn is_handled(&self) -> bool {
                self.handled
            }

            fn set_handled(&mut self, handled: bool) {
                self.handled = handled;
            }
        }
    };
}

event!(WindowResizeEvent {
    width: u32,
    height: u32,
    handled: bool,
});

event!(WindowCloseEvent {
    handled: bool,
});

event!(KeyPressedEvent {
    key: u8,
    repeat_count: u32,
    handled: bool,
});

event!(KeyReleasedEvent {
    key: u8,
    handled: bool,
});

event!(MouseButtonPressedEvent {
    button: u8,
    handled: bool,
});

event!(MouseButtonReleasedEvent {
    button: u8,
    handled: bool,
});

event!(MouseScrolledEvent {
    offset_x: f64,
    offset_y: f64,
    handled: bool,
});

event!(MouseMovedEvent {
    x: f64,
    y: f64,
    handled: bool,
});
