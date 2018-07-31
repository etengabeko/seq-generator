#ifndef SEQUENCE_GENERATOR_THREAD_SAFE_GENERATOR_H
#define SEQUENCE_GENERATOR_THREAD_SAFE_GENERATOR_H

#include <mutex>

#include "base_generator.h"

namespace sequence
{

template <typename T,
          typename IncOp = details::CustomIncrement<T>>
class ThreadSafeGenerator
{
public:
    ThreadSafeGenerator() = default;

    explicit ThreadSafeGenerator(const T& startValue) :
        m_gen(startValue)
    { }

    ThreadSafeGenerator(const T& startValue, IncOp increment) :
        m_gen(startValue, increment)
    { }

    T next()
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);
        return m_gen.next();
    }

    T peekNext() const
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);
        return m_gen.peekNext();
    }

private:
    BaseGenerator<T, IncOp> m_gen;
    mutable std::recursive_mutex m_mutex;

};

} // sequence

#endif // SEQUENCE_GENERATOR_THREAD_SAFE_GENERATOR_H
