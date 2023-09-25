#include "pch/enginepch.h"
#include "dispatcher.h"

EventDispatcher *EventDispatcher::get() {
    static auto instance = new EventDispatcher();
    return instance;
}

namespace {
    using HandlerMap = std::unordered_map<std::type_index, std::vector<void *>>;
    HandlerMap global_handlers;
    std::vector<HandlerMap> layer_handlers;
}

void EventDispatcher::add_global_handler(const std::type_index &type_index, void *handler_ptr) {
    global_handlers[type_index].push_back(handler_ptr);
}

void EventDispatcher::add_layer_handler(const unsigned layer_index, const std::type_index &type_index, void *handler_ptr) {
    if (layer_index >= layer_handlers.size()) {
        layer_handlers.resize(layer_index + 1);
    }
    layer_handlers[layer_index][type_index].push_back(handler_ptr);
}


void EventDispatcher::handle_event(const std::type_index &type_index, const Event &event) {
    // global handlers
    for (const auto &handler : global_handlers[type_index]) {
        const auto handler_ptr = static_cast<Handler<> *>(handler);
        (*handler_ptr)(event);
    }

    // layer handlers
    for (auto &layer_handler : layer_handlers) {
        for (const auto &handler : layer_handler[type_index]) {
            const auto handler_ptr = static_cast<Handler<> *>(handler);
            (*handler_ptr)(event);
        }
    }
}
