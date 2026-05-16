// tests/test_stream.cpp
#include <gtest/gtest.h>
#include "Stream.hpp"
#include "core/Array/MutableArraySequence.hpp"
#include "Complex.h"
#include "Student.h"
#include "Teacher.h"
#include <functional>

// Helper to create Stream from initializer list
template<typename T>
Stream<T>* MakeStream(const std::initializer_list<T>& list) {
    auto* seq = new MutableArraySequence<T>();
    for (const T& val : list) seq->Append(val);
    return new Stream<T>(seq);
}

TEST(StreamTest, Map) {
    auto* s = MakeStream<int>({1,2,3,4});
    auto* mapped = s->Map([](int x){ return x*x; });
    EXPECT_EQ(mapped->GetLength(), 4);
    EXPECT_EQ(mapped->GetFirst(), 1);
    EXPECT_EQ(mapped->GetLast(), 16);
    delete s; delete mapped;
}

TEST(StreamTest, Where) {
    auto* s = MakeStream<int>({1,2,3,4,5});
    auto* filtered = s->Where([](int x){ return x%2==0; });
    EXPECT_EQ(filtered->GetLength(), 2);
    EXPECT_EQ(filtered->Get(0), 2);
    EXPECT_EQ(filtered->Get(1), 4);
    delete s; delete filtered;
}

TEST(StreamTest, Reduce) {
    auto* s = MakeStream<int>({1,2,3,4});
    int sum = s->Reduce([](int a, int b){ return a+b; }, 0);
    EXPECT_EQ(sum, 10);
    delete s;
}

TEST(StreamTest, ContainsSubsequence) {
    auto* s1 = MakeStream<int>({1,2,3,4,5});
    auto* s2 = MakeStream<int>({2,3,4});
    EXPECT_TRUE(s1->ContainsSubsequence(*s2));
    delete s1; delete s2;
}

TEST(StreamTest, Split) {
    auto* s = MakeStream<int>({1,2,3,4,5,6});
    auto [even, odd] = s->Split([](int x){ return x%2==0; });
    EXPECT_EQ(even->GetLength(), 3);
    EXPECT_EQ(odd->GetLength(), 3);
    EXPECT_EQ(even->GetFirst(), 2);
    EXPECT_EQ(odd->GetFirst(), 1);
    delete s; delete even; delete odd;
}

TEST(StreamTest, Merge) {
    auto* s1 = MakeStream<int>({1,2,3});
    auto* s2 = MakeStream<int>({4,5,6});
    auto* merged = s1->Merge(*s2);
    EXPECT_EQ(merged->GetLength(), 6);
    EXPECT_EQ(merged->Get(3), 4);
    delete s1; delete s2; delete merged;
}

TEST(StreamTest, ComplexNumbers) {
    auto* s = MakeStream<Complex>({{1,2},{3,4},{5,6}});
    auto* mapped = s->Map([](Complex c){ return Complex{c.re*2, c.im*2}; });
    EXPECT_EQ(mapped->GetFirst().re, 2);
    EXPECT_EQ(mapped->GetFirst().im, 4);
    delete s; delete mapped;
}

TEST(StreamTest, Students) {
    Student s1{"Alice", 101}, s2{"Bob", 102};
    auto* stream = MakeStream<Student>({s1, s2});
    auto* filtered = stream->Where([](Student s){ return s.id == 101; });
    EXPECT_EQ(filtered->GetLength(), 1);
    EXPECT_EQ(filtered->GetFirst().name, "Alice");
    delete stream; delete filtered;
}

TEST(StreamTest, Teachers) {
    Teacher t1{"Smith", "Math"}, t2{"Johnson", "Physics"};
    auto* stream = MakeStream<Teacher>({t1, t2});
    auto* mapped = stream->Map([](Teacher t){ t.name = "Dr. " + t.name; return t; });
    EXPECT_EQ(mapped->GetFirst().name, "Dr. Smith");
    delete stream; delete mapped;
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}