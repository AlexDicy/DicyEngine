#pragma once

class TextureFormat {
public:
    enum FormatEnum : unsigned char {
        R,
        R_INT,
        RG,
        RG_INT,
        RGB,
        RGB_INT,
        RGBA,
        RGBA_INT,
        BGRA, // TODO: test removing this, see if the UI texture still works
        DEPTH,
        DEPTH_STENCIL,
    };

    constexpr TextureFormat() = default;
    /* implicit */ constexpr TextureFormat(const FormatEnum e) : e(e) {}

    // ReSharper disable once CppNonExplicitConversionOperator
    // allows comparisons with Enum constants.
    constexpr operator FormatEnum() const {
        return e;
    }

    // prevent if (x)
    explicit operator bool() const = delete;

    constexpr unsigned int getChannelCount() const {
        switch (e) {
            case R:
            case R_INT:
            case DEPTH:
                return 1;
            case RG:
            case RG_INT:
            case DEPTH_STENCIL:
                return 2;
            case RGB:
            case RGB_INT:
                return 3;
            case RGBA:
            case RGBA_INT:
            case BGRA:
                return 4;
            default:
                return 0;
        }
    }

private:
    FormatEnum e;
};


class TextureDataType {
public:
    // ReSharper disable CppInconsistentNaming, IdentifierTypo
    enum DataTypeEnum {
        BYTE,
        UBYTE,
        SHORT,
        USHORT,
        INT,
        UINT,
        HALF_FLOAT,
        FLOAT,
    };
    // ReSharper restore CppInconsistentNaming, IdentifierTypo

    constexpr TextureDataType() = default;
    /* implicit */ constexpr TextureDataType(const DataTypeEnum e) : e(e) {}

    // ReSharper disable once CppNonExplicitConversionOperator
    // allows comparisons with Enum constants.
    constexpr operator DataTypeEnum() const {
        return e;
    }

    // prevent if (x)
    explicit operator bool() const = delete;

    constexpr unsigned int getByteSize() const {
        switch (e) {
            case INT:
                return 4;
            case FLOAT:
                return 4;
            default:
                return 0;
        }
    }

private:
    DataTypeEnum e;
};


class TextureInternalFormat {
public:
    // ReSharper disable CppInconsistentNaming
    enum InternalFormatEnum {
        R8,
        R8_SNORM,
        R8_UINT,
        R8_INT,
        R16,
        R16_SNORM,
        R16_UINT,
        R16_INT,
        R16_FLOAT,
        R32_UINT,
        R32_INT,
        R32_FLOAT,
        RG8,
        RG8_SNORM,
        RG8_UINT,
        RG8_INT,
        RG16,
        RG16_SNORM,
        RG16_UINT,
        RG16_INT,
        RG16_FLOAT,
        RG32_UINT,
        RG32_INT,
        RG32_FLOAT,
        RGB8,
        RGB8_SNORM,
        RGB8_UINT,
        RGB8_INT,
        RGBA8,
        RGBA8_SNORM,
        RGBA8_UINT,
        RGBA8_INT,
        RGBA16,
        RGBA16_SNORM,
        RGBA16_UINT,
        RGBA16_INT,
        RGBA16_FLOAT,
        RGBA32_UINT,
        RGBA32_INT,
        RGBA32_FLOAT,
        D16,
        D24,
        D32,
        D24S8,
    };
    // ReSharper restore CppInconsistentNaming

    constexpr TextureInternalFormat() = default;
    /* implicit */ constexpr TextureInternalFormat(const InternalFormatEnum e) : e(e) {}

    // ReSharper disable once CppNonExplicitConversionOperator
    // allows comparisons with Enum constants.
    constexpr operator InternalFormatEnum() const {
        return e;
    }

    // prevent if (x)
    explicit operator bool() const = delete;

    constexpr TextureDataType getDataType() const {
        switch (e) {
            case R8_SNORM:
            case RG8_SNORM:
            case RGB8_SNORM:
            case RGBA8_SNORM:
            case R8_INT:
            case RG8_INT:
            case RGB8_INT:
            case RGBA8_INT:
                return TextureDataType::BYTE;
            case R8:
            case RG8:
            case RGB8:
            case RGBA8:
            case R8_UINT:
            case RG8_UINT:
            case RGB8_UINT:
            case RGBA8_UINT:
                return TextureDataType::UBYTE;
            case R16_SNORM:
            case RG16_SNORM:
            case RGBA16_SNORM:
            case R16_INT:
            case RG16_INT:
            case RGBA16_INT:
                return TextureDataType::SHORT;
            case R16:
            case RG16:
            case RGBA16:
            case R16_UINT:
            case RG16_UINT:
            case RGBA16_UINT:
            case D16:
                return TextureDataType::USHORT;
            case R32_INT:
            case RG32_INT:
            case RGBA32_INT:
                return TextureDataType::INT;
            case R32_UINT:
            case RG32_UINT:
            case RGBA32_UINT:
            case D24:
            case D24S8:
                return TextureDataType::UINT;
            case R16_FLOAT:
            case RG16_FLOAT:
            case RGBA16_FLOAT:
                return TextureDataType::HALF_FLOAT;
            case R32_FLOAT:
            case RG32_FLOAT:
            case RGBA32_FLOAT:
            case D32:
                return TextureDataType::FLOAT;
            default:
                return TextureDataType::FLOAT;
        }
    }

    constexpr unsigned int getSize() const {
        switch (e) {
            case R8:
            case R8_SNORM:
            case R8_UINT:
            case R8_INT:
                return 1;
            case R16:
            case R16_SNORM:
            case R16_UINT:
            case R16_INT:
            case R16_FLOAT:
            case RG8:
            case RG8_SNORM:
            case RG8_UINT:
            case RG8_INT:
            case D16:
                return 2;
            case RGB8:
            case RGB8_SNORM:
            case RGB8_UINT:
            case RGB8_INT:
            case D24:
                return 3;
            case R32_UINT:
            case R32_INT:
            case R32_FLOAT:
            case RG16:
            case RG16_SNORM:
            case RG16_UINT:
            case RG16_INT:
            case RG16_FLOAT:
            case RGBA8:
            case RGBA8_SNORM:
            case RGBA8_UINT:
            case RGBA8_INT:
            case D32:
            case D24S8:
                return 4;
            case RG32_UINT:
            case RG32_INT:
            case RG32_FLOAT:
            case RGBA16:
            case RGBA16_SNORM:
            case RGBA16_UINT:
            case RGBA16_INT:
            case RGBA16_FLOAT:
                return 8;
            case RGBA32_UINT:
            case RGBA32_INT:
            case RGBA32_FLOAT:
                return 16;
            default:
                return 0;
        }
    }

private:
    InternalFormatEnum e;
};

class TextureType {
public:
    // ReSharper disable CppInconsistentNaming
    enum TextureTypeEnum {
        TEXTURE_2D,
        TEXTURE_2D_ARRAY,
        TEXTURE_CUBE,
        TEXTURE_CUBE_ARRAY,
    };
    // ReSharper restore CppInconsistentNaming

    constexpr TextureType() = default;
    /* implicit */ constexpr TextureType(const TextureTypeEnum e) : e(e) {}

    // ReSharper disable once CppNonExplicitConversionOperator
    // allows comparisons with Enum constants.
    constexpr operator TextureTypeEnum() const {
        return e;
    }

    // prevent if (x)
    explicit operator bool() const = delete;

private:
    TextureTypeEnum e;
};

class TextureWrap {
public:
    // ReSharper disable CppInconsistentNaming, IdentifierTypo
    enum WrapEnum {
        REPEAT,
        MIRRORED_REPEAT,
        CLAMP_TO_EDGE,
        CLAMP_TO_BORDER,
    };
    // ReSharper restore CppInconsistentNaming, IdentifierTypo

    constexpr TextureWrap() = default;
    /* implicit */ constexpr TextureWrap(const WrapEnum e) : e(e) {}

    // ReSharper disable once CppNonExplicitConversionOperator
    // allows comparisons with Enum constants.
    constexpr operator WrapEnum() const {
        return e;
    }

    // prevent if (x)
    explicit operator bool() const = delete;

private:
    WrapEnum e;
};

class TextureFilter {
public:
    // ReSharper disable CppInconsistentNaming, IdentifierTypo
    enum FilterEnum {
        NEAREST,
        LINEAR,
        MIPMAP_NEAREST,
        MIPMAP_LINEAR,
    };
    // ReSharper restore CppInconsistentNaming, IdentifierTypo

    constexpr TextureFilter() = default;
    /* implicit */ constexpr TextureFilter(const FilterEnum e) : e(e) {}

    // ReSharper disable once CppNonExplicitConversionOperator
    // allows comparisons with Enum constants.
    constexpr operator FilterEnum() const {
        return e;
    }

    // prevent if (x)
    explicit operator bool() const = delete;

private:
    FilterEnum e;
};
