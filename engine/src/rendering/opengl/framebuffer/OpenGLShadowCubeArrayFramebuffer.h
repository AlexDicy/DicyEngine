#pragma once

class OpenGLShadowCubeArrayFramebuffer : public ShadowCubeArrayFramebuffer {
public:
    explicit OpenGLShadowCubeArrayFramebuffer(const Ref<Renderer>& renderer, unsigned int size);
    ~OpenGLShadowCubeArrayFramebuffer() override;

    void bind(unsigned int layer, unsigned int face) const override;

    void ensureLayersCapacity(unsigned int layersCount) override;

private:
    unsigned int id;
    Ref<Texture> depthTexture;
};
