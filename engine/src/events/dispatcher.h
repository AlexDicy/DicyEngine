#pragma once

#include "pch/enginepch.h"
#include "event.h"

class EventDispatcher {
public:
    EventDispatcher() = default;

    template <typename E = Event>
    using Handler = std::function<void(const E &)>;

    std::unordered_map<std::type_index, std::vector<void *>> handlers;

    template <typename E = Event>
    void register_handler(Handler<E> handler) {
        auto handler_ptr = new Handler<E>(handler);
        handlers[typeid(E)].push_back(static_cast<void *>(handler_ptr));
    }

    template <typename E = Event>
    void dispatch(const E &event) {
        DE_INFO("Dispatching event: {0}", typeid(E).name());
        for (const auto &handler : this->handlers[typeid(E)]) {
            auto handler_ptr = static_cast<Handler<E> *>(handler);
            (*handler_ptr)(event);
        }
    }

    DE_API static EventDispatcher* get();
};
