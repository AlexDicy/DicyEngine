#pragma once

class RenderCommands;

class RenderCommandBase {
public:
    virtual ~RenderCommandBase() = default;

    virtual void execute(RenderCommands* renderer) = 0;
};

template <typename T>
class RenderCommand;

template <typename... Args>
class RenderCommand<void (RenderCommands::*)(Args...)> : public RenderCommandBase {
public:
    explicit RenderCommand(void (RenderCommands::*method)(Args...), Args... args) : method(method), args(std::make_tuple(std::move(args)...)) {}

    void execute(RenderCommands* renderer) override {
        std::apply(
            [this, renderer](Args... unpackedArgs) {
                (renderer->*method)(unpackedArgs...);
            },
            args);
        
    }

private:
    void (RenderCommands::*method)(Args...);
    std::tuple<std::decay_t<Args>...> args;
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
