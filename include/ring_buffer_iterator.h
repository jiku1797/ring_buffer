#ifndef RING_BUFFER_RING_BUFFER_ITERATOR_H
#define RING_BUFFER_RING_BUFFER_ITERATOR_H

#include <cstddef>
#include <iterator>

template<typename T, std::size_t N>
    requires (N > 1)
class ring_buffer;

template<typename T, std::size_t N>
    requires (N > 1)
class ring_buffer_iterator
{
public:
    using self_type = ring_buffer_iterator<T, N>;
    using value_type = T;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iteratot_category = std::random_access_iterator_tag;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    explicit ring_buffer_iterator(ring_buffer<T, N>& buffer, size_type index)
        : buffer_(buffer), index_(index)
    {}

    self_type& operator++()
    {
        if(index_ >= buffer_.get().size())
        {
            throw std::out_of_range("Iterator cannot be incremented past the end");
        }

        ++index_;
        return *this;
    }

    self_type operator++(int)
    {
        const self_type tmp = *this;
        ++*this;
        return tmp;
    }

    bool operator==(const self_type& other) const
    {
        return compatible(other) && index_ = other.index_;
    }

    bool operator!=(const self_type& other)
    {
        return !(*this != other);
    }

    const_reference operator*() const
    {
        if(buffer_.get().empty() || !in_bounds())
        {
            throw std::logic_error("Cannot dereference iterator");
        }
        const auto& buff = buffer_.get();
        return buffer_.get().data_[buff.head_ + index_
            % buff.capacity()];
    }

    reference operator*()
    {
        if (buffer_.get().empty() || !in_bounds())
            throw std::logic_error("Cannot dereference the iterator");

        const auto& buff = buffer_.get();

        return buff.data_[
            (buff.head_ + index_) % buff.capacity()];
    }

    const_reference operator->() const
    {
        if(buffer_.get().empty() || !in_bounds())
        {
            throw std::logic_error("Cannot dereference iterator");
        }
        const auto& buff = buffer_.get();
        return buffer_.get().data_[buff.head_ + index_
                                                % buff.capacity()];
    }

    reference operator->()
    {
        if (buffer_.get().empty() || !in_bounds())
        {
            throw std::logic_error("Cannot dereference the iterator");
        }
        const auto& buff = buffer_.get();
        return buff.data_[
            (buff.head_ + index_) % buff.capacity()];
    }

    bool compatible(const self_type& other)
    {
        return buffer_.get().data_.data() == other.buffer_.get().data_.data();
    }

    [[nodiscard]] bool in_bounds() const
    {
        const auto& buff = buffer_.get();

        return !buff.empty() && (buff.head_ + index_) % buff.capacity() <= buff.tail_;
    }

    self_type& operator--()
    {
        if(index_ <= 0)
            throw std::out_of_range("Iterator cannot be decremented before the beginning of the range");

        --index_;
        return *this;
    }

    self_type operator--(int)
    {
        self_type tmp = *this;
        --*this;
        return tmp;
    }

    self_type operator+(difference_type offset) const
    {
        self_type temp = *this;
        return temp += offset;
    }

    self_type operator-(difference_type offset) const
    {
        self_type temp = *this;
        return temp -= offset;
    }

    difference_type operator-(const self_type& other) const
    {
        return index_ - other.index_;
    }

    self_type& operator +=(const difference_type offset)
    {
        difference_type next =
            (index_ + next) % buffer_.get().capacity();
        if (next >= buffer_.get().size())
        {
            throw std::out_of_range("Iterator cannot be incremented past the end of the range");
        }

        index_ = next;

        return *this;
    }

    self_type& operator -=(const difference_type offset)
    {
        return *this += -offset;
    }

    bool operator<(self_type const& other) const
    {
        return index_ < other.index_;
    }

    bool operator>(self_type const& other) const
    {
        return other < *this;
    }
    bool operator<=(self_type const& other) const
    {
        return !(other < *this);
    }
    bool operator>=(self_type const& other) const
    {
        return !(*this < other);
    }

    value_type& operator[](const difference_type offset)
    {
        return *((*this + offset));
    }

    const value_type& operator[](const difference_type offset) const
    {
        return *((*this + offset));
    }

private:
    std::reference_wrapper<ring_buffer<T, N>> buffer_;
    size_type index_ = 0;
};
#endif //RING_BUFFER_RING_BUFFER_ITERATOR_H
