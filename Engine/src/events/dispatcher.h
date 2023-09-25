#pragma once

#include "pch/enginepch.h"
#include "event.h"

class EventDispatcher {
    template <typename E = Event>
    using Handler = std::function<void(const E &)>;
    using HandlerMap = std::unordered_map<std::type_index, std::vector<void *>>;

    HandlerMap global_handlers;
    std::vector<HandlerMap> layer_handlers;

public:
    EventDispatcher() = default;

    template <typename E = Event>
    void register_global_handler(Handler<E> handler) {
        auto handler_ptr = new Handler<E>(handler);
        global_handlers[typeid(E)].push_back(handler_ptr);
    }

    template <typename E = Event>
    void register_layer_handler(const unsigned int layer_index, Handler<E> handler) {
        if (layer_index >= this->layer_handlers.size()) {
            this->layer_handlers.resize(layer_index + 1);
        }
        auto handler_ptr = new Handler<E>(handler);
        layer_handlers[layer_index][typeid(E)].push_back(handler_ptr);
    }

    template <typename E = Event>
    void dispatch(const E &event) {
        // global handlers
        for (const auto &handler : this->global_handlers[typeid(E)]) {
            auto handler_ptr = static_cast<Handler<E> *>(handler);
            (*handler_ptr)(event);
        }

        // layer handlers
        for (auto it = this->layer_handlers.begin(); it != this->layer_handlers.end(); ++it) {
            for (const auto &handler : (*it)[typeid(E)]) {
                auto handler_ptr = static_cast<Handler<E> *>(handler);
                (*handler_ptr)(event);
            }
        }
    }

    DE_API static EventDispatcher *get();
};
