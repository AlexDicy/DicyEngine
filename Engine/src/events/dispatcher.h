#pragma once

#include "pch/enginepch.h"
#include "event.h"

// ReSharper disable CppMemberFunctionMayBeStatic
class EventDispatcher {
    template <typename E = Event>
    using Handler = std::function<void(const E &)>;

public:
    EventDispatcher() = default;

    template <typename E = Event>
    void register_global_handler(Handler<E> handler) {
        add_global_handler(typeid(E), new Handler<E>(handler));
    }

    template <typename E = Event>
    void register_layer_handler(const unsigned int layer_index, Handler<E> handler) {
        add_layer_handler(layer_index, typeid(E), new Handler<E>(handler));
    }

    template <typename E = Event>
    void dispatch(const E &event) {
        handle_event(typeid(E), event);
    }

    DE_API static EventDispatcher *get();

private:
    // hide the implementation details, solves a bunch of memory issues when vectors need to be resized
    DE_API static void add_global_handler(const std::type_index &type_index, void *handler_ptr);
    DE_API static void add_layer_handler(unsigned int layer_index, const std::type_index &type_index, void *handler_ptr);
    DE_API static void handle_event(const std::type_index &type_index, const Event &event);
};
