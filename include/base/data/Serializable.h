/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include "base/def/PlatformDefine.h"


namespace sf {

    class STRAW_FRAMEWORK_API Buffer;


    enum SerializableType : uint16_t {
        t_Unknown = 0x00,
        t_Char,
        t_Boolean,
        t_Short,
        t_Integer,
        t_Long,
        t_UInt64,
        t_Float,
        t_Double,
        t_String,
        t_Array,
        t_Data,
        t_BinaryBuffer,

        t_CustomizeData = 1000,
    };


    class STRAW_FRAMEWORK_API Serializable {

    public:
        Serializable() {}
        virtual ~Serializable() {}

        virtual Serializable* clone() const = 0;
        virtual size_t byteSize() const = 0;
        virtual void serialize(char* buffer) const = 0;
        virtual size_t deserialize(const char* buffer) = 0;
        virtual SerializableType getType() const = 0;

        Buffer* serialize() const;
        size_t deserialize(const Buffer* buffer);
        size_t deserialize(const Buffer& buffer);

        using BufferType = const char*;
        SerializableType deserializeType(BufferType& buffer);
        uint32_t deserializeSize(BufferType& buffer);

    public:
        using SerializableCreator = Serializable* (*)(SerializableType type);
        static void registerSerializableCreator(
            uint16_t type, SerializableCreator creator);

        static Serializable* createByType(SerializableType type);
        static size_t serializeBuffer(
            char* destBuffer, SerializableType _type,
            const char* buffer, uint32_t length);

        static constexpr int getBaseSize() {
            return sizeof(uint16_t) + sizeof(uint32_t);
        }
    };

}
