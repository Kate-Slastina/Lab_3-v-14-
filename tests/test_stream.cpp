#include <gtest/gtest.h>
#include <vector>
#include <sstream>
#include <type_traits>
#include <cmath>
#include <memory>
#include <functional>
#include <string>
#include "Stream.hpp"
#include "core/MutableArraySequence.hpp"
#include "Complex.hpp"
#include "Student.hpp"
#include "Teacher.hpp"

// инфраструктура

template<typename T>
Stream<T> MakeStream(const std::initializer_list<T>& list) {
    auto seq = std::make_unique<MutableArraySequence<T>>();
    for (const T& val : list) seq->Append(val);
    return Stream<T>(std::move(seq));
}

template <typename T>
struct StreamContext {
    const char* test_name;
    const char* operation;
    std::vector<T> expected_data;
    double tolerance = 0.0;

    std::string Format() const {
        std::ostringstream msg;
        msg << "========================================\n";
        msg << "Test:      " << test_name << "\n";
        msg << "Operation: " << (operation ? operation : "null") << "\n";
        msg << "Expected Size: " << expected_data.size() << "\n";
        msg << "Expected Data: [";
        for (size_t i = 0; i < expected_data.size(); ++i) {
            msg << expected_data[i];
            if (i + 1 < expected_data.size()) msg << ", ";
        }
        msg << "]\n========================================\n";
        return msg.str();
    }
};

template <typename T>
::testing::AssertionResult AssertStreamState(const StreamContext<T>& ctx, const Stream<T>& stream) {
    if (stream.GetLength() != ctx.expected_data.size()) {
        return ::testing::AssertionFailure() 
            << "[SIZE MISMATCH]\n" << ctx.Format() 
            << "Actual Size: " << stream.GetLength() << "\n";
    }
    
    for (size_t i = 0; i < ctx.expected_data.size(); ++i) {
        T actual = stream.Get(i);
        T expected = ctx.expected_data[i];
        bool eq = false;
        
        if constexpr (std::is_floating_point_v<T>) {
            eq = std::abs(actual - expected) <= ctx.tolerance;
        } else {
            eq = (actual == expected);
        }
        
        if (!eq) {
            return ::testing::AssertionFailure() 
                << "[DATA MISMATCH at index " << i << "]\n" << ctx.Format() 
                << "Actual Data[" << i << "]: " << actual << "\n";
        }
    }
    return ::testing::AssertionSuccess();
}

// Спец для комплекса
template <>
::testing::AssertionResult AssertStreamState<Complex>(const StreamContext<Complex>& ctx, const Stream<Complex>& stream) {
    if (stream.GetLength() != ctx.expected_data.size()) {
        return ::testing::AssertionFailure() 
            << "[SIZE MISMATCH]\n" << ctx.Format() 
            << "Actual Size: " << stream.GetLength() << "\n";
    }
    
    for (size_t i = 0; i < ctx.expected_data.size(); ++i) {
        Complex actual = stream.Get(i);
        Complex expected = ctx.expected_data[i];
        
        constexpr double eps = 1e-9;
        if (std::abs(actual.re - expected.re) > eps || std::abs(actual.im - expected.im) > eps) {
            return ::testing::AssertionFailure() 
                << "[DATA MISMATCH at index " << i << "]\n" << ctx.Format() 
                << "Actual Data[" << i << "]: (" << actual.re << ", " << actual.im << ")\n";
        }
    }
    return ::testing::AssertionSuccess();
}

// функторы
struct SquareFunctor {
    int operator()(int x) const {
        return x * x;
    }
};
struct IntToStringFunctor {
    std::string operator()(int x) const {
        return std::to_string(x);
    }
};
struct IsEvenFunctor {
    bool operator()(int x) const {
        return x % 2 == 0;
    }
};
struct IsOddFunctor {
    bool operator()(int x) const {
        return x % 2 != 0;
    }
};
struct AddFunctor {
    int operator()(int a, int b) const {
        return a + b;
    }
};
struct MultiplyFunctor {
    int operator()(int a, int b) const {
        return a * b;
    }
};
struct ComplexDoubleFunctor {
    Complex operator()(Complex c) const {
        return Complex{c.re * 2, c.im * 2};
    }
};
struct GradeGreaterThan4Functor {
    bool operator()(Student s) const {
        return s.grade > 4.0;
    }
};

// Функтор для добавления "Dr. " к имени(просто красиво)
struct AddDrPrefixFunctor {
    Teacher operator()(Teacher t) const {
        t.name = "Dr. " + t.name;
        return t;
    }
};

// функции без лямбд
int Square(int x) {
    return x * x;
}

std::string IntToString(int x) {
    return std::to_string(x);
}

bool IsEven(int x) {
    return x % 2 == 0;
}

bool IsOdd(int x) {
    return x % 2 != 0;
}

int Add(int a, int b) {
    return a + b;
}

int Multiply(int a, int b) {
    return a * b;
}

// фикстура

class StreamTest : public ::testing::Test {
protected:
    void SetUp() override {
        ::testing::FLAGS_gtest_stack_trace_depth = 10;
    }
    // void TearDown();
};

// мама(папа)

TEST_F(StreamTest, Map) {
    auto s = MakeStream<int>({1, 2, 3, 4});
    auto mapped = s.Map<int>(SquareFunctor());
    
    StreamContext<int> ctx{"Map", "Map(SquareFunctor)", {1, 4, 9, 16}};
    ASSERT_TRUE(AssertStreamState(ctx, mapped));
}

TEST_F(StreamTest, MapWithFunction) {
    auto s = MakeStream<int>({1, 2, 3, 4});
    auto mapped = s.Map<int>(Square);
    
    StreamContext<int> ctx{"MapWithFunction", "Map(Square)", {1, 4, 9, 16}};
    ASSERT_TRUE(AssertStreamState(ctx, mapped));
}

TEST_F(StreamTest, MapChangeType) {
    auto s = MakeStream<int>({1, 2, 3});
    auto mapped = s.Map<std::string>(IntToStringFunctor());
    
    StreamContext<std::string> ctx{"MapChangeType", "Map(IntToStringFunctor)", {"1", "2", "3"}};
    ASSERT_TRUE(AssertStreamState(ctx, mapped));
}

TEST_F(StreamTest, MapChangeTypeWithFunction) {
    auto s = MakeStream<int>({1, 2, 3});
    auto mapped = s.Map<std::string>(IntToString);
    
    StreamContext<std::string> ctx{"MapChangeTypeWithFunction", "Map(IntToString)", {"1", "2", "3"}};
    ASSERT_TRUE(AssertStreamState(ctx, mapped));
}

// вере

TEST_F(StreamTest, Where) {
    auto s = MakeStream<int>({1, 2, 3, 4, 5});
    auto filtered = s.Where(IsEvenFunctor());
    
    StreamContext<int> ctx{"Where", "Where(IsEvenFunctor)", {2, 4}};
    ASSERT_TRUE(AssertStreamState(ctx, filtered));
}

TEST_F(StreamTest, WhereWithFunction) {
    auto s = MakeStream<int>({1, 2, 3, 4, 5});
    auto filtered = s.Where(IsEven);
    
    StreamContext<int> ctx{"WhereWithFunction", "Where(IsEven)", {2, 4}};
    ASSERT_TRUE(AssertStreamState(ctx, filtered));
}

TEST_F(StreamTest, WhereEmpty) {
    auto s = MakeStream<int>({});
    auto filtered = s.Where(IsEvenFunctor());
    
    StreamContext<int> ctx{"WhereEmpty", "Where(IsEvenFunctor)", {}};
    ASSERT_TRUE(AssertStreamState(ctx, filtered));
}

TEST_F(StreamTest, WhereNoMatch) {
    auto s = MakeStream<int>({1, 3, 5, 7});
    auto filtered = s.Where(IsEvenFunctor());
    
    StreamContext<int> ctx{"WhereNoMatch", "Where(IsEvenFunctor)", {}};
    ASSERT_TRUE(AssertStreamState(ctx, filtered));
}

TEST_F(StreamTest, WhereOdd) {
    auto s = MakeStream<int>({1, 2, 3, 4, 5});
    auto filtered = s.Where(IsOddFunctor());
    
    StreamContext<int> ctx{"WhereOdd", "Where(IsOddFunctor)", {1, 3, 5}};
    ASSERT_TRUE(AssertStreamState(ctx, filtered));
}

// редюс

TEST_F(StreamTest, Reduce) {
    auto s = MakeStream<int>({1, 2, 3, 4});
    int sum = s.Reduce(AddFunctor(), 0);
    
    StreamContext<int> ctx{"Reduce", "Reduce(AddFunctor, 0)", {}};
    SCOPED_TRACE(ctx.Format());
    EXPECT_EQ(sum, 10);
}

TEST_F(StreamTest, ReduceWithFunction) {
    auto s = MakeStream<int>({1, 2, 3, 4});
    int sum = s.Reduce(Add, 0);
    
    StreamContext<int> ctx{"ReduceWithFunction", "Reduce(Add, 0)", {}};
    SCOPED_TRACE(ctx.Format());
    EXPECT_EQ(sum, 10);
}

TEST_F(StreamTest, ReduceEmpty) {
    auto s = MakeStream<int>({});
    int sum = s.Reduce(AddFunctor(), 0);
    
    StreamContext<int> ctx{"ReduceEmpty", "Reduce(AddFunctor, 0)", {}};
    SCOPED_TRACE(ctx.Format());
    EXPECT_EQ(sum, 0);
}

TEST_F(StreamTest, ReduceProduct) {
    auto s = MakeStream<int>({1, 2, 3, 4});
    int product = s.Reduce(MultiplyFunctor(), 1);
    
    StreamContext<int> ctx{"ReduceProduct", "Reduce(MultiplyFunctor, 1)", {}};
    SCOPED_TRACE(ctx.Format());
    EXPECT_EQ(product, 24);
}

TEST_F(StreamTest, ReduceProductWithFunction) {
    auto s = MakeStream<int>({1, 2, 3, 4});
    int product = s.Reduce(Multiply, 1);
    
    StreamContext<int> ctx{"ReduceProductWithFunction", "Reduce(Multiply, 1)", {}};
    SCOPED_TRACE(ctx.Format());
    EXPECT_EQ(product, 24);
}

// сабсик

TEST_F(StreamTest, ContainsSubsequence) {
    auto s1 = MakeStream<int>({1, 2, 3, 4, 5});
    auto s2 = MakeStream<int>({2, 3, 4});
    
    StreamContext<int> ctx{"ContainsSubsequence", "ContainsSubsequence({2,3,4})", {1, 2, 3, 4, 5}};
    SCOPED_TRACE(ctx.Format());
    EXPECT_TRUE(s1.ContainsSubsequence(s2));
}

TEST_F(StreamTest, NotContainsSubsequence) {
    auto s1 = MakeStream<int>({1, 2, 3, 4, 5});
    auto s2 = MakeStream<int>({2, 3, 6});
    
    StreamContext<int> ctx{"NotContainsSubsequence", "ContainsSubsequence({2,3,6})", {1, 2, 3, 4, 5}};
    SCOPED_TRACE(ctx.Format());
    EXPECT_FALSE(s1.ContainsSubsequence(s2));
}

TEST_F(StreamTest, ContainsSubsequenceEmpty) {
    auto s1 = MakeStream<int>({1, 2, 3});
    auto s2 = MakeStream<int>({});
    
    StreamContext<int> ctx{"ContainsSubsequenceEmpty", "ContainsSubsequence({})", {1, 2, 3}};
    SCOPED_TRACE(ctx.Format());
    EXPECT_TRUE(s1.ContainsSubsequence(s2));
}

// сплит

TEST_F(StreamTest, Split) {
    auto s = MakeStream<int>({1, 2, 3, 4, 5, 6});
    auto [even, odd] = s.Split(IsEvenFunctor());
    
    StreamContext<int> ctx_even{"Split_Even", "Split(IsEvenFunctor) -> even", {2, 4, 6}};
    ASSERT_TRUE(AssertStreamState(ctx_even, even));
    
    StreamContext<int> ctx_odd{"Split_Odd", "Split(IsEvenFunctor) -> odd", {1, 3, 5}};
    ASSERT_TRUE(AssertStreamState(ctx_odd, odd));
}

TEST_F(StreamTest, SplitWithFunction) {
    auto s = MakeStream<int>({1, 2, 3, 4, 5, 6});
    auto [even, odd] = s.Split(IsEven);
    
    StreamContext<int> ctx_even{"SplitWithFunction_Even", "Split(IsEven) -> even", {2, 4, 6}};
    ASSERT_TRUE(AssertStreamState(ctx_even, even));
    
    StreamContext<int> ctx_odd{"SplitWithFunction_Odd", "Split(IsEven) -> odd", {1, 3, 5}};
    ASSERT_TRUE(AssertStreamState(ctx_odd, odd));
}

TEST_F(StreamTest, SplitAllMatch) {
    auto s = MakeStream<int>({2, 4, 6, 8});
    auto [even, odd] = s.Split(IsEvenFunctor());
    
    StreamContext<int> ctx_even{"SplitAllMatch_Even", "Split -> even", {2, 4, 6, 8}};
    ASSERT_TRUE(AssertStreamState(ctx_even, even));
    
    StreamContext<int> ctx_odd{"SplitAllMatch_Odd", "Split -> odd", {}};
    ASSERT_TRUE(AssertStreamState(ctx_odd, odd));
}

// мерджи

TEST_F(StreamTest, Merge) {
    auto s1 = MakeStream<int>({1, 2, 3});
    auto s2 = MakeStream<int>({4, 5, 6});
    auto merged = s1.Merge(s2);
    
    StreamContext<int> ctx{"Merge", "Merge({4,5,6})", {1, 2, 3, 4, 5, 6}};
    ASSERT_TRUE(AssertStreamState(ctx, merged));
}

TEST_F(StreamTest, MergeWithEmpty) {
    auto s1 = MakeStream<int>({1, 2, 3});
    auto s2 = MakeStream<int>({});
    auto merged = s1.Merge(s2);
    
    StreamContext<int> ctx{"MergeWithEmpty", "Merge({})", {1, 2, 3}};
    ASSERT_TRUE(AssertStreamState(ctx, merged));
}

// сабы

TEST_F(StreamTest, GetSubsequence) {
    auto s = MakeStream<int>({10, 20, 30, 40, 50});
    auto sub = s.GetSubsequence(1, 3);
    
    StreamContext<int> ctx{"GetSubsequence", "GetSubsequence(1, 3)", {20, 30, 40}};
    ASSERT_TRUE(AssertStreamState(ctx, sub));
}

TEST_F(StreamTest, GetSubsequenceOutOfRange) {
    auto s = MakeStream<int>({1, 2, 3});
    
    StreamContext<int> ctx{"GetSubsequenceOutOfRange", "GetSubsequence(1, 3)", {1, 2, 3}};
    SCOPED_TRACE(ctx.Format());
    
    try {
        auto sub = s.GetSubsequence(1, 3);
        FAIL() << ctx.Format() << "Expected std::out_of_range not thrown.\n";
    } catch (const std::out_of_range& e) {
        SUCCEED() << "Correctly caught std::out_of_range: " << e.what();
    } catch (const std::exception& e) {
        FAIL() << ctx.Format() << "Wrong exception type. Caught: " << typeid(e).name() 
               << " | Message: " << e.what() << "\n";
    }
}

TEST_F(StreamTest, GetSubsequenceInvalidRange) {
    auto s = MakeStream<int>({1, 2, 3});
    
    StreamContext<int> ctx{"GetSubsequenceInvalidRange", "GetSubsequence(2, 1)", {1, 2, 3}};
    SCOPED_TRACE(ctx.Format());
    
    try {
        auto sub = s.GetSubsequence(2, 1);
        FAIL() << ctx.Format() << "Expected std::out_of_range not thrown.\n";
    } catch (const std::out_of_range& e) {
        SUCCEED() << "Correctly caught std::out_of_range: " << e.what();
    } catch (const std::exception& e) {
        FAIL() << ctx.Format() << "Wrong exception type. Caught: " << typeid(e).name() 
               << " | Message: " << e.what() << "\n";
    }
}

// кастомы

TEST_F(StreamTest, ComplexNumbers) {
    auto s = MakeStream<Complex>({{1, 2}, {3, 4}, {5, 6}});
    auto mapped = s.Map<Complex>(ComplexDoubleFunctor());
    
    StreamContext<Complex> ctx{"ComplexNumbers", "Map(ComplexDoubleFunctor)", {{2, 4}, {6, 8}, {10, 12}}};
    ASSERT_TRUE(AssertStreamState(ctx, mapped));
}

TEST_F(StreamTest, Students) {
    Student s1{"Alice", 4.5}, s2{"Bob", 3.8};
    auto stream = MakeStream<Student>({s1, s2});
    auto filtered = stream.Where(GradeGreaterThan4Functor());
    
    StreamContext<Student> ctx{"Students", "Where(GradeGreaterThan4Functor)", {s1}};
    ASSERT_TRUE(AssertStreamState(ctx, filtered));
}

TEST_F(StreamTest, Teachers) {
    Teacher t1{"Smith", 50000}, t2{"Johnson", 60000};
    auto stream = MakeStream<Teacher>({t1, t2});
    auto mapped = stream.Map<Teacher>(AddDrPrefixFunctor());
    
    Teacher expected1{"Dr. Smith", 50000};
    Teacher expected2{"Dr. Johnson", 60000};
    
    StreamContext<Teacher> ctx{"Teachers", "Map(AddDrPrefixFunctor)", {expected1, expected2}};
    ASSERT_TRUE(AssertStreamState(ctx, mapped));
}

// комба

TEST_F(StreamTest, ChainedOperations) {
    auto s = MakeStream<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    
    auto result = s.Where(IsEvenFunctor())
                   .Map<int>(SquareFunctor());
    
    StreamContext<int> ctx{"ChainedOperations", "Where(IsEven).Map(Square)", {4, 16, 36, 64, 100}};
    ASSERT_TRUE(AssertStreamState(ctx, result));
}

TEST_F(StreamTest, ChainedOperationsWithFunctions) {
    auto s = MakeStream<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    
    auto result = s.Where(IsEven)
                   .Map<int>(Square);
    
    StreamContext<int> ctx{"ChainedOperationsWithFunctions", "Where(IsEven).Map(Square)", {4, 16, 360, 64, 100}};
    ASSERT_TRUE(AssertStreamState(ctx, result));
}

TEST_F(StreamTest, MapThenReduce) {
    auto s = MakeStream<int>({1, 2, 3, 4});
    int sum = s.Map<int>(SquareFunctor())
               .Reduce(AddFunctor(), 0);
    
    StreamContext<int> ctx{"MapThenReduce", "Map(Square).Reduce(Add)", {}};
    SCOPED_TRACE(ctx.Format());
    EXPECT_EQ(sum, 30);
}

TEST_F(StreamTest, MapThenReduceWithFunctions) {
    auto s = MakeStream<int>({1, 2, 3, 4});
    int sum = s.Map<int>(Square)
               .Reduce(Add, 0);
    
    StreamContext<int> ctx{"MapThenReduceWithFunctions", "Map(Square).Reduce(Add)", {}};
    SCOPED_TRACE(ctx.Format());
    EXPECT_EQ(sum, 30); 
}