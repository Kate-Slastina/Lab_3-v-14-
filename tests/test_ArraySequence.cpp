#include <gtest/gtest.h>
#include "MutableArraySequence.hpp"

TEST(ArraySequenceConstructors, Default) {
    MutableArraySequence<int> seq;
    EXPECT_EQ(seq.GetLength(), 0u);
    EXPECT_THROW(seq.GetFirst(), IndexOutOfRangeException);
    EXPECT_THROW(seq.GetLast(), IndexOutOfRangeException);
}

TEST(ArraySequenceConstructors, FromArray) {
    int items[] = {5,6,7};
    MutableArraySequence<int> seq(items, 3);
    ASSERT_EQ(seq.GetLength(), 3u);
    EXPECT_EQ(seq.GetFirst(), 5);
    EXPECT_EQ(seq.GetLast(), 7);
    EXPECT_EQ(seq.Get(1), 6);
}

TEST(ArraySequenceConstructors, FromLinkedList) {
    LinkedList<int> list;
    list.Append(10);
    list.Append(20);
    list.Append(30);
    MutableArraySequence<int> seq(list);
    ASSERT_EQ(seq.GetLength(), 3u);
    EXPECT_EQ(seq.Get(0), 10);
    EXPECT_EQ(seq.Get(1), 20);
    EXPECT_EQ(seq.Get(2), 30);
}

TEST(ArraySequenceConstructors, Copy) {
    MutableArraySequence<int> original;
    original.Append(1);
    original.Append(2);
    MutableArraySequence<int> copy(original);
    EXPECT_EQ(copy.GetLength(), 2u);
    EXPECT_EQ(copy.Get(0), 1);
    EXPECT_EQ(copy.Get(1), 2);
}

// ---------------------- Методы доступа ----------------------
TEST(ArraySequence, GetFirstLast) {
    MutableArraySequence<int> seq;
    seq.Append(100);
    seq.Append(200);
    seq.Append(300);
    EXPECT_EQ(seq.GetFirst(), 100);
    EXPECT_EQ(seq.GetLast(), 300);
    seq.RemoveAt(0);
    EXPECT_EQ(seq.GetFirst(), 200);
    seq.RemoveAt(1);
    EXPECT_EQ(seq.GetFirst(), 200);
    EXPECT_EQ(seq.GetLast(), 200);
}

TEST(ArraySequence, Get) {
    MutableArraySequence<int> seq;
    for (int i = 0; i < 5; ++i) seq.Append(i*10);
    for (size_t i = 0; i < seq.GetLength(); ++i)
        EXPECT_EQ(seq.Get(i), static_cast<int>(i*10));
    EXPECT_THROW(seq.Get(5), IndexOutOfRangeException);
}


TEST(ArraySequence, Append) {
    MutableArraySequence<int> seq;
    seq.Append(1);
    seq.Append(2);
    EXPECT_EQ(seq.GetLength(), 2u);
    EXPECT_EQ(seq.Get(0), 1);
    EXPECT_EQ(seq.Get(1), 2);
}

TEST(ArraySequence, Prepend) {
    MutableArraySequence<int> seq;
    seq.Prepend(10);
    seq.Prepend(20);
    EXPECT_EQ(seq.GetLength(), 2u);
    EXPECT_EQ(seq.Get(0), 20);
    EXPECT_EQ(seq.Get(1), 10);
}

TEST(ArraySequence, InsertAt) {
    MutableArraySequence<int> seq;
    seq.Append(1);
    seq.Append(3);
    seq.InsertAt(2, 1); // вставить 2 на позицию 1
    ASSERT_EQ(seq.GetLength(), 3u);
    EXPECT_EQ(seq.Get(0), 1);
    EXPECT_EQ(seq.Get(1), 2);
    EXPECT_EQ(seq.Get(2), 3);
    EXPECT_THROW(seq.InsertAt(99, 10), IndexOutOfRangeException);
}

TEST(ArraySequence, RemoveAt) {
    MutableArraySequence<int> seq;
    for (int i = 1; i <= 5; ++i) seq.Append(i);
    seq.RemoveAt(2); // удаляем 3 (индекс 2)
    ASSERT_EQ(seq.GetLength(), 4u);
    EXPECT_EQ(seq.Get(0), 1);
    EXPECT_EQ(seq.Get(1), 2);
    EXPECT_EQ(seq.Get(2), 4);
    EXPECT_EQ(seq.Get(3), 5);
    EXPECT_THROW(seq.RemoveAt(10), IndexOutOfRangeException);
}

TEST(ArraySequence, Concat) {
    MutableArraySequence<int> seq1;
    seq1.Append(1);
    seq1.Append(2);
    MutableArraySequence<int> seq2;
    seq2.Append(3);
    seq2.Append(4);
    Sequence<int>* concat = seq1.Concat(&seq2);
    ASSERT_EQ(concat->GetLength(), 4u);
    EXPECT_EQ(concat->Get(0), 1);
    EXPECT_EQ(concat->Get(1), 2);
    EXPECT_EQ(concat->Get(2), 3);
    EXPECT_EQ(concat->Get(3), 4);
    delete concat;
}

TEST(ArraySequence, GetSubsequence) {
    MutableArraySequence<int> seq;
    for (int i = 0; i < 10; ++i) seq.Append(i);
    Sequence<int>* sub = seq.GetSubsequence(2, 5);
    ASSERT_EQ(sub->GetLength(), 4u);
    EXPECT_EQ(sub->Get(0), 2);
    EXPECT_EQ(sub->Get(1), 3);
    EXPECT_EQ(sub->Get(2), 4);
    EXPECT_EQ(sub->Get(3), 5);
    delete sub;
    EXPECT_THROW(seq.GetSubsequence(5, 2), IndexOutOfRangeException);
    EXPECT_THROW(seq.GetSubsequence(10, 12), IndexOutOfRangeException);
}