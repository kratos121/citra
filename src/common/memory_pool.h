/* This file is part of the dynarmic project.
 * Copyright (c) 2016 MerryMage
 * This software may be used and distributed according to the terms of the GNU
 * General Public License version 2 or any later version.
 */

#pragma once

#include <cstddef>
#include <vector>

namespace Dynarmic {
namespace Common {

class Pool {
public:
    /**
     * @param object_size Byte-size of objects to construct
     * @param initial_pool_size Number of objects to have per slab
     */
    Pool(size_t object_size, size_t initial_pool_size);
    ~Pool();

    Pool(Pool&) = delete;
    Pool(Pool&&) = delete;

    /// Returns a pointer to an `object_size`-bytes block of memory.
    void* Alloc();

private:
    // Allocates a completely new memory slab.
    // Used when an entirely new slab is needed
    // due the current one running out of usable space.
    void AllocateNewSlab();

    size_t object_size;
    size_t slab_size;
    char* current_slab;
    char* current_ptr;
    size_t remaining;
    std::vector<char*> slabs;
};

} // namespace Common
} // namespace Dynarmic
