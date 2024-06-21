#include "pch/enginepch.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace DE::Logger {
    class LoggerWrapper {
    public:
        LoggerWrapper() {
            this->engine_logger = spdlog::stdout_color_mt("engine");
            this->engine_logger->set_level(spdlog::level::trace);
            this->application_logger = spdlog::stdout_color_mt("app");
            this->application_logger->set_level(spdlog::level::trace);
            spdlog::set_pattern("%^[%T] %n: %v%$");
        }
        std::shared_ptr<spdlog::logger> engine_logger;
        std::shared_ptr<spdlog::logger> application_logger;
    };

    LoggerWrapper *logger_wrapper = nullptr;

    DE_API inline std::shared_ptr<spdlog::logger> &get_engine_logger() {
        return logger_wrapper->engine_logger;
    }

    DE_API inline std::shared_ptr<spdlog::logger> &get_application_logger() {
        return logger_wrapper->application_logger;
    }
}

void Logger::init() {
    if (DE::Logger::logger_wrapper == nullptr) {
        DE::Logger::logger_wrapper = new DE::Logger::LoggerWrapper();
    }
}
