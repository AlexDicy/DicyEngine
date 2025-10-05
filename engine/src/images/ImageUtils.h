#pragma once
#include "LinearImage.h"

class ImageUtils {
public:
    static Ref<Texture> loadTextureFromFile(const Ref<Renderer>& renderer, const std::string& path);
    static Ref<LinearImage> acesFilmicTonemapping(const Ref<LinearImage>& image);
};
