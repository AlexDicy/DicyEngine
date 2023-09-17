use crate::logging::engine_log;
use crate::window::{create_window, Window};

pub struct Application {
    running: bool
}

impl Application {
    pub fn new() -> Self {
        Self { running: true }
    }

    pub fn run(&mut self) {
        engine_log!("Application::run()");
        let mut window = create_window("Sandbox", 1280, 720);
        while self.running {
            window.update();
        }
    }
}
