#pragma once
#pragma warning(push, 0)
#include "spdlog/spdlog.h"
#pragma warning(pop)


class Logger {
public:
    static void init();
};

namespace DE::Logger {
    DE_API inline std::shared_ptr<spdlog::logger> &get_engine_logger();
    DE_API inline std::shared_ptr<spdlog::logger> &get_application_logger();
}

#if defined(DE_DEBUG) || defined(DE_RELEASE)
    #ifdef DE_IS_ENGINE
        #define DE_TRACE(...) DE::Logger::get_engine_logger()->trace(__VA_ARGS__)
        #define DE_INFO(...) DE::Logger::get_engine_logger()->info(__VA_ARGS__)
        #define DE_WARN(...) DE::Logger::get_engine_logger()->warn(__VA_ARGS__)
    #else
        #define DE_TRACE(...) DE::Logger::get_application_logger()->trace(__VA_ARGS__)
        #define DE_INFO(...) DE::Logger::get_application_logger()->info(__VA_ARGS__)
        #define DE_WARN(...) DE::Logger::get_application_logger()->warn(__VA_ARGS__)
    #endif
#else
    #define DE_TRACE(...)
    #define DE_INFO(...)
    #define DE_WARN(...)
#endif

#ifdef DE_IS_ENGINE
    #define DE_ERROR(...) DE::Logger::get_engine_logger()->error(__VA_ARGS__)
#else
    #define DE_ERROR(...) DE::Logger::get_application_logger()->error(__VA_ARGS__)
#endif
