#pragma once
#include "rendering/RenderCommands.h"

class OpenGLCommands : public RenderCommands {
public:
    void initializeTexture(const Ref<Texture>& texture) const override;
    void createTextureStorage(const Ref<Texture>& texture, Ref<uint8_t[]> data) const override;
    void bindTexture(const Ref<const Texture>& texture) const override;
    void bindTexture(const Ref<const Texture>& texture, unsigned slot) const override;
    void clearTexture(const Ref<const Texture>& texture, Ref<uint8_t[]> color) const override;
    void copyTextureData(const Ref<const Texture>& src, unsigned level, void* destination) const override;

    void initializeFramebuffer(const Ref<Framebuffer>& framebuffer) const override;
    void bindFramebuffer(const Ref<const Framebuffer>& framebuffer) const override;
    void clearFramebuffer(const Ref<const Framebuffer>& framebuffer) const override;
    int readPixelInt(const Ref<const Framebuffer>& framebuffer, unsigned int x, unsigned int y, unsigned int attachmentIndex) const override;
    void copyColorData(const Ref<const Framebuffer>& src, const Ref<const Framebuffer>& dst, unsigned int srcAttachmentIndex, unsigned int dstAttachmentIndex) const override;
    void copyColorDataToScreen(const Ref<const Framebuffer>& src, unsigned int srcAttachmentIndex) const override;
};
