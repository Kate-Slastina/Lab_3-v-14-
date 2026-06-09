#include <gtest/gtest.h>
#include "DynamicArray.hpp"

TEST(DynamicArrayConstructors, Default) {
    DynamicArray<int> arr;
    EXPECT_EQ(arr.GetSize(), 0u)
        << "input: DynamicArray<int> arr;\n"
        << "expected: size == 0";
}

TEST(DynamicArrayConstructors, FromSize) {
    DynamicArray<int> arr(5);
    ASSERT_EQ(arr.GetSize(), 5u)
        << "input: DynamicArray<int> arr(5)\n"
        << "expected: size == 5";
    EXPECT_NO_THROW(arr.Get(0))
        << "input: arr.Get(0)\nexpected: no throw";
    EXPECT_NO_THROW(arr.Get(4))
        << "input: arr.Get(4)\nexpected: no throw";
    arr.Set(0, 10);
    arr.Set(4, 20);
    EXPECT_EQ(arr.Get(0), 10)
        << "input: arr.Set(0,10) -> arr.Get(0)\nexpected: 10";
    EXPECT_EQ(arr.Get(4), 20)
        << "input: arr.Set(4,20) -> arr.Get(4)\nexpected: 20";
}

TEST(DynamicArrayConstructors, FromArray) {
    int items[] = {1,2,3};
    DynamicArray<int> arr(items, 3);
    ASSERT_EQ(arr.GetSize(), 3u);
    EXPECT_EQ(arr.Get(0), 1);
    EXPECT_EQ(arr.Get(1), 2);
    EXPECT_EQ(arr.Get(2), 3);
}

TEST(DynamicArrayConstructors, Copy) {
    DynamicArray<int> original(3);
    original.Set(0, 5);
    original.Set(1, 7);
    original.Set(2, 9);
    DynamicArray<int> copy(original);
    ASSERT_EQ(copy.GetSize(), 3u);
    EXPECT_EQ(copy.Get(0), 5);
    EXPECT_EQ(copy.Get(1), 7);
    EXPECT_EQ(copy.Get(2), 9);
}

TEST(DynamicArrayConstructors, Move) {
    DynamicArray<int> original(2);
    original.Set(0, 100);
    original.Set(1, 200);
    DynamicArray<int> moved(std::move(original));
    EXPECT_EQ(moved.GetSize(), 2u);
    EXPECT_EQ(moved.Get(0), 100);
    EXPECT_EQ(moved.Get(1), 200);
    EXPECT_EQ(original.GetSize(), 0u); // после перемещения размер 0
}

TEST(DynamicArray, GetSet) {
    DynamicArray<int> arr(3);
    arr.Set(0, 42);
    arr.Set(2, 99);
    EXPECT_EQ(arr.Get(0), 42);
    EXPECT_EQ(arr.Get(2), 99);
    EXPECT_THROW(arr.Get(3), IndexOutOfRangeException);
    EXPECT_THROW(arr.Set(3, 100), IndexOutOfRangeException);
}

TEST(DynamicArray, Append) {
    DynamicArray<int> arr;
    arr.Append(1);
    arr.Append(2);
    arr.Append(3);
    EXPECT_EQ(arr.GetSize(), 3u);
    EXPECT_EQ(arr.Get(0), 1);
    EXPECT_EQ(arr.Get(1), 2);
    EXPECT_EQ(arr.Get(2), 3);
}

TEST(DynamicArray, Resize) {
    DynamicArray<int> arr(2);
    arr.Set(0, 10);
    arr.Set(1, 20);
    arr.Resize(5);
    EXPECT_EQ(arr.GetSize(), 5u);
    EXPECT_EQ(arr.Get(0), 10);
    EXPECT_EQ(arr.Get(1), 20);
    // Новые элементы должны быть value-initialized (0 для int)
    EXPECT_EQ(arr.Get(2), 0);
    EXPECT_EQ(arr.Get(4), 0);
    arr.Resize(1);
    EXPECT_EQ(arr.GetSize(), 1u);
    EXPECT_EQ(arr.Get(0), 10);
}

TEST(DynamicArray, EnsureCapacity) {
    DynamicArray<int> arr;
    arr.EnsureCapacity(10);
    // После reserve можно добавить 10 элементов без реаллокации
    for (int i = 0; i < 10; ++i) arr.Append(i);
    EXPECT_EQ(arr.GetSize(), 10u);
    for (int i = 0; i < 10; ++i) EXPECT_EQ(arr.Get(i), i);
}