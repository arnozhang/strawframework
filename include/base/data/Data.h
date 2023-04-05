/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include <map>

#include "base/data/Serializable.h"


namespace sf {

    class STRAW_FRAMEWORK_API Data : public Serializable {

    public:
        Data();
        Data(Data&& rhs);
        virtual ~Data();

        typedef std::map<std::string, Serializable*> DataMap;
        typedef DataMap::iterator iterator;
        typedef DataMap::const_iterator const_iterator;
        typedef DataMap::reference reference;
        typedef DataMap::const_reference const_reference;
        typedef DataMap::value_type::second_type value_type;

        size_t size() const;
        bool empty() const;
        void clear();

        iterator begin();
        const_iterator begin() const;
        iterator end();
        const_iterator end() const;

        void put(const std::string& key, Serializable* value);
        void put(const std::string& key, const Serializable& value);

        value_type& operator[](const std::string& key);
        bool has(const std::string& key);
        iterator find(const std::string& key);
        const_iterator find(const std::string& key) const;

    public:
        virtual Serializable* clone() const override;
        virtual size_t byteSize() const override;
        virtual void serialize(char* buffer) const override;
        virtual size_t deserialize(const char* buffer) override;
        virtual SerializableType getType() const override;

    protected:
        DataMap mMap;
    };

}
