#include <cstdint>
#include <memory>
#include <thread>
#include <type_traits>
#include <vector>

#include <gtest/gtest.h>

#include <thread_safe_generator.h>

using sequence::BaseGenerator;
using sequence::ThreadSafeGenerator;

namespace details
{
template <typename T>
void testDefaultGenerator()
{
    BaseGenerator<T> gen;

    auto testCase = [&gen]()
    {
        EXPECT_EQ(static_cast<T>(0), gen.peekNext());
        EXPECT_EQ(static_cast<T>(0), gen.next());
        EXPECT_EQ(static_cast<T>(1), gen.peekNext());
        EXPECT_EQ(static_cast<T>(1), gen.next());
    };

    testCase();
    gen.reset();
    testCase();
}

template <typename T>
void testGeneratorWithStartValue(T startValue)
{
    BaseGenerator<T> gen(startValue);

    const T nextValue = startValue + 1;
    auto testCase = [&gen, startValue, nextValue]()
    {
        EXPECT_EQ(static_cast<T>(startValue), gen.peekNext());
        EXPECT_EQ(static_cast<T>(startValue), gen.next());
        EXPECT_EQ(static_cast<T>(nextValue),  gen.peekNext());
        EXPECT_EQ(static_cast<T>(nextValue),  gen.next());
    };

    testCase();
    gen.reset();
    testCase();
}

template <typename T, typename IncOp>
void testGeneratorWithCustomIncrement(T startValue, IncOp increment)
{
    BaseGenerator<T, IncOp> gen(startValue, increment);

    T tmp = startValue;
    const T nextValue = increment(tmp);
    auto testCase = [&gen, startValue, nextValue]()
    {
        EXPECT_EQ(static_cast<T>(startValue), gen.peekNext());
        EXPECT_EQ(static_cast<T>(startValue), gen.next());
        EXPECT_EQ(static_cast<T>(nextValue),  gen.peekNext());
        EXPECT_EQ(static_cast<T>(nextValue),  gen.next());
    };

    testCase();
    gen.reset();
    testCase();
}

template <typename T, typename IncOp>
void testGeneratorOverflow(T startValue, IncOp increment)
{
    BaseGenerator<T, IncOp> gen(startValue, increment);
    for (size_t i = 0; i < 100; ++i)
        EXPECT_EQ(gen.next(), startValue);
}

} // details

TEST(BaseGeneratorTest, Defaults)
{
    details::testDefaultGenerator<int>();
    details::testDefaultGenerator<int8_t>();
    details::testDefaultGenerator<int64_t>();

    details::testDefaultGenerator<float>();
    details::testDefaultGenerator<double>();

    details::testDefaultGenerator<unsigned int>();
    details::testDefaultGenerator<uint8_t>();
    details::testDefaultGenerator<uint64_t>();
}

TEST(BaseGeneratorTest, StartValue)
{
    details::testGeneratorWithStartValue<int>(-10);
    details::testGeneratorWithStartValue<int8_t>(10);
    details::testGeneratorWithStartValue<int64_t>(-1000);

    details::testGeneratorWithStartValue<float>(-11.11);
    details::testGeneratorWithStartValue<double>(22.22);

    details::testGeneratorWithStartValue<unsigned int>(20);
    details::testGeneratorWithStartValue<uint8_t>(200);
    details::testGeneratorWithStartValue<uint64_t>(2000);
}

TEST(BaseGeneratorTest, CustomIncrement)
{
    details::testGeneratorWithCustomIncrement<int>(0, [](int& i) { i += 5; return i; });
    details::testGeneratorWithCustomIncrement<int8_t>(100, [](int8_t& i) { i += 2; return i; });
    details::testGeneratorWithCustomIncrement<int64_t>(-1000, [](int64_t& i) { i -= 10; return i; });

    details::testGeneratorWithCustomIncrement<float>(-11.11, [](float& v) { v += 0.01; return v; });
    details::testGeneratorWithCustomIncrement<double>(22.22, [](double& v) { v -= 2.00; return v; });

    details::testGeneratorWithCustomIncrement<unsigned int>(20, [](unsigned int& i) { return i; });
    details::testGeneratorWithCustomIncrement<uint8_t>(200, [](uint8_t& i) { i -= 5; return i; });
    details::testGeneratorWithCustomIncrement<uint64_t>(2000, [](uint64_t& i) { i += 10; return i; });
}

TEST(BaseGeneratorTest, Overflowing)
{
    details::testGeneratorOverflow<int8_t>(125, [](int8_t& i) { i += 10; return i; });
    details::testGeneratorOverflow<int8_t>(-125, [](int8_t& i) { i -= 10; return i; });
}

TEST(ThreadSafeGeneratorTest, Multithreads)
{
    const size_t kTicksCount = 10000;
    const size_t kThreadsCount = 100;

    ThreadSafeGenerator<size_t> gen;

    auto runFunc = [&gen, kTicksCount]()
    {
        for (size_t i = 0; i < kTicksCount; ++i)
        {
            gen.next();
        }
    };

    std::vector<std::unique_ptr<std::thread>> threads;
    for (size_t i = 0; i < kThreadsCount; ++i)
    {
        threads.emplace_back(new std::thread(runFunc));
    }

    for (auto& each : threads)
    {
        each->join();
    }

    EXPECT_EQ(kThreadsCount * kTicksCount, gen.peekNext());
}
