#pragma once
#include "model.h"
#include "scene/entities/entity.h"

class ModelImporter {
public:
    static Model import_from_file(const std::string& filename, const Ref<Entity>& temp_entity, const Ref<Renderer>& renderer);
};
