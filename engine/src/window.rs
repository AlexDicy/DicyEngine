use glfw::{Action, Context};
use crate::events::dispatcher::get_event_dispatcher;
use crate::events::event::{KeyPressedEvent, KeyReleasedEvent, WindowCloseEvent, WindowResizeEvent};
use crate::logging::engine_error;

pub(crate) trait Window {
    fn destroy(&mut self);
    fn update(&mut self);
    fn get_width(&self) -> u32;
    fn get_height(&self) -> u32;
    fn is_vsync(&self) -> bool;
    fn set_vsync(&mut self);
}

#[cfg(target_os = "windows")]
pub(crate) fn create_window(title: &str, width: u32, height: u32) -> impl Window {
    WindowsWindow::new(title, width, height)
}

struct WindowsWindow {
    glfw: glfw::Glfw,
    window: glfw::Window,
    vsync: bool,
}

impl Window for WindowsWindow {
    fn destroy(&mut self) {
        self.window.set_should_close(true);
    }

    fn update(&mut self) {
        self.glfw.poll_events();
        self.window.swap_buffers();
    }

    fn get_width(&self) -> u32 {
        self.window.get_size().0 as u32
    }

    fn get_height(&self) -> u32 {
        self.window.get_size().1 as u32
    }

    fn is_vsync(&self) -> bool {
        self.vsync
    }

    fn set_vsync(&mut self) {
        self.vsync = !self.vsync;
        self.glfw.set_swap_interval(if self.vsync { glfw::SwapInterval::Sync(1) } else { glfw::SwapInterval::None });
    }
}

impl WindowsWindow {
    fn new(title: &str, width: u32, height: u32) -> Self {
        let mut glfw = glfw::init(Self::error_callback).unwrap();
        let (mut window, _events) = glfw
            .create_window(width, height, title, glfw::WindowMode::Windowed)
            .expect("Failed to create GLFW window");

        window.make_current();
        gl::load_with(|symbol| window.get_proc_address(symbol) as *const _);

        let event_dispatcher = get_event_dispatcher();
        window.set_size_callback(|width, height| {
            event_dispatcher.dispatch(&WindowResizeEvent { width: width as u32, height: height as u32, handled: false });
        });
        window.set_close_callback(|| {
            event_dispatcher.dispatch(&WindowCloseEvent { handled: false });
        });
        window.set_key_callback(|key, _scancode, action, _modifiers| {
            match action {
                Action::Press => event_dispatcher.dispatch(&KeyPressedEvent { key: key as u32, repeat_count: 0, handled: false }),
                Action::Release => event_dispatcher.dispatch(&KeyReleasedEvent { key: key as u32, handled: false }),
                Action::Repeat => event_dispatcher.dispatch(&KeyPressedEvent { key: key as u32, repeat_count: 1, handled: false }),
            }
        });

        Self { glfw, window, vsync: false }
    }

    fn error_callback(err: glfw::Error, description: String) {
        engine_error!("GLFW error {:?}: {:?}", err, description);
    }
}
