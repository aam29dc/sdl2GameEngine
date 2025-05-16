#pragma once

#include <SDL_rect.h>

template<typename T>
struct Array {
    T x, y;

    inline T& operator[](const unsigned index) {
        return index == 0 ? x : y;
    }

    inline const T& operator[](const unsigned index) const {
        return index == 0 ? x : y;
    }
};

using Float2 = Array<float>;
using Int2 = Array<int>;
using UInt2 = Array<unsigned int>;

template<typename T>
inline bool operator==(const Array<T>& a, const Array<T>& b) {
    return a.x == b.x && a.y == b.y;
}

template<typename T>
inline bool operator!=(const Array<T>& a, const Array<T>& b) {
    return !(a == b);
}

template<typename T>
inline Array<T> operator+(const Array<T>& a, const Array<T>& b) {
    return { a.x + b.x, a.y + b.y };
}

template<typename T>
inline Array<T> operator-(const Array<T>& a) {
    return { -a.x, -a.y };
}

template<typename T>
inline Array<T> operator-(const Array<T>& a, const Array<T>& b) {
    return { a.x - b.x, a.y - b.y };
}

template<typename T>
inline Array<T> operator*(const Array<T>& a, const Array<T>& b) {
    return { a.x * b.x, a.y * b.y };
}

template<typename T>
inline Array<T> operator*(const Array<T>& a, const T scalar) {
    return { a.x * scalar, a.y * scalar };
}

template<typename T>
inline Array<T> operator*(const T scalar, const Array<T>& a) {
    return a * scalar;
}

template<typename T>
inline Array<T> operator/(const Array<T>& a, const Array<T>& b) {
    return { a.x / b.x, a.y / b.y };
}

template<typename T>
inline Array<T> operator/(const Array<T>& a, const T scalar) {
    return { a.x / scalar, a.y / scalar };
}

template <typename T>
inline SDL_FRect arrayToFRect(const Array<T>& pos, const Array<T>& size) {
    return SDL_FRect{ static_cast<float>(pos.x), static_cast<float>(pos.y), size.x, size.y };
}