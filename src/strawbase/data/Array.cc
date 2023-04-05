#include <cstring>

#include "base/data/Array.h"

#include "base/StrawUtils.h"


sf::Array::Array() {
}

sf::Array::Array(Array&& rhs)
    : mList(std::move(rhs.mList)) {
}

sf::Array::~Array() {
    clear();
}

sf::Serializable* sf::Array::clone() const {
    Array* array = new Array();
    for (auto node : mList) {
        array->push_back(node->clone());
    }

    return array;
}

size_t sf::Array::byteSize() const {
    size_t size = getBaseSize() + sizeof(uint32_t);
    for (auto node : mList) {
        size += node->byteSize();
    }

    return size;
}

void sf::Array::serialize(char* buffer) const {
    size_t offset = serializeBuffer(
        buffer, getType(), nullptr, byteSize() - getBaseSize());
    buffer += offset;

    uint32_t count = mList.size();
    offset = sizeof(uint32_t);
    memcpy(buffer, &count, offset);
    buffer += offset;

    for (auto node : mList) {
        node->serialize(buffer);
        buffer += node->byteSize();
    }
}

size_t sf::Array::deserialize(const char* buffer) {
    mList.clear();

    if (!buffer) {
        return 0;
    }

    // | type | size | count | node1 | node2 | ... |
    //
    // node1:
    //      | type | size | value_buffer |
    //
    SerializableType type = deserializeType(buffer);
    uint32_t size = deserializeSize(buffer);

    if (type != getType()) {
        straw_warning("Deserialize Array FAILED! find not matched type!");
        return size;
    }

    uint32_t count = *((uint32_t*) buffer);
    buffer += sizeof(uint32_t);

    for (int i = 0; i < count; ++i) {
        auto prepare = buffer;
        SerializableType nodeType = deserializeType(prepare);
        uint16_t nodeSize = deserializeSize(prepare);

        Serializable* node = Serializable::createByType(nodeType);
        int resolved = 0;
        if (node) {
            resolved = node->deserialize(buffer);
            mList.push_back(node);
        } else {
            resolved = nodeSize;
        }

        buffer += resolved + getBaseSize();
    }

    return size;
}

sf::SerializableType sf::Array::getType() const {
    return SerializableType::t_Array;
}

size_t sf::Array::size() const {
    return mList.size();
}

bool sf::Array::empty() const {
    return mList.empty();
}

void sf::Array::clear() {
    for (auto node : mList) {
        delete node;
    }

    mList.clear();
}

sf::Array::iterator sf::Array::begin() {
    return mList.begin();
}

sf::Array::const_iterator sf::Array::begin() const {
    return mList.begin();
}

sf::Array::iterator sf::Array::end() {
    return mList.end();
}

sf::Array::const_iterator sf::Array::end() const {
    return mList.end();
}

sf::Array::reference sf::Array::operator[](int pos) {
    return mList[pos];
}

sf::Array::const_reference sf::Array::operator[](int pos) const {
    return mList[pos];
}

void sf::Array::push_back(Serializable* value) {
    mList.push_back(value);
}

void sf::Array::push_back(const Serializable& value) {
    mList.push_back(value.clone());
}
