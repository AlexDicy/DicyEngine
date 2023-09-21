#pragma once
#include "events/dispatcher.h"

class Layer {
protected:
    unsigned index;
    EventDispatcher *event_dispatcher;

public:
    Layer(unsigned int index, EventDispatcher *event_dispatcher) : index(index), event_dispatcher(event_dispatcher) {
    }
    virtual ~Layer() = default;

    virtual void update() {
    }
};
