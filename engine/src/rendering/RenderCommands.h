#pragma once

/**
 * The Renderer enqueues rendering commands, this class is what actually executes them.
 */
class RenderCommands : public std::enable_shared_from_this<RenderCommands> {
public:
    virtual ~RenderCommands() = default;

    virtual void initializeTexture(const Ref<Texture>& texture) const = 0;
    virtual void createTextureStorage(const Ref<Texture>& texture, Ref<uint8_t[]> data) const = 0;
    virtual void bindTexture(const Ref<const Texture>& texture) const {} // TODO: implement and only bind if not already bound
    virtual void bindTexture(const Ref<const Texture>& texture, unsigned int slot) const = 0;
    virtual void clearTexture(const Ref<const Texture>& texture, Ref<uint8_t[]> color) const = 0;
    virtual void copyTextureData(const Ref<const Texture>& src, unsigned int level, void* destination) const = 0;

    virtual void initializeFramebuffer(const Ref<Framebuffer>& framebuffer) const = 0;
    virtual void bindFramebuffer(const Ref<const Framebuffer>& framebuffer) const = 0;
    virtual void clearFramebuffer(const Ref<const Framebuffer>& framebuffer) const = 0;
    virtual int readPixelInt(const Ref<const Framebuffer>& framebuffer, unsigned int x, unsigned int y, unsigned int attachmentIndex) const = 0;
    virtual void copyColorData(const Ref<const Framebuffer>& src, const Ref<const Framebuffer>& dst, unsigned int srcAttachmentIndex, unsigned int dstAttachmentIndex) const = 0;
    virtual void copyColorDataToScreen(const Ref<const Framebuffer>& src, unsigned int srcAttachmentIndex) const = 0;

    static void run(const std::function<void()>& function) {
        function();
    }
};
