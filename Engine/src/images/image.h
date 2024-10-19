#pragma once

class Image {
public:
    Image(uint64_t width, uint64_t height);

    uint64_t get_width() const {
        return width;
    }

    uint64_t get_height() const {
        return height;
    }

private:
    uint64_t width;
    uint64_t height;
};
