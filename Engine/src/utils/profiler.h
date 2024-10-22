#pragma once

namespace DE::Profiling {
    struct ProfilerResult {
        std::string name;
        long duration; // microseconds

        ProfilerResult(std::string  name, const long duration) : name(std::move(name)), duration(duration) {}
        ProfilerResult(const char* name, const long duration) : name(name), duration(duration) {}
    };

    class Profiler {
    public:
        void saveFunctionTime(const char* name, const long duration) {
            this->timings.emplace_back(name, duration);
        }

        const std::vector<ProfilerResult>& getTimings() {
            return this->timings;
        }

        void clearTimings() {
            this->timings.clear();
        }

    private:
        std::vector<ProfilerResult> timings;
    };

    extern Profiler profiler;

    inline Profiler& getProfiler() {
        return profiler;
    }

    class ProfilerTimer {
    public:
        explicit ProfilerTimer(const char* name) : name(name) {
            this->startTime = getCurrentTime();
        }
        ~ProfilerTimer() {
            const long long duration = getCurrentTime() - startTime;
            getProfiler().saveFunctionTime(this->name, static_cast<long>(duration));
        }

        static long long getCurrentTime() {
            const auto timePoint = std::chrono::high_resolution_clock::now();
            return std::chrono::time_point_cast<std::chrono::microseconds>(timePoint).time_since_epoch().count();
        }

    private:
        const char* name;
        long long startTime;
    };
}

#if defined(DE_DEBUG)
    #if defined(__GNUC__)
        #define DE_PROFILER_FN_SIG __PRETTY_FUNCTION__
    #elif defined(_MSC_VER)
        #define DE_PROFILER_FN_SIG __FUNCSIG__
    #else
        #define DE_PROFILER_FN_SIG __FUNCTION__
    #endif

    // should be the first line in a function, the profiling ends when it goes out of scope
    #define DE_PROFILE_FUNCTION() DE::Profiling::ProfilerTimer _timer_function(DE_PROFILER_FN_SIG)
#else
    #define DE_PROFILE_FUNCTION()
#endif
