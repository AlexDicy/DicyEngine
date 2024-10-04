#pragma once

namespace DE::Profiling {
    struct ProfilerResult {
        std::string name;
        long duration; // microseconds
    };

    class Profiler {
    public:
        void save_function_time(const char* name, const long duration) {
            this->timings.emplace_back(name, duration);
        }

        const std::vector<ProfilerResult>& get_timings() {
            return this->timings;
        }

        void clear_timings() {
            this->timings.clear();
        }

    private:
        std::vector<ProfilerResult> timings;
    };

    extern Profiler profiler;

    inline Profiler& get_profiler() {
        return profiler;
    }

    class ProfilerTimer {
    public:
        explicit ProfilerTimer(const char* name) : name(name) {
            this->start_time = get_current_time();
        }
        ~ProfilerTimer() {
            const long long duration = get_current_time() - start_time;
            get_profiler().save_function_time(this->name, static_cast<long>(duration));
        }

        static long long get_current_time() {
            const auto time_point = std::chrono::high_resolution_clock::now();
            return std::chrono::time_point_cast<std::chrono::microseconds>(time_point).time_since_epoch().count();
        }

    private:
        const char* name;
        long long start_time;
    };
}

#if defined(DE_DEBUG)
    #ifdef __GNUC__
        #define DE_PROFILER_FN_SIG __PRETTY_FUNCTION__
    #elifdef _MSC_VER
        #define DE_PROFILER_FN_SIG __FUNCSIG__
    #else
        #define DE_PROFILER_FN_SIG __FUNCTION__
    #endif

    // should be the first line in a function, the profiling ends when it goes out of scope
    #define DE_PROFILE_FUNCTION() DE::Profiling::ProfilerTimer _timer_function(DE_PROFILER_FN_SIG)
#else
    #define DE_PROFILE_FUNCTION()
#endif
