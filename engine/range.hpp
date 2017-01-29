#pragma once

#include <type_traits>

template <typename T>
class IntegralRange
{
    static_assert(std::is_integral<T>::value, "");
    
    class Iterator
    {
    public:
        Iterator(const Iterator&) = default;
        ~Iterator() = default;
    
        inline Iterator
        (
            T value
        ) :
            m_value(value)
        { }
        
        Iterator& operator=(const Iterator&) = default;
        
        inline bool operator==
        (
            const Iterator& rhs
        )
        {
            return m_value == rhs.m_value;
        }
        
        inline bool operator!=
        (
            const Iterator& rhs
        )
        {
            return m_value != rhs.m_value;
        }
        
        inline T operator*() const
        {
            return m_value;
        }
        
        inline T operator->() const
        {
            return m_value;
        }
        
        inline Iterator& operator++()
        {
            ++m_value;
            return *this;
        }
        
        inline Iterator operator++(int)
        {
            Iterator result { m_value };
            ++m_value;
            return result;
        }
    
    private:
        T m_value;
    };

public:
    inline IntegralRange
    (
        const T& begin,
        const T& end
    ) :
        m_begin(begin),
        m_end(end)
    {
    }
    
    inline Iterator begin()
    {
        return { m_begin };
    }
    
    inline Iterator end()
    {
        return { m_end };
    }

private:
    T m_begin;
    T m_end;
};

template <typename T>
inline IntegralRange<T> IRange
(
    T end
)
{
    return { T(0), end };
}

template <typename T>
inline IntegralRange<T> IRange
(
    T begin,
    T end
)
{
    return { begin, end };
}

template <typename T, typename I = std::size_t>
class IndexedRange
{
public:
    class Iterator;

    class IndexedValue
    {
        friend class Iterator;
        
    public:
        inline IndexedValue
        (
            T iterator,
            I index
        ) :
            m_iterator(iterator),
            m_index(index)
        { }
    
        inline I index() const
        {
            return m_index;
        }
        
        inline auto& value() const
        {
            return *m_iterator;
        }
        
        inline auto operator->() const
        {
            return m_iterator;
        }
        
    private:
        T m_iterator;
        I m_index;
    };
    
    class Iterator
    {
    public:
        Iterator(const Iterator&) = default;
        ~Iterator() = default;
    
        inline Iterator
        (
            T iterator,
            I index = I(0)
        ) :
            m_value(iterator, index)
        { }
        
        Iterator& operator=(const Iterator&) = default;
        
        inline bool operator==
        (
            const Iterator& rhs
        )
        {
            return m_value.m_iterator == rhs.m_value.m_iterator;
        }
        
        inline bool operator!=
        (
            const Iterator& rhs
        )
        {
            return m_value.m_iterator != rhs.m_value.m_iterator;
        }
        
        inline const IndexedValue& operator*() const
        {
            return m_value;
        }
        
        inline IndexedValue* operator->() const
        {
            return &m_value;
        }
        
        inline Iterator& operator++()
        {
            ++m_value.m_iterator;
            ++m_value.m_index;
            return *this;
        }
        
        inline Iterator operator++(int)
        {
            Iterator result { m_value.m_iterator, m_value.m_index };
            ++m_value.m_iterator;
            ++m_value.m_index;
            return result;
        }
    
    private:
        IndexedValue m_value;
    };
    
public:
    inline IndexedRange
    (
        const T& begin,
        const T& end
    ) :
        m_begin(begin),
        m_end(end)
    {
    }
    
    inline Iterator begin()
    {
        return { m_begin };
    }
    
    inline Iterator end()
    {
        return { m_end };
    }

private:
    T m_begin;
    T m_end;
};

template <typename T>
IndexedRange<T> IndexRange(
    T begin,
    T end
)
{
    return { begin, end };
}

template <typename I = std::size_t, typename C, typename T = decltype(std::declval<C>().begin())>
IndexedRange<T, I> IndexRange(
    C& container
)
{
    return { container.begin(), container.end() };
}
