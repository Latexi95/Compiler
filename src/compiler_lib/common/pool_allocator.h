#pragma once
#include <cinttypes>
#include <cassert>

template <std::size_t BLOCK_SIZE>
class pool_allocator
{
public:
    static const std::size_t block_size = BLOCK_SIZE;
    pool_allocator();
    ~pool_allocator();

    void *allocate(std::size_t s);
    void deallocate(void *ptr, std::size_t s);

private:
    void new_block();
    struct block {
        std::size_t used_size;
        block *before;
        char data[0];
    };

    block *_last_block;
};

template <std::size_t BLOCK_SIZE>
pool_allocator<BLOCK_SIZE>::pool_allocator() :
    _last_block(0)
{
}

template <std::size_t BLOCK_SIZE>
pool_allocator<BLOCK_SIZE>::~pool_allocator()
{
    block *b = _last_block;
    while (b) {
        block *before = b->before;
        delete[] (char*)b;
        b = before;
    }
}

template <std::size_t BLOCK_SIZE>
void *pool_allocator<BLOCK_SIZE>::allocate(std::size_t s)
{
    assert(s <= block_size);
    if (!_last_block || (block_size - _last_block->used_size) < s) {
        new_block();
    }

    char *ret = _last_block->data + _last_block->used_size;
    _last_block->used_size += s;
    return ret;
}

template <std::size_t BLOCK_SIZE>
void pool_allocator<BLOCK_SIZE>::deallocate(void *ptr, std::size_t s)
{
    if (_last_block->data + _last_block->used_size == ptr + s) {
        _last_block->used_size -= s;
    }
}

template <std::size_t BLOCK_SIZE>
void pool_allocator<BLOCK_SIZE>::new_block()
{
    block *b = (block*)new char[sizeof(block) + block_size];
    b->before = _last_block;
    b->used_size = 0;
    _last_block = b;
}


