#pragma once

enum class DataType { BOOL, INT, INT2, INT3, INT4, FLOAT, FLOAT2, FLOAT3, FLOAT4 };

static uint32_t getDatatypeSize(const DataType type) {
    switch (type) {
        case DataType::BOOL:
            return 1;
        case DataType::INT:
            return 4;
        case DataType::INT2:
            return 4 * 2;
        case DataType::INT3:
            return 4 * 3;
        case DataType::INT4:
            return 4 * 4;
        case DataType::FLOAT:
            return 4;
        case DataType::FLOAT2:
            return 4 * 2;
        case DataType::FLOAT3:
            return 4 * 3;
        case DataType::FLOAT4:
            return 4 * 4;
    }
    return 0;
}

struct BufferAttribute {
    DataType type;
    std::string name;
    uint32_t size;
    uint32_t offset;
    bool isNormalized = false;

    BufferAttribute(const DataType type, std::string name) : type(type), name(std::move(name)), size(getDatatypeSize(type)), offset(0) {}

    uint8_t getDatatypeCount() const {
        switch (this->type) {
            case DataType::BOOL:
            case DataType::INT:
            case DataType::FLOAT:
                return 1;
            case DataType::INT2:
            case DataType::FLOAT2:
                return 2;
            case DataType::INT3:
            case DataType::FLOAT3:
                return 3;
            case DataType::INT4:
            case DataType::FLOAT4:
                return 4;
        }
        return 0;
    }
};

class BufferLayout {
    std::vector<BufferAttribute> attributes;
    uint32_t size;

public:
    BufferLayout() = default;

    BufferLayout(const std::initializer_list<BufferAttribute>& attributes) : attributes(attributes) {
        uint32_t lastOffset = 0;
        for (auto& attribute : this->attributes) {
            attribute.offset = lastOffset;
            lastOffset += attribute.size;
        }
        size = lastOffset;
    }

    const std::vector<BufferAttribute>& getAttributes() const {
        return this->attributes;
    }

    uint32_t getSize() const {
        return this->size;
    }
};
