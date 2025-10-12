#pragma once

class RenderCommands;

class RenderCommandBase {
public:
    virtual ~RenderCommandBase() = default;

    virtual void execute(RenderCommands* renderer) const = 0;
};

template <typename M, typename... Args>
class RenderCommandGeneric : public RenderCommandBase {
public:
    explicit RenderCommandGeneric(M method, Args... args) : method(method), args(std::make_tuple(std::move(args)...)) {}

    void execute(RenderCommands* renderer) const override {
        std::apply(
            [this, renderer](Args... unpackedArgs) {
                (renderer->*method)(unpackedArgs...);
            },
            args);
    }

private:
    M method;
    std::tuple<std::decay_t<Args>...> args;
};

template <typename T>
class RenderCommand;

// non-const RenderCommands method
template <typename... Args>
class RenderCommand<void (RenderCommands::*)(Args...)> : public RenderCommandGeneric<void (RenderCommands::*)(Args...), Args...> {
public:
    using RenderCommandGeneric<void (RenderCommands::*)(Args...), Args...>::RenderCommandGeneric;
};

// const RenderCommands method
template <typename... Args>
class RenderCommand<void (RenderCommands::*)(Args...) const> : public RenderCommandGeneric<void (RenderCommands::*)(Args...) const, Args...> {
public:
    using RenderCommandGeneric<void (RenderCommands::*)(Args...) const, Args...>::RenderCommandGeneric;
};


class RenderCommandQueue {
public:
    void push(std::unique_ptr<RenderCommandBase> command);

    void swap();

    bool execute(RenderCommands* renderer, unsigned int timeoutMilliseconds);

private:
    std::vector<std::unique_ptr<RenderCommandBase>> buffers[2];
    int writeBuffer = 0;
    int readBuffer = 1;
    std::mutex mutex;
    std::condition_variable readCondition;
};
