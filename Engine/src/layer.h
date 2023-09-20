#pragma once
#include "events/dispatcher.h"

class Layer {
    unsigned index;
    EventDispatcher *event_dispatcher;

public:
    Layer(unsigned int index, EventDispatcher *event_dispatcher) : index(index), event_dispatcher(event_dispatcher) {
    }
};
