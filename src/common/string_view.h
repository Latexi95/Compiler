#pragma once
#include <string>
#include <cstring>
#include <algorithm>



template <typename CharT, typename Traits = std::char_traits<CharT> >
class basic_string_view {
public:
    typedef Traits traits_type;
    typedef CharT value_type;
    typedef size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef CharT &reference;
    typedef const CharT &const_reference;
    typedef CharT *pointer;
    typedef const CharT *const_pointer;
    typedef CharT *iterator;
    typedef const CharT *const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    basic_string_view() : _ptr(0), _size(0) {}
    basic_string_view(const basic_string_view &other) : _ptr(other._ptr), _size(other._size) {}
    basic_string_view(const CharT* s, size_type count) : _ptr(s), _size(count) {}
    basic_string_view(const CharT* s) : _ptr(s) { _size = std::strlen(s); }
    explicit basic_string_view(const std::basic_string<CharT, Traits> &s) : _ptr(s.data()), _size(s.size()) {}

    bool empty() const { return _size == 0; }

    const_pointer data() const { return _ptr; }

    size_type size() const { return _size; }
    size_type length() const { return _size; }


    void swap(basic_string_view &other) { std::swap(_ptr, other._ptr); std::swap(_size, other._size); }

    const_reference front() const { return *_ptr; }
    const_reference back() const { return _ptr[_size - 1]; }

    const_reference operator[](size_type pos) const { return _ptr[pos]; }
    const_reference at(size_type pos) const { return _ptr[pos]; }

    template <typename Allocator>
    operator std::basic_string<CharT, Traits, Allocator> () {
        return std::basic_string<CharT, Traits, Allocator>(_ptr, _size);
    }

    basic_string_view &operator = (const basic_string_view &s) {
        _ptr = s._ptr;
        _size = s._size;
        return *this;
    }

    bool operator == (const basic_string_view &s) const {
        if (_size != s._size) return false;
        return Traits::compare(_ptr, s._ptr, _size) == 0;
    }

    bool operator != (const basic_string_view &s) const {
        if (_size != s._size) return true;
        return Traits::compare(_ptr, s._ptr, _size) != 0;
    }

    int compare(const basic_string_view &s) const {
        size_type rlen = std::min(size(), s.size());
        int c = traits_type::compare(_ptr, s._ptr, rlen);
        if (c == 0) {
            return _size - s._size;
        }
        return c;
    }

    bool operator < (const basic_string_view &s) const { return compare(s) < 0; }
    bool operator > (const basic_string_view &s) const { return compare(s) > 0; }
    bool operator <= (const basic_string_view &s) const { return compare(s) <= 0; }
    bool operator >= (const basic_string_view &s) const { return compare(s) >= 0; }

    const_iterator begin() const { return _ptr; }
    const_iterator cbegin() const { return _ptr; }
    const_iterator end() const { return _ptr + _size; }
    const_iterator cend() const { return _ptr + _size; }
    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
    const_reverse_iterator crbegin() const { return const_reverse_iterator(end()); }
    const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }
    const_reverse_iterator crend() const { return const_reverse_iterator(begin()); }

private:
    const CharT *_ptr;
    std::size_t _size;
};

typedef basic_string_view<char> string_view;
typedef basic_string_view<wchar_t> wstring_view;

namespace std {
    template <typename CharT, typename Traits>
    class hash<basic_string_view<CharT, Traits> > {
    public:
        typedef basic_string_view<CharT, Traits> argument_type;
        typedef std::size_t result_type;
        result_type operator()(const basic_string_view<CharT, Traits> &s) const {
            result_type h = (sizeof(result_type) == 4) ? 2166136261ull : 14695981039346656037ull;
            result_type FNV_prime = (sizeof(result_type) == 4) ? 16777619ull : 1099511628211ull;
            for (CharT c : s) {
                for (unsigned i = 0; i < sizeof(CharT); ++i) {
                    h ^= (c & (0xFF << (i * 8))) >> (i * 8);
                    h *= FNV_prime;
                }
            }
            return h;
        }
    };
}


inline string_view operator "" _sv(const char *t, size_t s) {
    return string_view(t, s);
}



