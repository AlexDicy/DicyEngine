#pragma once
#pragma warning(push, 0)
#include "spdlog/spdlog.h"
#pragma warning(pop)


class Logger {
public:
    static void init();
};

class LoggerWrapper {
public:
    LoggerWrapper();
    std::shared_ptr<spdlog::logger> engineLogger;
    std::shared_ptr<spdlog::logger> applicationLogger;
};

namespace DE::Logger {
    extern LoggerWrapper* loggerWrapper;

    inline std::shared_ptr<spdlog::logger>& getEngineLogger() {
        return loggerWrapper->engineLogger;
    }
    inline std::shared_ptr<spdlog::logger>& getApplicationLogger() {
        return loggerWrapper->applicationLogger;
    }
}

#if defined(DE_DEBUG) || defined(DE_RELEASE)
    #ifdef DE_IS_ENGINE
        #define DE_TRACE(...) DE::Logger::getEngineLogger()->trace(__VA_ARGS__)
        #define DE_INFO(...) DE::Logger::getEngineLogger()->info(__VA_ARGS__)
        #define DE_WARN(...) DE::Logger::getEngineLogger()->warn(__VA_ARGS__)
    #else
        #define DE_TRACE(...) DE::Logger::getApplicationLogger()->trace(__VA_ARGS__)
        #define DE_INFO(...) DE::Logger::getApplicationLogger()->info(__VA_ARGS__)
        #define DE_WARN(...) DE::Logger::getApplicationLogger()->warn(__VA_ARGS__)
    #endif

    #define DE_ASSERT(x, ...) { if (!(x)) { DE_ERROR(__VA_ARGS__); } }
#else
    #define DE_TRACE(...)
    #define DE_INFO(...)
    #define DE_WARN(...)
#endif

#ifdef DE_IS_ENGINE
    #define DE_ERROR(...) DE::Logger::getEngineLogger()->error(__VA_ARGS__)
#else
    #define DE_ERROR(...) DE::Logger::getApplicationLogger()->error(__VA_ARGS__)
#endif
