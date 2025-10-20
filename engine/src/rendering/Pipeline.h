#pragma once

enum class PipelineBlendFactor : byte {
    ZERO,
    ONE,
    SRC_COLOR,
    ONE_MINUS_SRC_COLOR,
    DST_COLOR,
    ONE_MINUS_DST_COLOR,
    SRC_ALPHA,
    ONE_MINUS_SRC_ALPHA,
    DST_ALPHA,
    ONE_MINUS_DST_ALPHA,
    CONSTANT_COLOR,
    ONE_MINUS_CONSTANT_COLOR,
    CONSTANT_ALPHA,
    ONE_MINUS_CONSTANT_ALPHA,
    SRC_ALPHA_SATURATE,
};

enum class PipelineBlendOp : byte {
    ADD,
    SUBTRACT,
    REVERSE_SUBTRACT,
    MIN,
    MAX,
};

enum class PipelineCompareOp : byte {
    NEVER,
    LESS,
    EQUAL,
    LESS_EQUAL,
    GREATER,
    NOT_EQUAL,
    GREATER_EQUAL,
    ALWAYS,
};

enum class PipelineCullMode : byte {
    NONE,
    FRONT,
    BACK,
    FRONT_AND_BACK,
};

struct PipelineParams {
    struct {
        struct {
            PipelineBlendFactor srcFactor = PipelineBlendFactor::ONE;
            PipelineBlendFactor dstFactor = PipelineBlendFactor::ZERO;
            PipelineBlendOp blendOp = PipelineBlendOp::ADD;
        } color;

        struct {
            PipelineBlendFactor srcFactor = PipelineBlendFactor::ONE;
            PipelineBlendFactor dstFactor = PipelineBlendFactor::ZERO;
            PipelineBlendOp blendOp = PipelineBlendOp::ADD;
        } alpha;

        bool enabled = false;
    } blending;

    struct {
        PipelineCompareOp compareOp = PipelineCompareOp::LESS;
        bool test = true;
        bool write = true;
    } depth;

    PipelineCullMode cullMode = PipelineCullMode::BACK;
};

class Pipeline {
public:
    Pipeline() = default;
    virtual ~Pipeline() = default;

    virtual void bind() const = 0;

    PipelineParams params;
};
