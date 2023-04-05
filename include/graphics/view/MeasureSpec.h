/*
 * Copyright (C) 2016. The Straw Framework Project.
 *
 * @author  Arno Zhang
 * @date    2016/03/18
 */

#pragma once

namespace sf {

    enum MeasureMode {
        Unspecified,
        Exactly,
        AtMost,
    };


    struct MeasureSpec {
        MeasureMode mode;
        int dimension;

        MeasureSpec() {
            unspecified();
        }

        MeasureSpec(MeasureMode _mode, int _dimension) {
            set(_mode, _dimension);
        }

        bool operator == (const MeasureSpec& rhs) const {
            return mode == rhs.mode && dimension == rhs.dimension;
        }

        bool operator != (const MeasureSpec& rhs) const {
            return !(*this == rhs);
        }

        static MeasureSpec make(MeasureMode _mode, int _dimension) {
            return MeasureSpec(_mode, _dimension);
        }

        static MeasureSpec makeExactly(int _dimension) {
            return MeasureSpec(MeasureMode::Exactly, _dimension);
        }

        static MeasureSpec makeAtMost(int _dimension) {
            return MeasureSpec(MeasureMode::AtMost, _dimension);
        }

        bool isUnspecified() const {
            return mode == MeasureMode::Unspecified;
        }

        bool isExactly() const {
            return mode == MeasureMode::Exactly;
        }

        bool isAtMost() const {
            return mode == AtMost;
        }

        MeasureSpec& set(MeasureMode _mode, int _dimension) {
            mode = _mode;
            dimension = _dimension;
            return *this;
        }

        MeasureSpec& unspecified() {
            mode = MeasureMode::Unspecified;
            dimension = 0;
            return *this;
        }

        MeasureSpec& exactly(int _dimension) {
            mode = MeasureMode::Exactly;
            dimension = _dimension;
            return *this;
        }

        MeasureSpec& atMost(int _dimension) {
            mode = MeasureMode::AtMost;
            dimension = _dimension;
            return *this;
        }
    };

}
