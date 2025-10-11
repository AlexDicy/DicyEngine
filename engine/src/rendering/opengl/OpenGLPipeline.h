#pragma once
#include "rendering/Pipeline.h"

class OpenGLPipeline : public Pipeline {
public:
    void bind() const override;
};
