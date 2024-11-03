#pragma once
#include <stddef.h>

template <typename T, size_t N>
constexpr size_t arraySize(const T (&)[N]) noexcept {
    return N;
}