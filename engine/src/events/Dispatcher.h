#pragma once

#include "Event.h"

// ReSharper disable CppMemberFunctionMayBeStatic
class EventDispatcher {
    template <typename E = Event>
    using Handler = std::function<void(const E &)>;

public:
    EventDispatcher() = default;

    template <typename E = Event>
    void registerGlobalHandler(Handler<E> handler) {
        addGlobalHandler(typeid(E), new Handler<E>(handler));
    }

    template <typename E = Event>
    void registerLayerHandler(const unsigned int layerIndex, Handler<E> handler) {
        addLayerHandler(layerIndex, typeid(E), new Handler<E>(handler));
    }

    template <typename E = Event>
    void dispatch(const E &event) {
        handleEvent(typeid(E), event);
    }

    static EventDispatcher *get();

private:
    // hide the implementation details, solves a bunch of memory issues when vectors need to be resized
    static void addGlobalHandler(const std::type_index &typeIndex, void *handlerPtr);
    static void addLayerHandler(unsigned int layerIndex, const std::type_index &typeIndex, void *handlerPtr);
    static void handleEvent(const std::type_index &typeIndex, const Event &event);
};
