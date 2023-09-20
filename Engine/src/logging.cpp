#include "pch/enginepch.h"
#include "common.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

namespace logger {
    class Logger {
    public:
        Logger() {
            this->engine_logger = spdlog::stdout_color_mt("engine");
            this->engine_logger->set_level(spdlog::level::trace);
            this->application_logger = spdlog::stdout_color_mt("app");
            this->application_logger->set_level(spdlog::level::trace);
            spdlog::set_pattern("%^[%T] %n: %v%$");
        }
        std::shared_ptr<spdlog::logger> engine_logger;
        std::shared_ptr<spdlog::logger> application_logger;
    };

    Logger* logger = nullptr;

    void init() {
        if (logger == nullptr) {
            logger = new Logger();
        }
    }

    DE_API inline std::shared_ptr<spdlog::logger> get_engine_logger() {
        return logger->engine_logger;
    }

    DE_API inline std::shared_ptr<spdlog::logger> get_application_logger() {
        return logger->application_logger;
    }
}