#pragma once
#include "model.h"
#include "scene/entities/entity.h"

class ModelImporter {
public:
    static std::vector<Model> import_from_file(const std::string& filename);
};
