/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include <vector>

#include "base/data/Buffer.h"
#include "base/data/Serializable.h"


namespace sf {

    template<class T, SerializableType type>
    class NumericSerializable : public Serializable {

    public:
        NumericSerializable() {
            mValue = T();
        }

        NumericSerializable(T value) {
            mValue = value;
        }

        NumericSerializable(const NumericSerializable& rhs) {
            mValue = rhs.mValue;
        }

        virtual ~NumericSerializable() {
        }

        virtual Serializable* clone() const override {
            return new NumericSerializable(*this);
        }

        virtual size_t byteSize() const override {
            return getBaseSize() + sizeof(T);
        }

        virtual void serialize(char* buffer) const override {
            serializeBuffer(buffer, getType(),
                reinterpret_cast<const char*>(&mValue), sizeof(T));
        }

        virtual size_t deserialize(const char* buffer) override {
            mValue = T();

            if (!buffer) {
                return 0;
            }

            SerializableType deType = deserializeType(buffer);
            uint32_t size = deserializeSize(buffer);

            if (deType != getType() || size < sizeof(T)) {
                straw_warning("Deserialize FAILED! find not matched type!");
                return size;
            }

            mValue = (*(T*) buffer);
            return size;
        }

        virtual SerializableType getType() const override {
            return type;
        }

        operator T&() {
            return mValue;
        }

        T& value() {
            return mValue;
        }

        const T& value() const {
            return mValue;
        }

        NumericSerializable& operator= (const T value) {
            mValue = value;
            return *this;
        }

        NumericSerializable& operator= (const NumericSerializable& rhs) {
            mValue = rhs.mValue;
            return *this;
        }

    private:
        T mValue;
    };

    template<>
    class NumericSerializable<const char*, SerializableType::t_BinaryBuffer> : public Serializable {

    public:
        NumericSerializable() {
            mValue = nullptr;
            mLength = 0;
        }

        NumericSerializable(const char* value, uint32_t length) {
            mValue = value;
            mLength = length;
        }

        NumericSerializable(NumericSerializable& rhs) {
            *this = rhs;
        }

        virtual ~NumericSerializable() {
            mLength = 0;
            delete mValue;
        }

        virtual Serializable* clone() const override {
            char* buffer = nullptr;
            size_t length = mLength;

            if (mValue && mLength > 0) {
                buffer = new char[length];
                memcpy(buffer, mValue, length);
            }

            return new NumericSerializable(buffer, length);
        }

        virtual size_t byteSize() const override {
            return getBaseSize() + mLength;
        }

        virtual void serialize(char* buffer) const override {
            serializeBuffer(buffer, getType(), mValue, mLength);
        }

        virtual size_t deserialize(const char* buffer) override {
            delete[] mValue;
            mValue = nullptr;
            mLength = 0;

            if (!buffer) {
                return 0;
            }

            SerializableType type = deserializeType(buffer);
            uint32_t size = deserializeSize(buffer);

            if (type != getType()) {
                straw_warning("Deserialize FAILED! find not matched type!");
                return size;
            }

            char* newBuffer = new char[size];
            memcpy(newBuffer, buffer, size);

            mValue = newBuffer;
            mLength = size;

            return size;
        }

        virtual SerializableType getType() const override {
            return SerializableType::t_BinaryBuffer;
        }

        operator const char*() const {
            return mValue;
        }

        const char* value() const {
            return mValue;
        }

        int size() const {
            return mLength;
        }


        NumericSerializable& operator= (NumericSerializable& rhs) {
            mValue = rhs.mValue;
            mLength = rhs.mLength;

            rhs.mValue = nullptr;
            rhs.mLength = 0;
            return *this;
        }

    private:
        NumericSerializable& operator= (const char* value) = delete;

        const char* mValue;
        uint32_t mLength;
    };

    template<>
    class NumericSerializable<std::string, SerializableType::t_String> : public Serializable {

    public:
        NumericSerializable() {
        }

        NumericSerializable(const std::string& value) {
            mValue = value;
        }

        NumericSerializable(std::string&& value) : mValue(value) {
        }

        NumericSerializable(const char* value) {
            if (value) {
                mValue = value;
            }
        }

        NumericSerializable(const NumericSerializable& rhs) {
            mValue = rhs.mValue;
        }

        NumericSerializable(NumericSerializable&& rhs)
            : mValue(std::move(rhs.mValue)) {
        }

        virtual ~NumericSerializable() {
        }

        virtual Serializable* clone() const override {
            return new NumericSerializable(*this);
        }

        virtual size_t byteSize() const override {
            return getBaseSize() + mValue.size();
        }

        virtual void serialize(char* buffer) const override {
            serializeBuffer(buffer, getType(), mValue.c_str(), mValue.size());
        }

        virtual size_t deserialize(const char* buffer) override {
            mValue.clear();

            if (!buffer) {
                return 0;
            }

            SerializableType type = deserializeType(buffer);
            uint32_t size = deserializeSize(buffer);

            if (type != getType()) {
                straw_warning("Deserialize FAILED! find not matched type!");
                return size;
            }

            mValue.assign(buffer, size);

            return size;
        }

        virtual SerializableType getType() const override {
            return SerializableType::t_String;
        }

        bool empty() const {
            return mValue.empty();
        }

        void clear() {
            mValue.clear();
        }

        operator std::string&() {
            return mValue;
        }

        std::string& value() {
            return mValue;
        }

        const std::string& value() const {
            return mValue;
        }

        NumericSerializable& operator= (const char* value) {
            mValue.clear();
            if (value) {
                mValue = value;
            }

            return *this;
        }

        NumericSerializable& operator= (const std::string& value) {
            mValue = value;
            return *this;
        }

        NumericSerializable& operator= (const NumericSerializable& rhs) {
            mValue = rhs.mValue;
            return *this;
        }

    private:
        std::string mValue;
    };


    typedef class STRAW_TEMPLATE_API
        NumericSerializable<char, SerializableType::t_Char> Char;

    typedef class STRAW_TEMPLATE_API
        NumericSerializable<bool, SerializableType::t_Boolean> Boolean;

    typedef class STRAW_TEMPLATE_API
        NumericSerializable<int, SerializableType::t_Short> Short;

    typedef class STRAW_TEMPLATE_API
        NumericSerializable<int, SerializableType::t_Integer> Integer;

    typedef class STRAW_TEMPLATE_API
        NumericSerializable<long, SerializableType::t_Long> Long;

    typedef class STRAW_TEMPLATE_API
        NumericSerializable<uint16_t, SerializableType::t_UInt64> UInt64;

    typedef class STRAW_TEMPLATE_API
        NumericSerializable<float, SerializableType::t_Float> Float;

    typedef class STRAW_TEMPLATE_API
        NumericSerializable<double, SerializableType::t_Double> Double;

    typedef class STRAW_TEMPLATE_API
        NumericSerializable<std::string, SerializableType::t_String> String;

    typedef class STRAW_TEMPLATE_API
        NumericSerializable<const char*, SerializableType::t_BinaryBuffer> BinaryBuffer;

}
