#pragma once

class JavaBridge {
public:
    virtual ~JavaBridge() = default;

    static Ref<JavaBridge> get_instance();

    virtual void increment_counter() const = 0;
    virtual int get_counter() const = 0;

private:
    class JavaBridgeImpl;
    Ref<JavaBridgeImpl> implementation;
};
