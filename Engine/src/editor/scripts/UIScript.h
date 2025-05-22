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

    bool initializeCef() const;
    void runCef() const;

    Ref<Application> app;
    std::string url = "http://localhost:5173/";
    CefRefPtr<OSRCefHandler> handler;
#ifdef DE_PLATFORM_WINDOWS
    std::thread cefThread;
#endif
};

struct UITexture {
    Ref<Texture2D> texture;
};
