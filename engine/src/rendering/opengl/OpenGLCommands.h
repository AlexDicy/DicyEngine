#pragma once
#include "rendering/RenderCommands.h"

class OpenGLCommands : public RenderCommands {
public:
    void initializeTexture(const Ref<Texture>& texture) override;
    void createTextureStorage(const Ref<Texture>& texture, const void* data) override;
};
