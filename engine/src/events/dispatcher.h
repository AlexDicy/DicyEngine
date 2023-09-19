#pragma once

#include "pch/enginepch.h"
#include "event.h"

class EventDispatcher {
public:
    template <typename E = Event>
    using Handler = std::function<void(const E &)>;

private:
    std::unordered_map<std::type_index, std::vector<void *>> handlers;

public:
    EventDispatcher() = default;

    template <typename E = Event>
    void register_handler(Handler<E> handler) {
        auto handler_ptr = new Handler<E>(handler);
        handlers[typeid(E)].push_back(static_cast<void*>(handler_ptr));
    }

    template <typename E = Event>
    void dispatch(const E &event) {
        for (const auto &handler : this->handlers[typeid(E)]) {
            auto handler_ptr = static_cast<Handler<E>*>(handler);
            (*handler_ptr)(event);
        }
    }
};
