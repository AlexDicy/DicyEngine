#pragma once
#include <internal/cef_ptr.h>

#include "cef/OSRCefHandler.h"

class UIScript final : public EntityScript {
public:
    explicit UIScript(const Ref<Application>& app, const Ref<Entity>& entity);

    void onSpawn() override;
    void onUpdate(float deltaTime) override;

private:
    Ref<Application> app;
    std::string url = "http://localhost:5173/";
    CefRefPtr<OSRCefHandler> handler;
};

struct UITexture {
    Ref<Texture2D> texture;
};
