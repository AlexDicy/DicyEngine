#include "pch/enginepch.h"
#include "logging.h"
#include "spdlog/sinks/stdout_color_sinks.h"

LoggerWrapper* DE::Logger::loggerWrapper = nullptr;

void Logger::init() {
    if (DE::Logger::loggerWrapper == nullptr) {
        DE::Logger::loggerWrapper = new LoggerWrapper();
    }
}

LoggerWrapper::LoggerWrapper() {
    this->engineLogger = spdlog::stdout_color_mt("engine");
    this->engineLogger->set_level(spdlog::level::trace);
    this->applicationLogger = spdlog::stdout_color_mt("app");
    this->applicationLogger->set_level(spdlog::level::trace);
    spdlog::set_pattern("%^[%T] %n: %v%$");
}
