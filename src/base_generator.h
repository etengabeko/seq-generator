#ifndef SEQUENCE_GENERATOR_BASE_GENERATOR_H
#define SEQUENCE_GENERATOR_BASE_GENERATOR_H

namespace sequence
{
namespace details
{
template <typename T>
struct CustomIncrement
{
    T& operator()(T& t) const
    {
        return ++t;
    }
};

} // details

template <typename T,
          typename IncOp = details::CustomIncrement<T>>
class BaseGenerator
{
public:
    BaseGenerator(const T& startValue, IncOp increment) :
        m_currentValue(startValue),
        m_increment(increment)
    { }

    explicit BaseGenerator(const T& startValue) :
        BaseGenerator(startValue, IncOp())
    { }

    BaseGenerator() :
        BaseGenerator(T(), IncOp())
    { }

    T next()
    {
        return m_increment(m_currentValue);
    }

    T peekNext() const
    {
        T tmp(m_currentValue);
        return m_increment(tmp);
    }

private:
    T m_currentValue;
    const IncOp m_increment;

};

} // sequence

#endif // SEQUENCE_GENERATOR_BASE_GENERATOR_H
