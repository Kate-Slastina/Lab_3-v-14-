#include <gtest/gtest.h>
#include "Exceptions.hpp"
#include "LinkedList.hpp"

TEST(LinkedListConstructors, Default) {
    LinkedList<int> list;
    EXPECT_THROW(list.GetFirst(), IndexOutOfRangeException);
    EXPECT_EQ(list.GetLength(), 0u);
}

TEST(LinkedListConstructors, FromArray) {
    int items[] = {11,22,33};
    LinkedList<int> list(items, 3);
    ASSERT_EQ(list.GetLength(), 3u);
    EXPECT_EQ(list.GetFirst(), 11);
    EXPECT_EQ(list.GetLast(), 33);
    EXPECT_EQ(list.Get(1), 22);
}

TEST(LinkedListConstructors, Copy) {
    LinkedList<int> original;
    original.Append(1);
    original.Append(2);
    LinkedList<int> copy(original);
    EXPECT_EQ(copy.GetLength(), 2u);
    EXPECT_EQ(copy.Get(0), 1);
    EXPECT_EQ(copy.Get(1), 2);
}

TEST(LinkedListConstructors, Move) {
    LinkedList<int> original;
    original.Append(100);
    original.Append(200);
    LinkedList<int> moved(std::move(original));
    EXPECT_EQ(moved.GetLength(), 2u);
    EXPECT_EQ(moved.Get(0), 100);
    EXPECT_EQ(original.GetLength(), 0u);
}

TEST(LinkedList, AppendPrepend) {
    LinkedList<int> list;
    list.Append(1);
    list.Prepend(0);
    list.Append(2);
    EXPECT_EQ(list.GetLength(), 3u);
    EXPECT_EQ(list.Get(0), 0);
    EXPECT_EQ(list.Get(1), 1);
    EXPECT_EQ(list.Get(2), 2);
}

TEST(LinkedList, InsertAt) {
    LinkedList<int> list;
    list.Append(1);
    list.Append(3);
    list.InsertAt(2, 1);
    ASSERT_EQ(list.GetLength(), 3u);
    EXPECT_EQ(list.Get(0), 1);
    EXPECT_EQ(list.Get(1), 2);
    EXPECT_EQ(list.Get(2), 3);
    EXPECT_THROW(list.InsertAt(99, 10), IndexOutOfRangeException);
}

TEST(LinkedList, GetSubList) {
    LinkedList<int> list;
    for (int i = 0; i < 10; ++i) list.Append(i);
    LinkedList<int>* sub = list.GetSubList(3, 6);
    ASSERT_EQ(sub->GetLength(), 4u);
    EXPECT_EQ(sub->Get(0), 3);
    EXPECT_EQ(sub->Get(1), 4);
    EXPECT_EQ(sub->Get(2), 5);
    EXPECT_EQ(sub->Get(3), 6);
    delete sub;
    EXPECT_THROW(list.GetSubList(6, 3), IndexOutOfRangeException);
}

TEST(LinkedList, Concat) {
    LinkedList<int> list1;
    list1.Append(1);
    list1.Append(2);
    LinkedList<int> list2;
    list2.Append(3);
    list2.Append(4);
    LinkedList<int>* combined = list1.Concat(&list2);
    ASSERT_EQ(combined->GetLength(), 4u);
    EXPECT_EQ(combined->Get(0), 1);
    EXPECT_EQ(combined->Get(1), 2);
    EXPECT_EQ(combined->Get(2), 3);
    EXPECT_EQ(combined->Get(3), 4);
    delete combined;
}