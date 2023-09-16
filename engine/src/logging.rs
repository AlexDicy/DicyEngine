use time::OffsetDateTime;

pub fn get_time_string() -> String {
    let now = OffsetDateTime::now_local().unwrap();
    format!("{:02}:{:02}:{:02}", now.hour(), now.minute(), now.second())
}

#[macro_export]
macro_rules! log {
    ($($e:expr),+) => {
        {
            #[cfg(debug_assertions)]
            {
                println!("{} {}", $crate::logging::get_time_string(), format!($($e),+))
            }
        }
    };
}

#[macro_export]
macro_rules! error {
    ($($e:expr),+) => {
        {
            eprintln!("{} {}", $crate::logging::get_time_string(), format!($($e),+))
        }
    };
}

#[macro_export]
macro_rules! debug {
    ($($e:expr),+) => {
        {
            #[cfg(debug_assertions)]
            {
                dbg!($($e),+)
            }
        }
    };
}

macro_rules! engine_log {
    ($($e:expr),+) => {
        {
            #[cfg(debug_assertions)]
            {
                println!("[engine] {} {}", $crate::logging::get_time_string(), format!($($e),+))
            }
        }
    };
}

macro_rules! engine_error {
    ($($e:expr),+) => {
        {
            eprintln!("[engine] {} {}", $crate::logging::get_time_string(), format!($($e),+))
        }
    };
}

macro_rules! engine_debug {
    ($($e:expr),+) => {
        {
            #[cfg(debug_assertions)]
            {
                dbg!($($e),+)
            }
        }
    };
}

pub(crate) use engine_log;
pub(crate) use engine_error;
pub(crate) use engine_debug;
