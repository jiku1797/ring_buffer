#ifndef RING_BUFFER_RING_BUFFER_H
#define RING_BUFFER_RING_BUFFER_H

#include <cstddef>
#include <array>
#include <algorithm>

#include "ring_buffer_iterator.h"

namespace sr = std::ranges;

/*template<typename T, std::size_t N>
    requires (N > 1)
class ring_buffer_iterator;*/

template<typename T, std::size_t N>
    requires(N > 1)
class ring_buffer
{
public:
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = ring_buffer_iterator<T, N>;
    using const_iterator = ring_buffer_iterator<const T, N>;

    constexpr ring_buffer() = default;
    explicit constexpr ring_buffer(value_type (&values)[N])
        : size_(N), tail_(N-1)
    {
        std::copy(sr::copy(values, data_));
    }

    explicit constexpr ring_buffer(const_reference value)
        : size_(N), tail_(N-1)
    {
        sr::fill(data_, value);
    }

    [[nodiscard]] constexpr size_type size() const
    {
        return size_;
    }

    [[nodiscard]] constexpr size_type capacity() const
    {
        return N;
    }

    [[nodiscard]] constexpr bool empty() const
    {
        return size_ == 0;
    }

    [[nodiscard]] constexpr bool full() const
    {
        return size_ == N;
    }

    constexpr void clear()
    {
        size_ = 0;
        head_ = 0;
        tail_ = 0;
    }

    constexpr reference operator[](size_type pos)
    {
        return data_[(head_ + pos) % N];
    }

    constexpr const_reference operator[](size_type pos) const
    {
        return operator[](pos);
    }

    constexpr reference at(size_type pos)
    {
        if(pos >= N){throw std::out_of_range("Invalid index");}

        return operator[](pos);
    }

    constexpr const_reference at(size_type pos) const
    {
        if(pos >= N){throw std::out_of_range("Invalid index");}

        return operator[](pos);
    }

    constexpr reference front()
    {
        if(size_ == 0)
        {
            throw std::logic_error("Empty buffer");
        }

        return data_[head_];
    }

    constexpr const_reference front() const
    {
        if(size_ == 0)
        {
            throw std::logic_error("Empty buffer");
        }

        return data_[head_];
    }

    constexpr reference back()
    {
        if(size_ == 0)
        {
            throw std::logic_error("Empty buffer");
        }

        return data_[tail_];
    }

    constexpr const_reference back() const
    {
        if(size_ == 0)
        {
            throw std::logic_error("Empty buffer");
        }

        return data_[tail_];
    }

    constexpr void push_back(const T& value)
    {
        if(empty())
        {
            data_[tail_] = value;
            ++size_;
        }
        else if(!full())
        {
            data_[++tail_] = value;
            ++size_;
        }
        else
        {
            head_ = (head_ + 1) % N;
            tail_ = (tail_ + 1) % N;
            data_[tail_] = value;
        }
    }

    constexpr void push_back(T&& value)
    {
        if(empty())
        {
            data_[tail_] = std::move(value);
            ++size_;
        }
        else if(!full())
        {
            data_[++tail_] = std::move(value);
            ++size_;
        }
        else
        {
            head_ = (head_ + 1) % N;
            tail_ = (tail_ + 1) % N;
            data_[tail_] = std::move(value);
        }
    }

    constexpr T pop_front()
    {
        if(empty())
        {
            throw std::logic_error("Empty buffer");
        }

        --size_;

        return data_[std::exchange(head_, (head_ + 1) % N )];
    }

    iterator begin()
    {
        return iterator(*this, 0);
    }

    iterator end()
    {
        return iterator(*this, size_);
    }

    const_iterator begin() const
    {
        return const_iterator(*this, 0);
    }

    const_iterator end() const
    {
        return const_iterator(*this, size_);
    }

private:
    friend ring_buffer_iterator<T, N>;

    std::array<T, N> data_{};
    size_type head_ = 0;
    size_type tail_ = 0;
    size_type size_ = 0;
};

#endif //RING_BUFFER_RING_BUFFER_H
