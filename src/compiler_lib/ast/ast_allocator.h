#pragma once
#include <memory>
#include "../common/pool_allocator.h"
namespace ast {
typedef pool_allocator<4096> allocator;
}

inline void * operator new (std::size_t size, ast::allocator & allocator)
{
    return allocator.allocate(size);
}

inline void * operator new[] (std::size_t size, ast::allocator & allocator)
{
    return allocator.allocate(size);
}
