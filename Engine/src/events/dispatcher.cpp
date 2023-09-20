#include "pch/enginepch.h"
#include "dispatcher.h"

EventDispatcher* EventDispatcher::get() {
    static auto instance = new EventDispatcher();
    return instance;
}
