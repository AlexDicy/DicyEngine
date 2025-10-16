#include "pch/enginepch.h"
#include "RenderCommand.h"

void RenderCommandQueue::push(std::unique_ptr<RenderCommandBase> command) {
    buffers[writeBuffer].push_back(std::move(command));
}

void RenderCommandQueue::pushSync(std::unique_ptr<RenderCommandBase> command) {
    if (std::this_thread::get_id() == renderThreadId) {
        command->execute(renderCommands.get());
        return;
    }
    std::unique_lock lock(mutex);
    immediateQueue.push_back(std::move(command));
    immediateExecuted = false;
    readCondition.notify_one();

    immediateCondition.wait(lock, [this] {
        return immediateExecuted;
    });
}

void RenderCommandQueue::swap() {
    std::unique_lock lock(mutex);
    std::swap(writeBuffer, readBuffer);
    buffers[writeBuffer].clear();
    readCondition.notify_one();
}

bool RenderCommandQueue::execute(const unsigned int timeoutMilliseconds) {
    std::unique_lock lock(mutex);
    if (!readCondition.wait_for(lock, std::chrono::milliseconds(timeoutMilliseconds), [this] {
            return !immediateQueue.empty() || !buffers[readBuffer].empty();
        })) {
        return false;
    }

    if (!immediateQueue.empty()) {
        for (const auto& command : immediateQueue) {
            command->execute(renderCommands.get());
        }
        immediateQueue.clear();
        immediateExecuted = true;
        immediateCondition.notify_all();
    }

    for (const auto& command : buffers[readBuffer]) {
        command->execute(renderCommands.get());
    }
    buffers[readBuffer].clear();
    return true;
}
