#pragma once
#include "LinearImage.h"

class ImageUtils {
public:
    static Ref<LinearImage> acesFilmicTonemapping(const Ref<LinearImage>& image);
};
