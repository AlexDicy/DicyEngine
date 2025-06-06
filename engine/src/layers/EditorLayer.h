#pragma once
#include "Layer.h"
#include "editor/scripts/EditorScript.h"

class EditorLayer : public Layer {
public:
    explicit EditorLayer(const std::unique_ptr<Context>& ctx);

    void play(const std::unique_ptr<Context>& ctx) override {}
    void update(const std::unique_ptr<Context>& ctx) override;
    void stop(const std::unique_ptr<Context>& ctx) override {}

private:
    Ref<EditorScript> editorScript;
    Ref<Mesh> uiMesh;
    Ref<Shader> uiShader;
};
