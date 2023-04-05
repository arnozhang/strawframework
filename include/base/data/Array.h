/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include <vector>

#include "base/data/Serializable.h"


namespace sf {

    class STRAW_FRAMEWORK_API Array : public Serializable {

    public:
        Array();
        Array(Array&& rhs);
        virtual ~Array();

        typedef std::vector<Serializable*> ArrayList;
        typedef ArrayList::iterator iterator;
        typedef ArrayList::const_iterator const_iterator;
        typedef ArrayList::value_type value_type;
        typedef ArrayList::reference reference;
        typedef ArrayList::const_reference const_reference;

        size_t size() const;
        bool empty() const;
        void clear();

        iterator begin();
        const_iterator begin() const;
        iterator end();
        const_iterator end() const;

        reference operator[](int pos);
        const_reference operator[](int pos) const;

        void push_back(Serializable* value);
        void push_back(const Serializable& value);

    public:
        virtual Serializable* clone() const override;
        virtual size_t byteSize() const override;
        virtual void serialize(char* buffer) const override;
        virtual size_t deserialize(const char* buffer) override;
        virtual SerializableType getType() const override;

    protected:
        ArrayList mList;
    };


    template <typename T>
    class STRAW_FRAMEWORK_API ArrayT : public Array {

    public:
        template <typename _Inner>
        class _Iter {
        public:
            _Iter(_Inner _iter) {
                iter = _iter;
            }

            _Iter& operator++() {
                ++iter;
                return *this;
            }

            _Iter operator++(int) {
                _Iter tmp;
                tmp.iter = iter;
                ++iter;
                return tmp;
            }

            _Iter& operator--() {
                --iter;
                return *this;
            }

            _Iter operator--(int) {
                _Iter tmp;
                tmp.iter = iter;
                --iter;
                return tmp;
            }

            T* operator->() const {
                return reinterpret_cast<T*>(iter.operator->());
            }

            bool operator!=(const _Iter& rhs) const {
                return iter != rhs.iter;
            }

        private:
            _Inner iter;
        };


        typedef _Iter<Array::iterator> iterator;
        typedef _Iter<Array::const_iterator> const_iterator;


        iterator begin() {
            return iterator(Array::begin());
        }

        const_iterator begin() const {
            return const_iterator(Array::begin());
        }

        iterator end() {
            return iterator(Array::end());
        }

        const_iterator end() const {
            return const_iterator(Array::end());
        }

        T* operator[](int pos) {
            return reinterpret_cast<T*>(Array::operator[](pos));
        }

        const T*& operator[](int pos) const {
            return reinterpret_cast<T*>(Array::operator[](pos));
        }
    };
}
