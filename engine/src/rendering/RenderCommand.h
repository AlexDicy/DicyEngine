#pragma once

class RenderCommands;

class RenderCommandQueue {
public:
    RenderCommandQueue(const Ref<RenderCommands>& renderCommands, const std::thread::id& renderThreadId) : renderCommands(renderCommands), renderThreadId(renderThreadId) {}

    // Push a command to be executed in the next frame (render thread). This is non-blocking.
    void push(std::function<void(RenderCommands*)> command);
    // Push a command and block until it has been executed in the render thread.
    // Once the current queue has been executed, the command will be executed.
    void pushSync(std::function<void(RenderCommands*)> command);

    void swap();

    bool execute(unsigned int timeoutMilliseconds);

private:
    Ref<RenderCommands> renderCommands;
    std::thread::id renderThreadId;

    std::vector<std::function<void(RenderCommands*)>> buffers[2];
    int writeBuffer = 0;
    int readBuffer = 1;
    std::vector<std::function<void(RenderCommands*)>> immediateQueue;
    std::mutex mutex;
    std::condition_variable readCondition;
    std::condition_variable immediateCondition;
    bool immediateExecuted = false;
};
