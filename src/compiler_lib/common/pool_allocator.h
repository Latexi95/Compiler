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

    block *last_block;
};

template <std::size_t BLOCK_SIZE>
void *pool_allocator<BLOCK_SIZE>::allocate(std::size_t s)
{
    assert(s <= block_size);
    if (!last_block || (block_size - last_block->used_size) < s) {
        new_block();
    }

    char *ret = last_block->data + last_block->used_size;
    last_block->used_size += s;
    return ret;
}

template <std::size_t BLOCK_SIZE>
void pool_allocator<BLOCK_SIZE>::deallocate(void *ptr, std::size_t s)
{
    if (last_block->data + last_block->used_size == ptr + s) {
        last_block->used_size -= s;
    }
}

template <std::size_t BLOCK_SIZE>
void pool_allocator<BLOCK_SIZE>::new_block()
{
    block *b = (block*)new char[sizeof(block) + block_size];
    b->before = last_block;
    b->used_size = 0;
    last_block = b;
}


