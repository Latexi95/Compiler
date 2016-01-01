#pragma once
#include <utility>

template <typename T>
class object_construct_builder {
public:
    object_construct_builder(T *p) : _ptr(p) {}
    template <typename... P>
    T *operator ()(P&&... params) {
        new (_ptr)T(std::forward(params)...);
        return _ptr;
    }

private:
    T *_ptr;
};
