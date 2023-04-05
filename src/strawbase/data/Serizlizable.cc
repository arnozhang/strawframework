#include "base/data/Array.h"

#include "base/data/Data.h"
#include "base/data/Buffer.h"
#include "base/data/NumericSerializable.h"


namespace {

    std::map<uint16_t, sf::Serializable::SerializableCreator>
        g_SerizlizableCreatorMap;

} // anonymous namespace ends here.


size_t sf::Serializable::deserialize(const Buffer* buffer) {
    if (!buffer || buffer->empty()) {
        return 0;
    }

    return deserialize(buffer->buffer());
}

size_t sf::Serializable::deserialize(const Buffer& buffer) {
    if (buffer.empty()) {
        return 0;
    }

    return deserialize(buffer.buffer());
}

sf::Buffer* sf::Serializable::serialize() const {
    size_t total = byteSize();
    char* buffer = new char[total];
    serialize(buffer);

    return new sf::Buffer(buffer, total);
}

void sf::Serializable::registerSerializableCreator(
    uint16_t type, SerializableCreator creator) {

    g_SerizlizableCreatorMap[type] = creator;
}

sf::Serializable* sf::Serializable::createByType(sf::SerializableType type) {
    if (type >= t_CustomizeData) {
        auto iter = g_SerizlizableCreatorMap.find(type);
        if (iter != g_SerizlizableCreatorMap.end()) {
            return iter->second(type);
        }

        return nullptr;
    }

    switch (type) {
        case sf::t_Char:
            return new Char();
            break;
        case sf::t_Boolean:
            return new Boolean();
            break;
        case sf::t_Short:
            return new Short();
            break;
        case sf::t_Integer:
            return new Integer();
            break;
        case sf::t_Long:
            return new Long();
            break;
        case sf::t_UInt64:
            return new UInt64();
            break;
        case sf::t_Float:
            return new Float();
            break;
        case sf::t_Double:
            return new Double();
            break;
        case sf::t_String:
            return new String();
            break;
        case sf::t_Array:
            return new Array();
            break;
        case sf::t_Data:
            return new Data();
            break;
        case sf::t_BinaryBuffer:
            return new BinaryBuffer();
            break;
        case sf::t_Unknown:
        default:
            break;
    }

    return nullptr;
}

size_t sf::Serializable::serializeBuffer(
    char* destBuffer, SerializableType _type,
    const char* buffer, uint32_t length) {

    size_t totalOffset = 0;

    uint16_t type = _type;
    size_t offset = sizeof(uint16_t);
    memcpy(destBuffer, &type, offset);
    totalOffset += offset;
    destBuffer += offset;

    offset = sizeof(uint32_t);
    memcpy(destBuffer, &length, offset);
    totalOffset += offset;
    destBuffer += offset;

    if (buffer) {
        memcpy(destBuffer, buffer, length);
        totalOffset += length;
    }

    return totalOffset;
}

sf::SerializableType sf::Serializable::deserializeType(BufferType& buffer) {
    SerializableType type = static_cast<SerializableType>(*((uint16_t*) buffer));
    buffer += sizeof(uint16_t);
    return type;
}

uint32_t sf::Serializable::deserializeSize(BufferType& buffer) {
    uint32_t size = *((uint32_t*) buffer);
    buffer += sizeof(uint32_t);
    return size;
}
