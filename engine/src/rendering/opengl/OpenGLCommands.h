#pragma once
#include "rendering/RenderCommands.h"

class OpenGLCommands : public RenderCommands {
public:
    void initializeTexture(const Ref<Texture>& texture) const override;
    void createTextureStorage(const Ref<Texture>& texture, std::unique_ptr<uint8_t[]> data) const override;
    void bindTexture(const Ref<const Texture>& texture) const override;
    void copyTextureData(const Ref<const Texture>& src, unsigned level, void* destination) const override;
};
