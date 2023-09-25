#pragma once
#include "spdlog/spdlog.h"


class Logger {
public:
    static void init();
};

namespace logger {
    DE_API inline std::shared_ptr<spdlog::logger> &get_engine_logger();
    DE_API inline std::shared_ptr<spdlog::logger> &get_application_logger();
}

#if defined(DE_DEBUG) || defined(DE_RELEASE)
    #ifdef DE_IS_ENGINE
        #define DE_TRACE(...) logger::get_engine_logger()->trace(__VA_ARGS__)
        #define DE_INFO(...) logger::get_engine_logger()->info(__VA_ARGS__)
        #define DE_WARN(...) logger::get_engine_logger()->warn(__VA_ARGS__)
    #else
        #define DE_TRACE(...) logger::get_application_logger()->trace(__VA_ARGS__)
        #define DE_INFO(...) logger::get_application_logger()->info(__VA_ARGS__)
        #define DE_WARN(...) logger::get_application_logger()->warn(__VA_ARGS__)
    #endif
#else
    #define DE_TRACE(...)
    #define DE_INFO(...)
    #define DE_WARN(...)
#endif

#ifdef DE_IS_ENGINE
    #define DE_ERROR(...) logger::get_engine_logger()->error(__VA_ARGS__)
#else
    #define DE_ERROR(...) logger::get_application_logger()->error(__VA_ARGS__)
#endif
