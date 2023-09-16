use crate::logging::engine_log;

pub struct Application {}

impl Application {
    pub fn new() -> Self {
        Application {}
    }

    pub fn run(&self) {
        engine_log!("Application::run()");
        loop {}
    }
}
