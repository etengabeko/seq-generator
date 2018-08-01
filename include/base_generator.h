#ifndef SEQUENCE_GENERATOR_BASE_GENERATOR_H
#define SEQUENCE_GENERATOR_BASE_GENERATOR_H

#include <cassert>
#include <limits>
#include <type_traits>

namespace sequence
{
namespace details
{
/**
 * @struct CustomIncrement
 * @brief Метод вычисления следующего значения последовательности,
 *        применяемый генератором по умолчанию.
 */
template <typename T>
struct CustomIncrement
{
    T& operator()(T& t) const
    {
        return ++t;
    }
};

/**
 * @brief isNextInRange - template overloads for signed/unsigned types.
 * @sa BaseGenerator::isNextInRange
 */
template <typename T,
          typename IncOp = details::CustomIncrement<T>,
          typename std::enable_if<std::is_signed<T>::value, T>::type* = nullptr>
bool isNextInRange(const T& current, const IncOp& increment)
{
    T zero = static_cast<T>(0);
    return (std::numeric_limits<T>::max() - std::abs(current) >= std::abs(increment(zero)));
}

template <typename T,
          typename IncOp = details::CustomIncrement<T>,
          typename std::enable_if<std::is_unsigned<T>::value, T>::type* = nullptr>
bool isNextInRange(const T& /*current*/, const IncOp& /*increment*/)
{
    return true;
}

} // details

/**
 * @class BaseGenerator
 * @brief Генератор последовательности числовых значений.
 */
template <typename T,
          typename IncOp = details::CustomIncrement<T>,
          typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
class BaseGenerator
{
public:
    /**
     * @brief BaseGenerator constructor
     * @param startValue - начальное значение генерируемой последовательности
     * @param increment - метод получения следующего значения последовательности.
     */
    BaseGenerator(const T& startValue, IncOp increment) :
        m_currentValue(startValue),
        m_isFirstRun(true),
        m_startValue(startValue),
        m_increment(increment)
    { }

    /**
     * @brief BaseGenerator constructor
     * @param startValue - начальное значение генерируемой последовательности
     */
    explicit BaseGenerator(const T& startValue) :
        BaseGenerator(startValue, IncOp())
    { }

    /**
     * @brief BaseGenerator default constructor
     */
    BaseGenerator() :
        BaseGenerator(T(), IncOp())
    { }

    /**
     * @brief reset - сбрасывает текущее значение генерируемой последовательности в значение startValue.
     */
    void reset()
    {
        reset(m_currentValue);
        m_isFirstRun = true;
    }

    /**
     * @brief next - генерирует следующее значение последовательности.
     *        Заменяет текущее значение на сгенерированное.
     * @return следующее значение последовательности
     */
    T next()
    {
        if (m_isFirstRun)
        {
            m_isFirstRun = false;
            return m_currentValue;
        }

        return safeIncrement(m_currentValue);
    }

    /**
     * @brief peekNext - возвращает следующее значение для генерируемой последовательности,
     *        не изменяя при этом текущее.
     * @return следующее значение последовательности.
     */
    T peekNext() const
    {
        if (m_isFirstRun)
        {
            return m_currentValue;
        }

        T tmp(m_currentValue);
        return safeIncrement(tmp);
    }

private:
    /**
     * @brief isNextInRange - проверяет допустимость следующего значения последовательности.
     * @param current - текущее значение последовательности.
     * @return true если следующее значение входит в допустимый диапазон значений для типа.
     */
    bool isNextInRange(const T& current) const
    {
        return details::isNextInRange(current, m_increment);
    }

    /**
     * @brief safeIncrement - вычисляет следующее значение последовательности.
     * @param current - текущее значение последовательности.
     * @return следующее значение последовательности.
     *
     * @note Если следующее значение последовательности выходит за границы допустимого для типа диапазона,
     *       то будет возвращено startValue.
     */
    T safeIncrement(T& current) const
    {
        return (isNextInRange(current) ? m_increment(current)
                                       : reset(current));
    }

    /**
     * @brief reset - сбрасывает значение value в значение startValue.
     * @param value - ссылка на сбрасываемую переменную.
     * @return новое значение сброшенной переменной (startValue).
     */
    T& reset(T& value) const
    {
        value = m_startValue;
        return value;
    }

private:
    T m_currentValue;        //!< Текущее значение последовательности
    bool m_isFirstRun;       //!< Флаг, идентифицирующий было ли сгенерировано первое (начальное) значение последовательности.

    const T m_startValue;    //!< Начальное значение последовательности.
    const IncOp m_increment; //!< Метод получения следующего значения последовательности.

};

} // sequence

#endif // SEQUENCE_GENERATOR_BASE_GENERATOR_H
