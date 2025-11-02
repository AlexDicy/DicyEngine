#include "pch/enginepch.h"
#include "OpenGLCommands.h"

#include "OpenGLFramebuffer.h"
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

void OpenGLCommands::createTextureStorage(const Ref<Texture>& texture, const Ref<uint8_t[]> data) const {
    const Ref<OpenGLTexture> t = std::static_pointer_cast<OpenGLTexture>(texture);
    bindTexture(texture);
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

void OpenGLCommands::bindTexture(const Ref<const Texture>& texture) const {
    const Ref<const OpenGLTexture> t = std::static_pointer_cast<const OpenGLTexture>(texture);
    glBindTexture(t->glTextureType, t->id);
}

void OpenGLCommands::bindTexture(const Ref<const Texture>& texture, const unsigned slot) const {
    glBindTextureUnit(slot, std::static_pointer_cast<const OpenGLTexture>(texture)->id);
}

void OpenGLCommands::clearTexture(const Ref<const Texture>& texture, const Ref<uint8_t[]> color) const {
    bindTexture(texture);
    const Ref<const OpenGLTexture> t = std::static_pointer_cast<const OpenGLTexture>(texture);
    glClearTexImage(t->id, 0, t->glFormat, t->dataType, color.get());
}

void OpenGLCommands::copyTextureData(const Ref<const Texture>& src, const unsigned level, void* destination) const {
    if (src->getType() != TextureType::TEXTURE_CUBE) {
        glGetTexImage(OpenGLTypes::getFromTextureType(src->getType(), src->getSamples()), static_cast<GLint>(level), OpenGLTypes::getFromTextureFormat(src->getFormat()),
                      OpenGLTypes::getPixelTypeFromInternalFormat(src->getInternalFormat()), destination);
        return;
    }
    // using level as the face index here, refactor if needed
    glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X + level, 0, OpenGLTypes::getFromTextureFormat(src->getFormat()),
                  OpenGLTypes::getPixelTypeFromInternalFormat(src->getInternalFormat()), destination);
}

void OpenGLCommands::initializeFramebuffer(const Ref<Framebuffer>& framebuffer) const {
    const Ref<OpenGLFramebuffer> fb = std::static_pointer_cast<OpenGLFramebuffer>(framebuffer);
    glGenFramebuffers(1, &fb->id);
    glBindFramebuffer(GL_FRAMEBUFFER, fb->id);

    std::vector<GLenum> drawBuffers;
    for (unsigned int i = 0; i < fb->params.colorAttachments.size(); i++) {
        const Ref<const OpenGLTexture> colorAttachment = std::static_pointer_cast<const OpenGLTexture>(fb->params.colorAttachments[i]);
        const GLenum attachment = GL_COLOR_ATTACHMENT0 + i;
        if (colorAttachment->getSamples() > 1) {
            glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D_MULTISAMPLE, colorAttachment->id, 0);
        } else {
            glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, colorAttachment->glTextureType, colorAttachment->id, 0);
        }
        drawBuffers.push_back(attachment);
    }

    if (!drawBuffers.empty()) {
        glDrawBuffers(static_cast<GLsizei>(drawBuffers.size()), drawBuffers.data());
    }

    if (fb->params.depthAttachment) {
        const Ref<const OpenGLTexture> depthAttachment = std::static_pointer_cast<const OpenGLTexture>(fb->params.depthAttachment);
        if (depthAttachment->getSamples() > 1) {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, depthAttachment->id, 0);
        } else {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthAttachment->glTextureType, depthAttachment->id, 0);
        }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLCommands::bindFramebuffer(const Ref<const Framebuffer>& framebuffer) const {
    const Ref<const OpenGLFramebuffer> fb = std::static_pointer_cast<const OpenGLFramebuffer>(framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, fb->id);
}

void OpenGLCommands::clearFramebuffer(const Ref<const Framebuffer>& framebuffer) const {
    bindFramebuffer(framebuffer);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

int OpenGLCommands::readPixelInt(const Ref<const Framebuffer>& framebuffer, const unsigned int x, const unsigned int y, const unsigned int attachmentIndex) const {
    DE_ASSERT(static_cast<size_t>(attachmentIndex) < framebuffer->getColorAttachments().size(), "Attachment index out of bounds when reading pixel from framebuffer")
    bindFramebuffer(framebuffer);
    const GLenum attachment = GL_COLOR_ATTACHMENT0 + attachmentIndex;
    glReadBuffer(attachment);
    const GLenum format = std::static_pointer_cast<const OpenGLTexture>(framebuffer->getColorAttachments()[attachmentIndex])->glFormat;
    int pixelData = -1;
    glReadPixels(static_cast<int>(x), static_cast<int>(framebuffer->getHeight() - y), 1, 1, format, GL_INT, &pixelData);
    return pixelData;
}

void OpenGLCommands::copyColorData(const Ref<const Framebuffer>& src, const Ref<const Framebuffer>& dst, const unsigned int srcAttachmentIndex,
                                   const unsigned int dstAttachmentIndex) const {
    DE_ASSERT(static_cast<size_t>(srcAttachmentIndex) < src->getColorAttachments().size(), "Source attachment index out of bounds when copying color data between framebuffers")
    DE_ASSERT(static_cast<size_t>(dstAttachmentIndex) < dst->getColorAttachments().size(),
              "Destination attachment index out of bounds when copying color data between framebuffers")
    DE_ASSERT(src->getWidth() == dst->getWidth() && src->getHeight() == dst->getHeight(),
              "Source and destination framebuffers must have the same dimensions when copying color data between them")
    const GLenum srcAttachment = GL_COLOR_ATTACHMENT0 + srcAttachmentIndex;
    const GLenum dstAttachment = GL_COLOR_ATTACHMENT0 + dstAttachmentIndex;
    glBindFramebuffer(GL_READ_FRAMEBUFFER, std::static_pointer_cast<const OpenGLFramebuffer>(src)->id);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, std::static_pointer_cast<const OpenGLFramebuffer>(dst)->id);
    glReadBuffer(srcAttachment);
    glDrawBuffer(dstAttachment);
    glBlitFramebuffer(0, 0, static_cast<int>(src->getWidth()), static_cast<int>(src->getHeight()), 0, 0, static_cast<int>(dst->getWidth()), static_cast<int>(dst->getHeight()),
                      GL_COLOR_BUFFER_BIT, GL_NEAREST);
    bindFramebuffer(src);
}

void OpenGLCommands::copyColorDataToScreen(const Ref<const Framebuffer>& src, unsigned int srcAttachmentIndex) const {
    DE_ASSERT(static_cast<size_t>(srcAttachmentIndex) < src->getColorAttachments().size(), "Source attachment index out of bounds when copying color data to screen")
    const GLenum srcAttachment = GL_COLOR_ATTACHMENT0 + srcAttachmentIndex;
    glBindFramebuffer(GL_READ_FRAMEBUFFER, std::static_pointer_cast<const OpenGLFramebuffer>(src)->id);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glReadBuffer(srcAttachment);
    glBlitFramebuffer(0, 0, static_cast<int>(src->getWidth()), static_cast<int>(src->getHeight()), 0, 0, static_cast<int>(src->getWidth()), static_cast<int>(src->getHeight()),
                      GL_COLOR_BUFFER_BIT, GL_NEAREST);
    bindFramebuffer(src);
}

void OpenGLCommands::copyDepthData(const Ref<const Framebuffer>& src, const Ref<const Framebuffer>& dst) const {
    DE_ASSERT(src->getWidth() == dst->getWidth() && src->getHeight() == dst->getHeight(),
              "Source and destination framebuffers must have the same dimensions when copying depth data between them")
    glBindFramebuffer(GL_READ_FRAMEBUFFER, std::static_pointer_cast<const OpenGLFramebuffer>(src)->id);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, std::static_pointer_cast<const OpenGLFramebuffer>(dst)->id);
    glBlitFramebuffer(0, 0, static_cast<int>(src->getWidth()), static_cast<int>(src->getHeight()), 0, 0, static_cast<int>(dst->getWidth()), static_cast<int>(dst->getHeight()),
                      GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    bindFramebuffer(src);
}
