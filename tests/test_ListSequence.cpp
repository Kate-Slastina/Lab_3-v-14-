#include <gtest/gtest.h>
#include <vector>
#include <sstream>
#include <type_traits>
#include <cmath>
#include "core/MutableListSequence.hpp"
#include "core/LinkedList.hpp"

template <typename T>
struct ListSequenceContext {
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
::testing::AssertionResult AssertListSequenceState(const ListSequenceContext<T>& ctx, const Sequence<T>& seq) {
    if (seq.GetLength() != ctx.expected_data.size()) {
        return ::testing::AssertionFailure() 
            << "[SIZE MISMATCH]\n" << ctx.Format() 
            << "Actual Size: " << seq.GetLength() << "\n";
    }
    
    for (size_t i = 0; i < ctx.expected_data.size(); ++i) {
        T actual = seq.Get(i);
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

// фикса

class ListSequenceTest : public ::testing::Test {
protected:
    void SetUp() override {
        ::testing::FLAGS_gtest_stack_trace_depth = 10;
    }
};

// констры

TEST_F(ListSequenceTest, Constructor_Default) {
    MutableListSequence<int> seq;
    ListSequenceContext<int> ctx{"Constructor_Default", "Constructor()", {}};
    ASSERT_TRUE(AssertListSequenceState(ctx, seq));
    
    SCOPED_TRACE(ctx.Format());
    EXPECT_THROW(seq.GetFirst(), IndexOutOfRangeException);
    EXPECT_THROW(seq.GetLast(), IndexOutOfRangeException);
}

TEST_F(ListSequenceTest, Constructor_FromArray) {
    int items[] = {1, 3, 5};
    MutableListSequence<int> seq(items, 3);
    ListSequenceContext<int> ctx{"Constructor_FromArray", "Constructor(arr, 3)", {1, 3, 5}};
    ASSERT_TRUE(AssertListSequenceState(ctx, seq));
    
    EXPECT_EQ(seq.GetFirst(), 1);
    EXPECT_EQ(seq.GetLast(), 5);
}

TEST_F(ListSequenceTest, Constructor_FromLinkedList) {
    LinkedList<int> list;
    list.Append(7);
    list.Append(8);
    list.Append(9);
    MutableListSequence<int> seq(list);
    ListSequenceContext<int> ctx{"Constructor_FromLinkedList", "Constructor(list)", {7, 8, 9}};
    ASSERT_TRUE(AssertListSequenceState(ctx, seq));
}

// муты

TEST_F(ListSequenceTest, AppendPrependInsertRemove) {
    MutableListSequence<int> seq;
    
    seq.Append(10);
    ASSERT_TRUE(AssertListSequenceState({"AppendPrependInsertRemove", "Append(10)", {10}}, seq));
    
    seq.Prepend(5);
    ASSERT_TRUE(AssertListSequenceState({"AppendPrependInsertRemove", "Prepend(5)", {5, 10}}, seq));
    
    seq.InsertAt(7, 1);
    ListSequenceContext<int> ctx1{"AppendPrependInsertRemove", "InsertAt(7, 1)", {5, 7, 10}};
    ASSERT_TRUE(AssertListSequenceState(ctx1, seq));
    
    seq.RemoveAt(1);
    ListSequenceContext<int> ctx2{"AppendPrependInsertRemove", "RemoveAt(1)", {5, 10}};
    ASSERT_TRUE(AssertListSequenceState(ctx2, seq));
}

// конкат и саб
TEST_F(ListSequenceTest, Concat) {
    MutableListSequence<int> seq1;
    seq1.Append(1);
    seq1.Append(2);
    
    MutableListSequence<int> seq2;
    seq2.Append(3);
    
    Sequence<int>* concat = seq1.Concat(&seq2);
    ASSERT_NE(concat, nullptr) << "Concat returned nullptr!";
    
    ListSequenceContext<int> ctx{"Concat", "Concat(seq2)", {1, 2, 3}};
    ASSERT_TRUE(AssertListSequenceState(ctx, *concat));
    
    delete concat;
}

TEST_F(ListSequenceTest, GetSubsequence) {
    MutableListSequence<int> seq;
    for (int i = 0; i < 10; ++i) seq.Append(i);
    
    Sequence<int>* sub = seq.GetSubsequence(3, 6);
    ASSERT_NE(sub, nullptr) << "GetSubsequence returned nullptr!";
    
    ListSequenceContext<int> ctx{"GetSubsequence", "GetSubsequence(3, 6)", {3, 4, 5, 6}};
    ASSERT_TRUE(AssertListSequenceState(ctx, *sub));
    
    delete sub;
    
    ListSequenceContext<int> ctx_err{"GetSubsequence_Err", "GetSubsequence(6, 3)", {}};
    SCOPED_TRACE(ctx_err.Format());
    EXPECT_THROW(seq.GetSubsequence(6, 3), IndexOutOfRangeException);
    EXPECT_THROW(seq.GetSubsequence(10, 12), IndexOutOfRangeException);
}

// НИГАтив

TEST_F(ListSequenceTest, Get_OutOfBounds_ThrowsCorrectException) {
    MutableListSequence<int> seq;
    seq.Append(10);
    seq.Append(20);
    
    ListSequenceContext<int> ctx{"Get_OutOfBounds", "Get(10)", {10, 20}};
    SCOPED_TRACE(ctx.Format());
    
    try {
        seq.Get(10);
        FAIL() << ctx.Format() << "Expected IndexOutOfRangeException not thrown.\n";
    } catch (const IndexOutOfRangeException& e) {
        SUCCEED() << "Correctly caught IndexOutOfRangeException: " << e.what();
    } catch (const std::exception& e) {
        FAIL() << ctx.Format() << "Wrong exception type. Caught: " << typeid(e).name() 
               << " | Message: " << e.what() << "\n";
    }
}

TEST_F(ListSequenceTest, RemoveAt_OutOfBounds_ThrowsCorrectException) {
    MutableListSequence<int> seq;
    seq.Append(10);
    seq.Append(20);
    
    ListSequenceContext<int> ctx{"RemoveAt_OutOfBounds", "RemoveAt(10)", {10, 20}};
    SCOPED_TRACE(ctx.Format());
    
    try {
        seq.RemoveAt(10);
        FAIL() << ctx.Format() << "Expected IndexOutOfRangeException not thrown.\n";
    } catch (const IndexOutOfRangeException& e) {
        SUCCEED() << "Correctly caught IndexOutOfRangeException: " << e.what();
    } catch (const std::exception& e) {
        FAIL() << ctx.Format() << "Wrong exception type. Caught: " << typeid(e).name() 
               << " | Message: " << e.what() << "\n";
    }
}

TEST_F(ListSequenceTest, InsertAt_OutOfBounds_ThrowsCorrectException) {
    MutableListSequence<int> seq;
    seq.Append(10);
    seq.Append(20);
    
    ListSequenceContext<int> ctx{"InsertAt_OutOfBounds", "InsertAt(99, 10)", {10, 20}};
    SCOPED_TRACE(ctx.Format());
    
    try {
        seq.InsertAt(99, 10);
        FAIL() << ctx.Format() << "Expected IndexOutOfRangeException not thrown.\n";
    } catch (const IndexOutOfRangeException& e) {
        SUCCEED() << "Correctly caught IndexOutOfRangeException: " << e.what();
    } catch (const std::exception& e) {
        FAIL() << ctx.Format() << "Wrong exception type. Caught: " << typeid(e).name() 
               << " | Message: " << e.what() << "\n";
    }
}

TEST_F(ListSequenceTest, GetSubsequence_InvalidRange_ThrowsCorrectException) {
    MutableListSequence<int> seq;
    seq.Append(1);
    seq.Append(2);
    seq.Append(3);
    
    ListSequenceContext<int> ctx{"GetSubsequence_InvalidRange", "GetSubsequence(5, 2)", {1, 2, 3}};
    SCOPED_TRACE(ctx.Format());
    
    try {
        Sequence<int>* sub = seq.GetSubsequence(5, 2);
        delete sub;
        FAIL() << ctx.Format() << "Expected IndexOutOfRangeException not thrown.\n";
    } catch (const IndexOutOfRangeException& e) {
        SUCCEED() << "Correctly caught IndexOutOfRangeException: " << e.what();
    } catch (const std::exception& e) {
        FAIL() << ctx.Format() << "Wrong exception type. Caught: " << typeid(e).name() 
               << " | Message: " << e.what() << "\n";
    }
}

//  плавки

TEST_F(ListSequenceTest, Double_Precision) {
    MutableListSequence<double> seq;
    seq.Append(1.5);
    seq.Append(2.7);
    seq.Append(3.14159);
    
    ListSequenceContext<double> ctx{"Double_Precision", "Append(double)", {1.5, 2.7, 3.14159}};
    ctx.tolerance = 1e-9;
    ASSERT_TRUE(AssertListSequenceState(ctx, seq));
}

