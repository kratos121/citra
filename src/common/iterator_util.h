/* This file is part of the dynarmic project.
 * Copyright (c) 2016 MerryMage
 * This software may be used and distributed according to the terms of the GNU
 * General Public License version 2 or any later version.
 */

#pragma once

#include <iterator>

namespace Dynarmic {
namespace Common {

namespace detail {

template<typename T>
struct ReverseAdapter {
    T& iterable;

    auto begin() {
        using namespace std;
        return rbegin(iterable);
    }

    auto end() {
        using namespace std;
        return rend(iterable);
    }
};

} // namespace detail

template<typename T>
detail::ReverseAdapter<T> Reverse(T&& iterable) {
    return detail::ReverseAdapter<T>{iterable};
}

} // namespace Common
} // namespace Dynarmic
