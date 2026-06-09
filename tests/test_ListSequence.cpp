#include <gtest/gtest.h>
#include "core/MutableListSequence.hpp"

TEST(ListSequenceConstructors, Default) {
    MutableListSequence<int> seq;
    EXPECT_EQ(seq.GetLength(), 0u);
    EXPECT_THROW(seq.GetFirst(), IndexOutOfRangeException);
}

TEST(ListSequenceConstructors, FromArray) {
    int items[] = {1,3,5};
    MutableListSequence<int> seq(items, 3);
    ASSERT_EQ(seq.GetLength(), 3u);
    EXPECT_EQ(seq.GetFirst(), 1);
    EXPECT_EQ(seq.GetLast(), 5);
    EXPECT_EQ(seq.Get(1), 3);
}

TEST(ListSequenceConstructors, FromLinkedList) {
    LinkedList<int> list;
    list.Append(7);
    list.Append(8);
    list.Append(9);
    MutableListSequence<int> seq(list);
    ASSERT_EQ(seq.GetLength(), 3u);
    EXPECT_EQ(seq.Get(0), 7);
    EXPECT_EQ(seq.Get(1), 8);
    EXPECT_EQ(seq.Get(2), 9);
}

TEST(ListSequence, AppendPrependInsertRemove) {
    MutableListSequence<int> seq;
    seq.Append(10);
    seq.Prepend(5);
    seq.InsertAt(7, 1);
    EXPECT_EQ(seq.GetLength(), 3u);
    EXPECT_EQ(seq.Get(0), 5);
    EXPECT_EQ(seq.Get(1), 7);
    EXPECT_EQ(seq.Get(2), 10);
    seq.RemoveAt(1);
    EXPECT_EQ(seq.GetLength(), 2u);
    EXPECT_EQ(seq.Get(0), 5);
    EXPECT_EQ(seq.Get(1), 10);
}

TEST(ListSequence, Concat) {
    MutableListSequence<int> seq1;
    seq1.Append(1);
    seq1.Append(2);
    MutableListSequence<int> seq2;
    seq2.Append(3);
    Sequence<int>* concat = seq1.Concat(&seq2);
    ASSERT_EQ(concat->GetLength(), 3u);
    EXPECT_EQ(concat->Get(0), 1);
    EXPECT_EQ(concat->Get(1), 2);
    EXPECT_EQ(concat->Get(2), 3);
    delete concat;
}

TEST(ListSequence, GetSubsequence) {
    MutableListSequence<int> seq;
    for (int i = 0; i < 10; ++i) seq.Append(i);
    Sequence<int>* sub = seq.GetSubsequence(3, 6);
    ASSERT_EQ(sub->GetLength(), 4u);
    EXPECT_EQ(sub->Get(0), 3);
    EXPECT_EQ(sub->Get(1), 4);
    EXPECT_EQ(sub->Get(2), 5);
    EXPECT_EQ(sub->Get(3), 6);
    delete sub;
    EXPECT_THROW(seq.GetSubsequence(6, 3), IndexOutOfRangeException);
}