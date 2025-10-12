#include "pch/enginepch.h"
#include "RenderCommand.h"

void RenderCommandQueue::push(std::unique_ptr<RenderCommandBase> command) {
    buffers[writeBuffer].push_back(std::move(command));
}

void RenderCommandQueue::swap() {
    std::unique_lock lock(mutex);
    std::swap(writeBuffer, readBuffer);
    buffers[writeBuffer].clear();
    readCondition.notify_one();
}

bool RenderCommandQueue::execute(RenderCommands* renderer, const unsigned int timeoutMilliseconds) {
    std::unique_lock lock(mutex);
    if (!readCondition.wait_for(lock, std::chrono::milliseconds(timeoutMilliseconds), [this] {
            return !buffers[readBuffer].empty();
        })) {
        return false;
    }

    for (const auto& command : buffers[readBuffer]) {
        command->execute(renderer);
    }
    buffers[readBuffer].clear();
    return true;
}
