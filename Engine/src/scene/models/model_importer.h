#pragma once
#include "model.h"

class ModelImporter {
public:
    static Model import_from_file(const std::string& filename);
};
