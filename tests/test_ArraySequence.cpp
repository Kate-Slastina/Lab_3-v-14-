#include <gtest/gtest.h>
#include <vector>
#include <sstream>
#include <type_traits>
#include <cmath>
#include "MutableArraySequence.hpp"
#include "LinkedList.hpp"

// infrastrucura

template <typename T>
struct SequenceContext {
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
::testing::AssertionResult AssertSequenceState(const SequenceContext<T>& ctx, const Sequence<T>& seq) {
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

template <typename T>
::testing::AssertionResult AssertValueEq(const SequenceContext<T>& ctx, T actual, T expected) {
    bool match = false;
    if constexpr (std::is_floating_point_v<T>) {
        match = std::abs(actual - expected) <= ctx.tolerance;
    } else {
        match = actual == expected;
    }
    if (match) return ::testing::AssertionSuccess();

    std::ostringstream msg;
    msg << "[VALUE MISMATCH]\n";
    msg << ctx.Format();
    msg << "Actual:      " << actual << "\n";
    msg << "Expected:    " << expected << "\n";
    return ::testing::AssertionFailure() << msg.str();
}

// fixt

class ArraySequenceTest : public ::testing::Test {
protected:
    void SetUp() override {
        ::testing::FLAGS_gtest_stack_trace_depth = 10;
    }
};

// конструкторы

TEST_F(ArraySequenceTest, Constructor_Default) {
    MutableArraySequence<int> seq;
    SequenceContext<int> ctx{"Constructor_Default", "Constructor()", {}};
    ASSERT_TRUE(AssertSequenceState(ctx, seq));
    
    SCOPED_TRACE(ctx.Format());
    EXPECT_THROW(seq.GetFirst(), IndexOutOfRangeException);
    EXPECT_THROW(seq.GetLast(), IndexOutOfRangeException);
}

TEST_F(ArraySequenceTest, Constructor_FromArray) {
    int items[] = {5, 6, 7};
    MutableArraySequence<int> seq(items, 3);
    SequenceContext<int> ctx{"Constructor_FromArray", "Constructor(arr, 3)", {5, 6, 7}};
    ASSERT_TRUE(AssertSequenceState(ctx, seq));
}

TEST_F(ArraySequenceTest, Constructor_FromLinkedList) {
    LinkedList<int> list;
    list.Append(10);
    list.Append(20);
    list.Append(30);
    MutableArraySequence<int> seq(list);
    SequenceContext<int> ctx{"Constructor_FromLinkedList", "Constructor(list)", {10, 20, 30}};
    ASSERT_TRUE(AssertSequenceState(ctx, seq));
}

TEST_F(ArraySequenceTest, Constructor_Copy) {
    MutableArraySequence<int> original;
    original.Append(1);
    original.Append(2);
    MutableArraySequence<int> copy(original);
    SequenceContext<int> ctx{"Constructor_Copy", "CopyConstructor()", {1, 2}};
    ASSERT_TRUE(AssertSequenceState(ctx, copy));
}

// аксессы

TEST_F(ArraySequenceTest, GetFirstLast) {
    MutableArraySequence<int> seq;
    seq.Append(100);
    seq.Append(200);
    seq.Append(300);
    
    SequenceContext<int> ctx{"GetFirstLast", "Append(100, 200, 300)", {100, 200, 300}};
    ASSERT_TRUE(AssertSequenceState(ctx, seq));
    EXPECT_EQ(seq.GetFirst(), 100);
    EXPECT_EQ(seq.GetLast(), 300);
    
    seq.RemoveAt(0);
    SequenceContext<int> ctx2{"GetFirstLast", "RemoveAt(0)", {200, 300}};
    ASSERT_TRUE(AssertSequenceState(ctx2, seq));
    EXPECT_EQ(seq.GetFirst(), 200);
    
    seq.RemoveAt(1);
    SequenceContext<int> ctx3{"GetFirstLast", "RemoveAt(1)", {200}};
    ASSERT_TRUE(AssertSequenceState(ctx3, seq));
    EXPECT_EQ(seq.GetFirst(), 200);
    EXPECT_EQ(seq.GetLast(), 200);
}

TEST_F(ArraySequenceTest, Get) {
    MutableArraySequence<int> seq;
    for (int i = 0; i < 5; ++i) seq.Append(i * 10);
    
    SequenceContext<int> ctx{"Get", "Append(0..40)", {0, 10, 20, 30, 40}};
    ASSERT_TRUE(AssertSequenceState(ctx, seq));
    
    SCOPED_TRACE(ctx.Format());
    EXPECT_THROW(seq.Get(5), IndexOutOfRangeException);
    EXPECT_THROW(seq.Get(100), IndexOutOfRangeException);
}

// муты

TEST_F(ArraySequenceTest, Append) {
    MutableArraySequence<int> seq;
    seq.Append(1);
    ASSERT_TRUE(AssertSequenceState({"Append", "Append(1)", {1}}, seq));
    
    seq.Append(2);
    ASSERT_TRUE(AssertSequenceState({"Append", "Append(2)", {1, 2}}, seq));
    
    seq.Append(3);
    ASSERT_TRUE(AssertSequenceState({"Append", "Append(3)", {1, 2, 3}}, seq));
}

TEST_F(ArraySequenceTest, Prepend) {
    MutableArraySequence<int> seq;
    seq.Prepend(10);
    ASSERT_TRUE(AssertSequenceState({"Prepend", "Prepend(10)", {10}}, seq));
    
    seq.Prepend(20);
    ASSERT_TRUE(AssertSequenceState({"Prepend", "Prepend(20)", {20, 10}}, seq));
    
    seq.Prepend(30);
    ASSERT_TRUE(AssertSequenceState({"Prepend", "Prepend(30)", {30, 20, 10}}, seq));
}

TEST_F(ArraySequenceTest, InsertAt) {
    MutableArraySequence<int> seq;
    seq.Append(1);
    seq.Append(3);
    
    seq.InsertAt(2, 1);
    SequenceContext<int> ctx{"InsertAt", "InsertAt(2, 1)", {1, 2, 3}};
    ASSERT_TRUE(AssertSequenceState(ctx, seq));
    
    seq.InsertAt(99, 0);
    ASSERT_TRUE(AssertSequenceState({"InsertAt", "InsertAt(99, 0)", {99, 1, 2, 3}}, seq));
    
    SCOPED_TRACE(ctx.Format());
    EXPECT_THROW(seq.InsertAt(99, 10), IndexOutOfRangeException);
}

TEST_F(ArraySequenceTest, RemoveAt) {
    MutableArraySequence<int> seq;
    for (int i = 1; i <= 5; ++i) seq.Append(i);
    
    seq.RemoveAt(2);
    SequenceContext<int> ctx{"RemoveAt", "RemoveAt(2)", {1, 2, 4, 5}};
    ASSERT_TRUE(AssertSequenceState(ctx, seq));
    
    seq.RemoveAt(0);
    ASSERT_TRUE(AssertSequenceState({"RemoveAt", "RemoveAt(0)", {2, 4, 5}}, seq));
    
    seq.RemoveAt(2);
    ASSERT_TRUE(AssertSequenceState({"RemoveAt", "RemoveAt(2)", {2, 4}}, seq));
    
    SCOPED_TRACE(ctx.Format());
    EXPECT_THROW(seq.RemoveAt(10), IndexOutOfRangeException);
    EXPECT_THROW(seq.RemoveAt(5), IndexOutOfRangeException);
}

// допы

TEST_F(ArraySequenceTest, Concat) {
    MutableArraySequence<int> seq1;
    seq1.Append(1);
    seq1.Append(2);
    
    MutableArraySequence<int> seq2;
    seq2.Append(3);
    seq2.Append(4);
    
    Sequence<int>* concat = seq1.Concat(&seq2);
    ASSERT_NE(concat, nullptr) << "Concat returned nullptr!";
    
    SequenceContext<int> ctx{"Concat", "Concat(seq2)", {1, 2, 3, 4}};
    ASSERT_TRUE(AssertSequenceState(ctx, *concat));
    
    delete concat;
}

TEST_F(ArraySequenceTest, GetSubsequence) {
    MutableArraySequence<int> seq;
    for (int i = 0; i < 10; ++i) seq.Append(i);
    
    Sequence<int>* sub = seq.GetSubsequence(2, 5);
    ASSERT_NE(sub, nullptr) << "GetSubsequence returned nullptr!";
    
    SequenceContext<int> ctx{"GetSubsequence", "GetSubsequence(2, 5)", {2, 3, 4, 5}};
    ASSERT_TRUE(AssertSequenceState(ctx, *sub));
    
    delete sub;
    
    SequenceContext<int> ctx_err{"GetSubsequence_Err", "GetSubsequence(5, 2)", {}};
    SCOPED_TRACE(ctx_err.Format());
    EXPECT_THROW(seq.GetSubsequence(5, 2), IndexOutOfRangeException);
    EXPECT_THROW(seq.GetSubsequence(10, 12), IndexOutOfRangeException);
}

// кастомы

TEST_F(ArraySequenceTest, Double_Precision) {
    MutableArraySequence<double> seq;
    seq.Append(1.5);
    seq.Append(2.7);
    seq.Append(3.14159);
    
    SequenceContext<double> ctx{"Double_Precision", "Append(double)", {1.5, 2.7, 3.14159}};
    ctx.tolerance = 1e-9;
    ASSERT_TRUE(AssertSequenceState(ctx, seq));
}

// нигативы

TEST_F(ArraySequenceTest, Get_OutOfBounds_ThrowsCorrectException) {
    MutableArraySequence<int> seq;
    seq.Append(10);
    seq.Append(20);
    
    SequenceContext<int> ctx{"Get_OutOfBounds", "Get(10)", {10, 20}};
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

TEST_F(ArraySequenceTest, RemoveAt_EmptySequence_Throws) {
    MutableArraySequence<int> seq;
    
    SequenceContext<int> ctx{"RemoveAt_Empty", "RemoveAt(0)", {}};
    SCOPED_TRACE(ctx.Format());
    
    try {
        seq.RemoveAt(0);
        FAIL() << ctx.Format() << "Expected exception not thrown on empty sequence.\n";
    } catch (const IndexOutOfRangeException& e) {
        SUCCEED() << "Correctly caught: " << e.what();
    } catch (const std::exception& e) {
        FAIL() << ctx.Format() << "Wrong exception type: " << typeid(e).name() 
               << " | Message: " << e.what() << "\n";
    }
}