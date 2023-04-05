#include "base/data/Buffer.h"


sf::Buffer::Buffer() {
    mSize = 0;
    mBuffer = nullptr;
    mReleaseWhenClear = true;
}

sf::Buffer::Buffer(char* buffer, size_t size) {
    mBuffer = buffer;
    mSize = size;
    mReleaseWhenClear = true;
}

sf::Buffer::~Buffer() {
    clear();
}

void sf::Buffer::assign(char* buffer, size_t size) {
    clear();
    mBuffer = buffer;
    mSize = size;
}

char* sf::Buffer::buffer() const {
    return mBuffer;
}

size_t sf::Buffer::size() const {
    return mBuffer ? mSize : 0;
}

bool sf::Buffer::empty() const {
    return size() <= 0;
}

void sf::Buffer::clear() {
    if (mReleaseWhenClear) {
        delete[] mBuffer;
    }

    mBuffer = nullptr;
    mSize = 0;
}

void sf::Buffer::setReleaseWhenClear(bool release) {
    mReleaseWhenClear = release;
}
