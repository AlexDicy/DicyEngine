#include "pch/enginepch.h"
#include "Dispatcher.h"

EventDispatcher *EventDispatcher::get() {
    static auto instance = new EventDispatcher();
    return instance;
}

namespace {
    using HandlerMap = std::unordered_map<std::type_index, std::vector<void *>>;
    HandlerMap globalHandlers;
    std::vector<HandlerMap> layerHandlers;
}

void EventDispatcher::addGlobalHandler(const std::type_index &typeIndex, void *handlerPtr) {
    globalHandlers[typeIndex].push_back(handlerPtr);
}

void EventDispatcher::addLayerHandler(const unsigned layerIndex, const std::type_index &typeIndex, void *handlerPtr) {
    if (layerIndex >= layerHandlers.size()) {
        layerHandlers.resize(layerIndex + 1);
    }
    layerHandlers[layerIndex][typeIndex].push_back(handlerPtr);
}


void EventDispatcher::handleEvent(const std::type_index &typeIndex, const Event &event) {
    // global handlers
    for (const auto &handler : globalHandlers[typeIndex]) {
        const auto handlerPtr = static_cast<Handler<> *>(handler);
        (*handlerPtr)(event);
    }

    // layer handlers
    for (auto &layerHandler : layerHandlers) {
        for (const auto &handler : layerHandler[typeIndex]) {
            const auto handlerPtr = static_cast<Handler<> *>(handler);
            (*handlerPtr)(event);
        }
    }
}
