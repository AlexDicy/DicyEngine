#include "pch/enginepch.h"
#include "OpenGLCommands.h"

#include "OpenGLTexture.h"
#include "OpenGLTypes.h"

void OpenGLCommands::initializeTexture(const Ref<Texture>& texture) const {
    const Ref<OpenGLTexture> t = std::static_pointer_cast<OpenGLTexture>(texture);
    glGenTextures(1, &t->id);
    glBindTexture(t->glTextureType, t->id);

    if (texture->getSamples() <= 1) {
        glTexParameteri(t->glTextureType, GL_TEXTURE_MIN_FILTER, OpenGLTypes::getFromTextureFilter(texture->getFilterMin()));
        glTexParameteri(t->glTextureType, GL_TEXTURE_MAG_FILTER, OpenGLTypes::getFromTextureFilter(texture->getFilterMag()));
        glTexParameteri(t->glTextureType, GL_TEXTURE_WRAP_S, OpenGLTypes::getFromTextureWrap(texture->getWrapU()));
        glTexParameteri(t->glTextureType, GL_TEXTURE_WRAP_T, OpenGLTypes::getFromTextureWrap(texture->getWrapV()));
        glTexParameteri(t->glTextureType, GL_TEXTURE_WRAP_R, OpenGLTypes::getFromTextureWrap(texture->getWrapW()));
    }
}

void OpenGLCommands::createTextureStorage(const Ref<Texture>& texture, const std::unique_ptr<uint8_t[]> data) const {
    const Ref<OpenGLTexture> t = std::static_pointer_cast<OpenGLTexture>(texture);
    t->bind();
    switch (t->glTextureType) {
        case GL_TEXTURE_2D:
            glTexImage2D(GL_TEXTURE_2D, 0, t->glInternalFormat, static_cast<int>(t->getWidth()), static_cast<int>(t->getHeight()), 0, t->glFormat, t->dataType, data.get());
            break;
        case GL_TEXTURE_2D_MULTISAMPLE:
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, static_cast<int>(t->getSamples()), t->glInternalFormat, static_cast<int>(t->getWidth()),
                                    static_cast<int>(t->getHeight()), true);
            break;
        case GL_TEXTURE_2D_ARRAY:
        case GL_TEXTURE_CUBE_MAP_ARRAY:
            glTexImage3D(t->glTextureType, 0, t->glInternalFormat, static_cast<int>(t->getWidth()), static_cast<int>(t->getHeight()), static_cast<int>(t->getLayers()), 0,
                         t->glFormat, t->dataType, data.get());
            break;
        case GL_TEXTURE_CUBE_MAP:
            {
                const size_t faceSize = static_cast<size_t>(t->getWidth()) * t->getHeight() * t->getInternalFormat().getSize();
                for (int face = 0; face < 6; face++) {
                    const uint8_t* const dataOffset = data == nullptr ? nullptr : data.get() + (face * faceSize);
                    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, t->glInternalFormat, static_cast<int>(t->getWidth()), static_cast<int>(t->getHeight()), 0, t->glFormat,
                                 t->dataType, dataOffset);
                }
                break;
            }
        default:
            DE_ASSERT(false, "Unsupported texture type")
    }

    if (t->shouldGenerateMipmaps()) {
        glGenerateMipmap(t->glTextureType);
    }
}
