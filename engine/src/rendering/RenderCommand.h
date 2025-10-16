#pragma once

class RenderCommands;

class RenderCommandBase {
public:
    virtual ~RenderCommandBase() = default;

    virtual void execute(RenderCommands* renderer) = 0;
};

template <typename M, typename... Args>
class RenderCommandGeneric : public RenderCommandBase {
public:
    explicit RenderCommandGeneric(M method, Args&&... args) : method(method), args(std::forward<Args>(args)...) {}

    void execute(RenderCommands* renderer) override {
        std::apply(
            [this, renderer]<typename... UArgs>(UArgs&&... unpackedArgs) {
                (renderer->*method)(std::forward<UArgs>(unpackedArgs)...);
            },
            std::move(args));
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
    RenderCommandQueue(const Ref<RenderCommands>& renderCommands, const std::thread::id& renderThreadId) : renderCommands(renderCommands), renderThreadId(renderThreadId) {}

    // Push a command to be executed in the next frame (render thread). This is non-blocking.
    void push(std::unique_ptr<RenderCommandBase> command);
    // Push a command and block until it has been executed in the render thread.
    // Once the current queue has been executed, the command will be executed.
    void pushSync(std::unique_ptr<RenderCommandBase> command);

    void swap();

    bool execute(unsigned int timeoutMilliseconds);

private:
    Ref<RenderCommands> renderCommands;
    std::thread::id renderThreadId;

    std::vector<std::unique_ptr<RenderCommandBase>> buffers[2];
    int writeBuffer = 0;
    int readBuffer = 1;
    std::vector<std::unique_ptr<RenderCommandBase>> immediateQueue;
    std::mutex mutex;
    std::condition_variable readCondition;
    std::condition_variable immediateCondition;
    bool immediateExecuted = false;
};
