#include <gtest/gtest.h>
#include "Stream.hpp"
#include "core/MutableArraySequence.hpp"
#include "Complex.hpp"
#include "Student.hpp"
#include "Teacher.hpp"
#include <memory>
#include <functional>
#include <string>

template<typename T>
Stream<T> MakeStream(const std::initializer_list<T>& list) {
    auto seq = std::make_unique<MutableArraySequence<T>>();
    for (const T& val : list) seq->Append(val);
    return Stream<T>(std::move(seq));
}

TEST(StreamTest, Map) {
    auto s = MakeStream<int>({1,2,3,4});
    auto mapped = s.Map<int>([](int x){ return x*x; });
    EXPECT_EQ(mapped.GetLength(), 4);
    EXPECT_EQ(mapped.GetFirst(), 1);
    EXPECT_EQ(mapped.GetLast(), 16);
}

TEST(StreamTest, MapChangeType) {
    auto s = MakeStream<int>({1,2,3});
    auto mapped = s.Map<std::string>([](int x){ return std::to_string(x); });
    EXPECT_EQ(mapped.GetLength(), 3);
    EXPECT_EQ(mapped.GetFirst(), "1");
    EXPECT_EQ(mapped.GetLast(), "3");
}

TEST(StreamTest, Where) {
    auto s = MakeStream<int>({1,2,3,4,5});
    auto filtered = s.Where([](int x){ return x%2==0; });
    EXPECT_EQ(filtered.GetLength(), 2);
    EXPECT_EQ(filtered.Get(0), 2);
    EXPECT_EQ(filtered.Get(1), 4);
}

TEST(StreamTest, WhereEmpty) {
    auto s = MakeStream<int>({});
    auto filtered = s.Where([](int){ return true; });
    EXPECT_EQ(filtered.GetLength(), 0);
}

TEST(StreamTest, Reduce) {
    auto s = MakeStream<int>({1,2,3,4});
    int sum = s.Reduce([](int a, int b){ return a+b; }, 0);
    EXPECT_EQ(sum, 10);
}

TEST(StreamTest, ReduceEmpty) {
    auto s = MakeStream<int>({});
    int sum = s.Reduce([](int a, int b){ return a+b; }, 0);
    EXPECT_EQ(sum, 0);
}

TEST(StreamTest, ContainsSubsequence) {
    auto s1 = MakeStream<int>({1,2,3,4,5});
    auto s2 = MakeStream<int>({2,3,4});
    EXPECT_TRUE(s1.ContainsSubsequence(s2));
}

TEST(StreamTest, NotContainsSubsequence) {
    auto s1 = MakeStream<int>({1,2,3,4,5});
    auto s2 = MakeStream<int>({2,3,6});
    EXPECT_FALSE(s1.ContainsSubsequence(s2));
}

TEST(StreamTest, Split) {
    auto s = MakeStream<int>({1,2,3,4,5,6});
    auto [even, odd] = s.Split([](int x){ return x%2==0; });
    EXPECT_EQ(even.GetLength(), 3);
    EXPECT_EQ(odd.GetLength(), 3);
    EXPECT_EQ(even.GetFirst(), 2);
    EXPECT_EQ(odd.GetFirst(), 1);
}

TEST(StreamTest, Merge) {
    auto s1 = MakeStream<int>({1,2,3});
    auto s2 = MakeStream<int>({4,5,6});
    auto merged = s1.Merge(s2);
    EXPECT_EQ(merged.GetLength(), 6);
    EXPECT_EQ(merged.Get(3), 4);
}

TEST(StreamTest, GetSubsequence) {
    auto s = MakeStream<int>({10,20,30,40,50});
    auto sub = s.GetSubsequence(1, 3);
    EXPECT_EQ(sub.GetLength(), 3);
    EXPECT_EQ(sub.Get(0), 20);
    EXPECT_EQ(sub.Get(2), 40);
}

TEST(StreamTest, GetSubsequenceOutOfRange) {
    auto s = MakeStream<int>({1,2,3});
    EXPECT_THROW(s.GetSubsequence(1, 3), std::out_of_range);
    EXPECT_THROW(s.GetSubsequence(2, 1), std::out_of_range);
}

TEST(StreamTest, ComplexNumbers) {
    auto s = MakeStream<Complex>({{1,2},{3,4},{5,6}});
    auto mapped = s.Map<Complex>([](Complex c){ return Complex{c.re*2, c.im*2}; });
    EXPECT_DOUBLE_EQ(mapped.GetFirst().re, 2.0);
    EXPECT_DOUBLE_EQ(mapped.GetFirst().im, 4.0);
}

TEST(StreamTest, Students) {
    Student s1{"Alice", 4.5}, s2{"Bob", 3.8};
    auto stream = MakeStream<Student>({s1, s2});
    auto filtered = stream.Where([](Student s){ return s.grade > 4.0; });
    EXPECT_EQ(filtered.GetLength(), 1);
    EXPECT_EQ(filtered.GetFirst().name, "Alice");
}

TEST(StreamTest, Teachers) {
    Teacher t1{"Smith", 50000}, t2{"Johnson", 60000};
    auto stream = MakeStream<Teacher>({t1, t2});
    auto mapped = stream.Map<Teacher>([](Teacher t){ t.name = "Dr. " + t.name; return t; });
    EXPECT_EQ(mapped.GetFirst().name, "Dr. Smith");
}