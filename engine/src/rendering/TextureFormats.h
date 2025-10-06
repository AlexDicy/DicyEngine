#pragma once

class TextureFormats {
public:
    class Format {
    public:
        enum FormatEnum : unsigned char {
            RED,
            RG,
            RGB,
            RGBA,
            BGRA,
            DEPTH,
        };

        constexpr Format() = default;
        /* implicit */ constexpr Format(const FormatEnum e) : e(e) {}

        // ReSharper disable once CppNonExplicitConversionOperator
        // allows comparisons with Enum constants.
        constexpr operator FormatEnum() const {
            return e;
        }

        // prevent if (x)
        explicit operator bool() const = delete;

        constexpr unsigned int getChannelCount() const {
            switch (e) {
                case RED:
                    return 1;
                case RG:
                    return 2;
                case RGB:
                    return 3;
                case BGRA:
                case RGBA:
                    return 4;
                case DEPTH:
                    return 1;
                default:
                    return 0;
            }
        }

    private:
        FormatEnum e;
    };


    class DataType {
    public:
        // ReSharper disable CppInconsistentNaming
        enum DataTypeEnum {
            INT,
            FLOAT,
        };
        // ReSharper restore CppInconsistentNaming

        constexpr DataType() = default;
        /* implicit */ constexpr DataType(const DataTypeEnum e) : e(e) {}

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


    class InternalFormat {
    public:
        // ReSharper disable CppInconsistentNaming
        enum InternalFormatEnum {
            R8,
            R16,
            R16F,
            R32F,
            RG8,
            RG16,
            RG16F,
            RG32F,
            RGB8,
            RGB16,
            RGB16F,
            RGB32F,
            RGBA8,
            RGBA16,
            RGBA16F,
            RGBA32F,
            D16,
            D32,
            D24S8,
        };
        // ReSharper restore CppInconsistentNaming

        constexpr InternalFormat() = default;
        /* implicit */ constexpr InternalFormat(const InternalFormatEnum e) : e(e) {}

        // ReSharper disable once CppNonExplicitConversionOperator
        // allows comparisons with Enum constants.
        constexpr operator InternalFormatEnum() const {
            return e;
        }

        // prevent if (x)
        explicit operator bool() const = delete;

        constexpr DataType getDataType() const {
            switch (e) {
                case R8:
                case R16:
                case RG8:
                case RG16:
                case RGB8:
                case RGB16:
                case RGBA8:
                case RGBA16:
                    return DataType::INT;
                case R16F:
                case R32F:
                case RG16F:
                case RG32F:
                case RGB16F:
                case RGB32F:
                case RGBA16F:
                case RGBA32F:
                    return DataType::FLOAT;
                case D16:
                    return DataType::INT;
                case D32:
                    return DataType::FLOAT;
                case D24S8:
                    return DataType::INT;
                default:
                    return DataType::INT;
            }
        }

        constexpr unsigned int getSize() const {
            switch (e) {
                case R8:
                    return 1;
                case R16:
                case R16F:
                case RG8:
                case D16:
                    return 2;
                case RGB8:
                    return 3;
                case R32F:
                case RG16:
                case RG16F:
                case RGBA8:
                case D32:
                case D24S8:
                    return 4;
                case RGB16:
                case RGB16F:
                    return 6;
                case RG32F:
                case RGBA16:
                case RGBA16F:
                    return 8;
                case RGB32F:
                    return 12;
                case RGBA32F:
                    return 16;
                default:
                    return 0;
            }
        }

    private:
        InternalFormatEnum e;
    };
};
