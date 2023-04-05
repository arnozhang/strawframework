/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

#include <mutex>


namespace sf {

    template<typename T>
    class PointTemplate {

    public:
        PointTemplate(T _x = 0, T _y = 0) {
            x = _x;
            y = _y;
        }

        PointTemplate& set(T _x, T _y) {
            x = _x;
            y = _y;
            return *this;
        }

        PointTemplate& reset() {
            set(0, 0);
            return *this;
        }

        PointTemplate& operator+= (const PointTemplate& rhs) {
            x += rhs.x;
            y += rhs.y;
            return *this;
        }

        PointTemplate& operator+= (T v) {
            x += v;
            y += v;
            return *this;
        }

        bool operator== (const PointTemplate& rhs) const {
            return x == rhs.x && y == rhs.y;
        }

        bool operator!= (const PointTemplate& rhs) const {
            return !(*this == rhs);
        }

    public:
        T x;
        T y;
    };


    template<typename T>
    class SizeTemplate {

    public:
        SizeTemplate(T _width = 0, T _height = 0) {
            width = _width;
            height = _height;
        }

        SizeTemplate& set(T _width, T _height) {
            width = _width;
            height = _height;
            return *this;
        }

        SizeTemplate& reset() {
            set(0, 0);
            return *this;
        }

        SizeTemplate& operator+= (const SizeTemplate& rhs) {
            width += rhs.width;
            height += rhs.height;
            return *this;
        }

        SizeTemplate& operator+= (T v) {
            width += v;
            height += v;
            return *this;
        }

        bool operator== (const SizeTemplate& rhs) const {
            return width == rhs.width && height == rhs.height;
        }

        bool operator!= (const SizeTemplate& rhs) const {
            return !(*this == rhs);
        }

    public:
        T width;
        T height;
    };


    template<typename T>
    class Directionx4Template {

    public:
        Directionx4Template(T _left = 0, T _top = 0, T _right = 0, T _bottom = 0)
            : left(_left), top(_top), right(_right), bottom(_bottom) {}

        T horzDistance() const {
            return right - left;
        }

        T vertDistance() const {
            return bottom - top;
        }

        T horzCenter() const {
            return horzDistance() / 2 + left;
        }

        T vertCenter() const {
            return vertDistance() / 2 + top;
        }

        T horzSum() const {
            return left + right;
        }

        T vertSum() const {
            return top + bottom;
        }

        bool empty() const {
            return left >= right || top >= bottom;
        }

        template<typename P>
        bool contains(PointTemplate<P>& pt) {
            return contains(pt.x, pt.y);
        }

        bool contains(T x, T y) const {
            return x >= left && x <= right && y >= top && y <= bottom;
        }

        bool isIntersect(Directionx4Template& rhs) const {
            return !empty() && !rhs.empty()
                && left < rhs.right && rhs.left < right
                && top < rhs.bottom && rhs.top < bottom;
        }

        Directionx4Template& merge(Directionx4Template& rhs) {
            if (left > rhs.left) {
                left = rhs.left;
            }

            if (top > rhs.top) {
                top = rhs.top;
            }

            if (right < rhs.right) {
                right = rhs.right;
            }

            if (bottom < rhs.bottom) {
                bottom = rhs.bottom;
            }

            return *this;
        }

        Directionx4Template& set(T _left, T _top, T _right, T _bottom) {
            left = _left;
            top = _top;
            right = _right;
            bottom = _bottom;
            return *this;
        }

        Directionx4Template& reset() {
            set(0, 0, 0, 0);
            return *this;
        }

        Directionx4Template& operator+= (const Directionx4Template& rhs) {
            left += rhs.left;
            top += rhs.top;
            right += rhs.right;
            bottom += rhs.bottom;
            return *this;
        }

        Directionx4Template& operator+= (T v) {
            left += v;
            top += v;
            right += v;
            bottom += v;
            return *this;
        }

        template<typename P>
        Directionx4Template& move(PointTemplate<P> pt) {
            return move(pt.x, pt.y);
        }

        Directionx4Template& move(T x, T y) {
            left += x;
            right += x;
            top += y;
            bottom += y;
            return *this;
        }

        bool operator== (const Directionx4Template& rhs) const {
            return left == rhs.left
                && right == rhs.right
                && top == rhs.top
                && bottom == rhs.bottom;
        }

        bool operator!= (const Directionx4Template& rhs) const {
            return !(*this == rhs);
        }

    public:
        T left;
        T top;
        T right;
        T bottom;
    };


    inline bool checkFlag(int flag, int checked) {
        return (flag & checked) == checked;
    }


    using MutexLock = std::unique_lock<std::mutex>;

}
