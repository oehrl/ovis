#pragma once

#include <type_traits>
#include <vector>
#include <SDL_assert.h>
#include <range.hpp>

template <typename T, typename V>
class Spline
{
    static_assert(std::is_floating_point<T>::value, "T must be a floating point");
    
public:
    template <typename IteratorType1, typename IteratorType2>
    inline Spline(IteratorType1 t_begin, IteratorType1 t_end, IteratorType2 v_begin) :
        m_t(t_begin, t_end),
        m_v(v_begin, std::next(v_begin, m_t.size()))
    {
        for (const auto i : IRange(m_t.size() - 1))
        {
            SDL_assert(m_t[i] < m_t[i + 1]);
        }
        SDL_assert(m_t.size() == m_v.size());
        SDL_assert(m_t.size() > 0);
        CalculateK();
    }
    
    inline V Sample(T t) const
    {
        static const auto q = [](const T t, const V& y1, const V& y2, const V& a, const V& b)
        {
            const T one_minus_t = T(1) - t;
            return
                one_minus_t * y1 +
                t * y2 +
                t * one_minus_t * (a * one_minus_t + b * t);
        };
        
        if (t < m_t.front())
        {
            return m_v.front() + m_k.front() * (t - m_t.front());
        }
        else if (t > m_t.back())
        {
            return m_v.back() + m_k.back() * (t - m_t.back());
        }
        else
        {
            const auto i = FindInterval(t);
            SDL_assert(i < m_t.size() - 1);
            SDL_assert(i < m_v.size() - 1);
            
            const T t1 = m_t[i];
            const T t2 = m_t[i + 1];
            const V v1 = m_v[i];
            const V v2 = m_v[i + 1];
            const V k1 = m_k[i];
            const V k2 = m_k[i + 1];
            
            const T dt = t2 - t1;
            const V dv = v2 - v1;
            const T s = (t - t1) / dt;
            const V a =  k1 * dt - dv;
            const V b = -k2 * dt + dv;
            
            SDL_assert(q(s, v1, v2, a, b) == q(s, v1, v2, a, b));
            return q(s, v1, v2, a, b);
        }
    }
    
    inline V operator()(T t) const
    {
        return Sample(t);
    }

private:
    std::vector<T> m_t;
    std::vector<V> m_v;
    std::vector<V> m_k;
    
    inline std::size_t FindInterval(T t) const
    {
        SDL_assert(m_t.front() <= t && t <= m_t.back());
        // TODO: "linear search:
        std::size_t i = 0;
        while (t > m_t[i + 1])
        {
            ++i;
        }
        SDL_assert(i < m_t.size() - 1);
        return i;
    }
    
    inline void CalculateK()
    {
        const std::size_t num_points = m_v.size();
        
        std::vector<T> dt(num_points - 1);
        for (auto i : IRange<std::size_t>(0, num_points - 1))
        {
            dt[i] = m_t[i+1] - m_t[i];
        }
        
        std::vector<V> dv(num_points - 1);
        for (auto i : IRange<std::size_t>(0, num_points - 1))
        {
            dv[i] = m_v[i+1] - m_v[i];
        }
        
        std::vector<V> d(num_points);
        d.front() = V(2.0f / dt[0]);
        for (auto i : IRange<std::size_t>(1, num_points - 1))
        {
            d[i] = V(2.0 * (1.0f / dt[i-1] + 1.0f / dt[i]));
        }
        d.back() = V(2.0f / dt[num_points-2]);
        
        std::vector<V> a(num_points - 1);
        for (auto i : IRange(num_points - 1))
        {
            a[i] = V(1.0f / dt[i]);
        }
        
        std::vector<V> b(num_points);
        b.front() = 3.0f * dv[0] / (dt[0] * dt[0]);
        for (auto i : IRange<std::size_t>(1, num_points - 1))
        {
            b[i] = 3.0f * (dv[i-1] / (dt[i-1]*dt[i-1]) + dv[i] / (dt[i]*dt[i]));
        }
        b.back() = 3.0f * dv[num_points-2] / (dt[num_points-2] * dt[num_points-2]);
        
        a[0] = a[0] / d[0];
        for (auto i : IRange<std::size_t>(1, num_points - 1))
        {
            a[i] = a[i] / (d[i] - d[i-1] * a[i-1] * a[i-1]);
            SDL_assert(a[i] == a[i]);
        }
        
        b[0] = b[0] / d[0];
        for (auto i : IRange<std::size_t>(1, num_points))
        {
            b[i] = (b[i] - d[i-1] * a[i-1] * b[i-1]) / (d[i] - d[i-1] * a[i-1] * a[i-1]);
            SDL_assert(b[i] == b[i]);
        }
        
        m_k.resize(num_points);
        m_k.back() = b.back();
        for (std::size_t i = num_points - 1; i > 0; --i)
        {
            m_k[i - 1] = b[i - 1] - a[i - 1] * m_k[i];
        }
    }
};
