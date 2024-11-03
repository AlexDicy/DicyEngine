#pragma once

class OpenGLShadowCubeArrayFramebuffer : public ShadowCubeArrayFramebuffer {
public:
    explicit OpenGLShadowCubeArrayFramebuffer(unsigned int size, unsigned int layersCount);
    ~OpenGLShadowCubeArrayFramebuffer() override;

    void bind(unsigned int layer) const override;

private:
    unsigned int id;
    unsigned int depthTextureId;
    unsigned int shadowCubeTextureId;
};
