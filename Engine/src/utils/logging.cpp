#include "pch/enginepch.h"
#include "logging.h"
#include "spdlog/sinks/stdout_color_sinks.h"

LoggerWrapper* DE::Logger::logger_wrapper = nullptr;

void Logger::init() {
    if (DE::Logger::logger_wrapper == nullptr) {
        DE::Logger::logger_wrapper = new LoggerWrapper();
    }
}

LoggerWrapper::LoggerWrapper() {
    this->engine_logger = spdlog::stdout_color_mt("engine");
    this->engine_logger->set_level(spdlog::level::trace);
    this->application_logger = spdlog::stdout_color_mt("app");
    this->application_logger->set_level(spdlog::level::trace);
    spdlog::set_pattern("%^[%T] %n: %v%$");
}
