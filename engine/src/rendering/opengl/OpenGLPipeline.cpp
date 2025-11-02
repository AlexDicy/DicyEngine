#include "pch/enginepch.h"
#include "OpenGLPipeline.h"

#include "OpenGLTypes.h"

#include <glad/gl.h>

void OpenGLPipeline::bind() const {
    if (params.blending.enabled) {
        glEnable(GL_BLEND);
        glBlendFuncSeparate(OpenGLTypes::getFromBlendFactor(params.blending.color.srcFactor), OpenGLTypes::getFromBlendFactor(params.blending.color.dstFactor),
                            OpenGLTypes::getFromBlendFactor(params.blending.alpha.srcFactor), OpenGLTypes::getFromBlendFactor(params.blending.alpha.dstFactor));
        glBlendEquationSeparate(OpenGLTypes::getFromBlendOp(params.blending.color.blendOp), OpenGLTypes::getFromBlendOp(params.blending.alpha.blendOp));
    } else {
        glDisable(GL_BLEND);
    }

    if (params.depth.test) {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(OpenGLTypes::getFromCompareOp(params.depth.compareOp));
    } else {
        glDisable(GL_DEPTH_TEST);
    }
    glDepthMask(params.depth.write ? GL_TRUE : GL_FALSE);

    if (params.cullMode == PipelineCullMode::NONE) {
        glDisable(GL_CULL_FACE);
    } else {
        glEnable(GL_CULL_FACE);
        switch (params.cullMode) {
            case PipelineCullMode::FRONT:
                glCullFace(GL_FRONT);
                break;
            case PipelineCullMode::BACK:
                glCullFace(GL_BACK);
                break;
            case PipelineCullMode::FRONT_AND_BACK:
                glCullFace(GL_FRONT_AND_BACK);
                break;
            case PipelineCullMode::NONE:
                DE_ASSERT(false, "Unreachable")
                break;
        }
    }
}
