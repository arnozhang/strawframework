#include "base/data/Data.h"

#include "base/data/Buffer.h"


sf::Data::Data() {
}

sf::Data::Data(Data&& rhs)
    : mMap(std::move(rhs.mMap)) {
}

sf::Data::~Data() {
    clear();
}

sf::Serializable* sf::Data::clone() const {
    Data* data = new Data();
    for (auto& node : mMap) {
        data->put(node.first, node.second->clone());
    }

    return data;
}

size_t sf::Data::byteSize() const {
    size_t size = getBaseSize() + sizeof(uint32_t);
    for (auto& node : mMap) {
        size += 4 + node.first.size() + node.second->byteSize();
    }

    return size;
}

void sf::Data::serialize(char* buffer) const {
    size_t offset = serializeBuffer(
        buffer, getType(), nullptr, byteSize() - getBaseSize());
    buffer += offset;

    uint32_t count = static_cast<uint32_t>(mMap.size());
    offset = sizeof(uint32_t);
    memcpy(buffer, &count, offset);
    buffer += offset;

    for (auto& node : mMap) {
        uint32_t keyLength = node.first.size();
        offset = sizeof(uint32_t);
        memcpy(buffer, &keyLength, offset);
        buffer += offset;

        memcpy(buffer, node.first.c_str(), keyLength);
        buffer += keyLength;

        node.second->serialize(buffer);
        buffer += node.second->byteSize();
    }
}

size_t sf::Data::deserialize(const char* buffer) {
    mMap.clear();

    if (!buffer) {
        return 0;
    }

    // | type | size | count | node1 | node2 | ... |
    //
    // node1:
    //      | key-length | key | type | size | value_buffer |
    //
    SerializableType type = deserializeType(buffer);
    uint32_t size = deserializeSize(buffer);

    if (type != getType()) {
        straw_warning("Deserialize Data FAILED! find not matched type!");
        return size;
    }

    uint32_t count = *((uint32_t*) buffer);
    buffer += sizeof(uint32_t);

    for (int i = 0; i < count; ++i) {
        uint32_t keyLength = *((uint32_t*) buffer);
        buffer += sizeof(uint32_t);

        char* keyBuffer = new char[keyLength];
        memcpy(keyBuffer, buffer, keyLength);
        std::string key(keyBuffer, keyLength);
        buffer += keyLength;
        delete[] keyBuffer;

        auto prepare = buffer;
        SerializableType nodeType = deserializeType(prepare);
        uint16_t nodeSize = deserializeSize(prepare);

        Serializable* node = Serializable::createByType(nodeType);
        int resolved = 0;
        if (node) {
            resolved = node->deserialize(buffer);
            mMap[key] = node;
        } else {
            resolved = nodeSize;
        }

        buffer += resolved + getBaseSize();
    }

    return size;
}

sf::SerializableType sf::Data::getType() const {
    return SerializableType::t_Data;
}

size_t sf::Data::size() const {
    return mMap.size();
}

bool sf::Data::empty() const {
    return mMap.empty();
}

void sf::Data::clear() {
    for (auto& node : mMap) {
        delete node.second;
    }

    mMap.clear();
}

sf::Data::iterator sf::Data::begin() {
    return mMap.begin();
}

sf::Data::const_iterator sf::Data::begin() const {
    return mMap.begin();
}

sf::Data::iterator sf::Data::end() {
    return mMap.end();
}

sf::Data::const_iterator sf::Data::end() const {
    return mMap.end();
}

void sf::Data::put(const std::string& key, Serializable* value) {
    Serializable* old = mMap[key];
    if (old) {
        delete old;
    }

    mMap[key] = value;
}

void sf::Data::put(const std::string& key, const Serializable& value) {
    put(key, value.clone());
}

sf::Data::value_type& sf::Data::operator[](const std::string& key) {
    return mMap[key];
}

bool sf::Data::has(const std::string& key) {
    return mMap.find(key) != mMap.end();
}

sf::Data::iterator sf::Data::find(const std::string& key) {
    return mMap.find(key);
}

sf::Data::const_iterator sf::Data::find(const std::string& key) const {
    return mMap.find(key);
}
