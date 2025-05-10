#pragma once
#include <internal/cef_ptr.h>

#include "cef/OSRCefHandler.h"

class UIScript final : public EntityScript {
public:
    explicit UIScript(const Ref<Application>& app, const Ref<Entity>& entity);

    void onSpawn() override;
    void onUpdate(float deltaTime) override;

private:
    static MessageDictionary createEntityDictionary(const Ref<Entity>& entity);

    bool initializeCef();
    void runCefThread();

    Ref<Application> app;
    std::string url = "http://localhost:5173/";
    CefRefPtr<OSRCefHandler> handler;

    std::thread cefThread;
    std::mutex initMutex;
    std::condition_variable initCondition;
    bool initializeCefReturned = false;
    bool cefStarted = false;
};

struct UITexture {
    Ref<Texture2D> texture;
};
