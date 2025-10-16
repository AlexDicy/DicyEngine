#pragma once

/**
 * The Renderer enqueues rendering commands, this class is what actually executes them.
 */
class RenderCommands : public std::enable_shared_from_this<RenderCommands> {
public:
    virtual ~RenderCommands() = default;

    virtual void initializeTexture(const Ref<Texture>& texture) const = 0;
    virtual void createTextureStorage(const Ref<Texture>& texture, std::unique_ptr<uint8_t[]> data) const = 0;
    virtual void bindTexture(const Ref<const Texture>& texture) const {} // TODO: implement and only bind if not already bound
    virtual void copyTextureData(const Ref<const Texture>& src, unsigned int level, void* destination) const = 0;

    static void run(const std::function<void()>& function) {
        function();
    }
};
