#pragma once
#include "TextureFormats.h"
#include "images/Image.h"


class CubeMap;
class Renderer;
class TextureBuilder;

class Texture : public std::enable_shared_from_this<Texture> {
public:
    virtual ~Texture() = default;

    unsigned int getWidth() const {
        return this->params.width;
    }

    unsigned int getHeight() const {
        return this->params.height;
    }

    unsigned int getLayers() const {
        return this->params.layers;
    }

    unsigned int getSamples() const {
        return this->params.samples;
    }

    TextureFormat getFormat() const {
        return this->params.format;
    }

    TextureInternalFormat getInternalFormat() const {
        return this->params.internalFormat;
    }

    TextureType getType() const {
        return this->params.type;
    }

    TextureFilter getFilterMin() const {
        return this->params.filterMin;
    }

    TextureFilter getFilterMag() const {
        return this->params.filterMag;
    }

    TextureWrap getWrapU() const {
        return this->params.wrapU;
    }

    TextureWrap getWrapV() const {
        return this->params.wrapV;
    }

    TextureWrap getWrapW() const {
        return this->params.wrapW;
    }

    bool shouldGenerateMipmaps() const {
        return this->params.generateMipmaps;
    }

    virtual void bind(unsigned int slot) const = 0;

    virtual void setRawData(const void* data, unsigned int size) = 0;
    virtual void resize(unsigned int width, unsigned int height) = 0;
    virtual void resize(unsigned int width, unsigned int height, unsigned int layers) = 0;

    virtual Ref<CubeMap> toCubemap() const = 0;

    static TextureBuilder builder();

    struct TextureParams {
        unsigned int width = 0;
        unsigned int height = 0;
        unsigned int layers = 1;
        unsigned int samples = 1;
        TextureFormat format = TextureFormat::RGBA;
        TextureInternalFormat internalFormat = TextureInternalFormat::RGBA8;
        TextureType type = TextureType::TEXTURE_2D;
        TextureFilter filterMin = TextureFilter::LINEAR;
        TextureFilter filterMag = TextureFilter::LINEAR;
        TextureWrap wrapU = TextureWrap::REPEAT;
        TextureWrap wrapV = TextureWrap::REPEAT;
        TextureWrap wrapW = TextureWrap::REPEAT;
        bool generateMipmaps = false;
    };

protected:
    Texture(const TextureParams& params, const Ref<Renderer>& renderer);

    Ref<Renderer> getRenderer() const {
        Ref<Renderer> ref = renderer.lock();
        if (!ref) {
            throw std::runtime_error("The renderer associated with this texture has been destroyed.");
        }
        return ref;
    }

    TextureParams params;

private:
    std::weak_ptr<Renderer> renderer;
};


class TextureCubeUtils {
public:
    static const glm::mat4 invertedViewMatrices[6];
    static const glm::mat4 shadowViewMatrices[6];
};


class TextureBuilder {
public:
    TextureBuilder& width(const unsigned int width) {
        this->params.width = width;
        return *this;
    }

    TextureBuilder& height(const unsigned int height) {
        this->params.height = height;
        return *this;
    }

    TextureBuilder& size(const unsigned int size) {
        this->width(size);
        this->height(size);
        return *this;
    }

    TextureBuilder& size(const unsigned int width, const unsigned int height) {
        this->width(width);
        this->height(height);
        return *this;
    }

    TextureBuilder& layers(const unsigned int layers) {
        this->params.layers = layers;
        return *this;
    }

    TextureBuilder& samples(const unsigned int samples) {
        this->params.samples = samples;
        return *this;
    }

    TextureBuilder& format(const TextureFormat format) {
        this->params.format = format;
        return *this;
    }

    TextureBuilder& internalFormat(const TextureInternalFormat internalFormat) {
        this->params.internalFormat = internalFormat;
        return *this;
    }

    TextureBuilder& type(const TextureType type) {
        this->params.type = type;
        return *this;
    }

    TextureBuilder& filterMin(const TextureFilter filter) {
        this->params.filterMin = filter;
        return *this;
    }

    TextureBuilder& filterMag(const TextureFilter filter) {
        this->params.filterMag = filter;
        return *this;
    }

    TextureBuilder& filter(const TextureFilter filter) {
        this->filterMin(filter);
        this->filterMag(filter);
        return *this;
    }

    TextureBuilder& filter(const TextureFilter minFilter, const TextureFilter maxFilter) {
        this->filterMin(minFilter);
        this->filterMag(maxFilter);
        return *this;
    }

    TextureBuilder& wrapU(const TextureWrap wrap) {
        this->params.wrapU = wrap;
        return *this;
    }

    TextureBuilder& wrapV(const TextureWrap wrap) {
        this->params.wrapV = wrap;
        return *this;
    }

    TextureBuilder& wrapW(const TextureWrap wrap) {
        this->params.wrapW = wrap;
        return *this;
    }

    TextureBuilder& wrapUV(const TextureWrap wrap) {
        this->wrapU(wrap);
        this->wrapV(wrap);
        return *this;
    }

    TextureBuilder& wrap(const TextureWrap wrap) {
        this->wrapU(wrap);
        this->wrapV(wrap);
        this->wrapW(wrap);
        return *this;
    }

    TextureBuilder& generateMipmaps(const bool generateMipmaps) {
        this->params.generateMipmaps = generateMipmaps;
        return *this;
    }

    TextureBuilder& data(std::unique_ptr<uint8_t[]> data) {
        this->textureData = std::move(data);
        return *this;
    }

    TextureBuilder& fromImage(const Ref<Image>& image);

    Ref<Texture> build(const Ref<Renderer>& renderer);

private:
    friend class Texture;
    TextureBuilder() = default;

    Texture::TextureParams params;
    std::unique_ptr<uint8_t[]> textureData = nullptr;
};
