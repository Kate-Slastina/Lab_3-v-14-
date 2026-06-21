#include <gtest/gtest.h>
#include <vector>
#include <sstream>
#include <type_traits>
#include <cmath>
#include "Exceptions.hpp"
#include "LinkedList.hpp"

// инфра

template <typename T>
struct ListContext {
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
::testing::AssertionResult AssertListState(const ListContext<T>& ctx, const LinkedList<T>& list) {
    if (list.GetLength() != ctx.expected_data.size()) {
        return ::testing::AssertionFailure() 
            << "[SIZE MISMATCH]\n" << ctx.Format() 
            << "Actual Size: " << list.GetLength() << "\n";
    }
    
    for (size_t i = 0; i < ctx.expected_data.size(); ++i) {
        T actual = list.Get(i);
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

// фикс

class LinkedListTest : public ::testing::Test {
protected:
    void SetUp() override {
        ::testing::FLAGS_gtest_stack_trace_depth = 10;
    }
};

// констры

TEST_F(LinkedListTest, Constructor_Default) {
    LinkedList<int> list;
    ListContext<int> ctx{"Constructor_Default", "Constructor()", {}};
    ASSERT_TRUE(AssertListState(ctx, list));
    
    SCOPED_TRACE(ctx.Format());
    EXPECT_THROW(list.GetFirst(), IndexOutOfRangeException);
    EXPECT_THROW(list.GetLast(), IndexOutOfRangeException);
}

TEST_F(LinkedListTest, Constructor_FromArray) {
    int items[] = {11, 22, 33};
    LinkedList<int> list(items, 3);
    ListContext<int> ctx{"Constructor_FromArray", "Constructor(arr, 3)", {11, 22, 33}};
    ASSERT_TRUE(AssertListState(ctx, list));
    
    EXPECT_EQ(list.GetFirst(), 11);
    EXPECT_EQ(list.GetLast(), 33);
}

TEST_F(LinkedListTest, Constructor_Copy) {
    LinkedList<int> original;
    original.Append(1);
    original.Append(2);
    
    LinkedList<int> copy(original);
    ListContext<int> ctx{"Constructor_Copy", "CopyConstructor()", {1, 2}};
    ASSERT_TRUE(AssertListState(ctx, copy));
}

TEST_F(LinkedListTest, Constructor_Move) {
    LinkedList<int> original;
    original.Append(100);
    original.Append(200);
    
    LinkedList<int> moved(std::move(original));
    ListContext<int> ctx{"Constructor_Move", "MoveConstructor()", {100, 200}};
    ASSERT_TRUE(AssertListState(ctx, moved));
    ListContext<int> ctx_original{"Constructor_Move", "Original after move", {}};
    ASSERT_TRUE(AssertListState(ctx_original, original));
}

// муты

TEST_F(LinkedListTest, AppendPrepend) {
    LinkedList<int> list;
    
    list.Append(1);
    ASSERT_TRUE(AssertListState({"AppendPrepend", "Append(1)", {1}}, list));
    
    list.Prepend(0);
    ASSERT_TRUE(AssertListState({"AppendPrepend", "Prepend(0)", {0, 1}}, list));
    
    list.Append(2);
    ASSERT_TRUE(AssertListState({"AppendPrepend", "Append(2)", {0, 1, 2}}, list));
}

TEST_F(LinkedListTest, InsertAt) {
    LinkedList<int> list;
    list.Append(1);
    list.Append(3);
    
    list.InsertAt(2, 1);
    ListContext<int> ctx{"InsertAt", "InsertAt(2, 1)", {1, 2, 3}};
    ASSERT_TRUE(AssertListState(ctx, list));
    
    SCOPED_TRACE(ctx.Format());
    EXPECT_THROW(list.InsertAt(99, 10), IndexOutOfRangeException);
}

// саблисты

TEST_F(LinkedListTest, GetSubList) {
    LinkedList<int> list;
    for (int i = 0; i < 10; ++i) list.Append(i);
    
    LinkedList<int>* sub = list.GetSubList(3, 6);
    ASSERT_NE(sub, nullptr) << "GetSubList returned nullptr!";
    
    ListContext<int> ctx{"GetSubList", "GetSubList(3, 6)", {3, 4, 5, 6}};
    ASSERT_TRUE(AssertListState(ctx, *sub));
    
    delete sub;
    
    ListContext<int> ctx_err{"GetSubList_Err", "GetSubList(6, 3)", {}};
    SCOPED_TRACE(ctx_err.Format());
    EXPECT_THROW(list.GetSubList(6, 3), IndexOutOfRangeException);
    EXPECT_THROW(list.GetSubList(10, 12), IndexOutOfRangeException);
}

TEST_F(LinkedListTest, Concat) {
    LinkedList<int> list1;
    list1.Append(1);
    list1.Append(2);
    
    LinkedList<int> list2;
    list2.Append(3);
    list2.Append(4);
    
    LinkedList<int>* combined = list1.Concat(&list2);
    ASSERT_NE(combined, nullptr) << "Concat returned nullptr!";
    
    ListContext<int> ctx{"Concat", "Concat(list2)", {1, 2, 3, 4}};
    ASSERT_TRUE(AssertListState(ctx, *combined));
    
    delete combined;
}

// НИГАтив

TEST_F(LinkedListTest, Get_OutOfBounds_ThrowsCorrectException) {
    LinkedList<int> list;
    list.Append(10);
    list.Append(20);
    
    ListContext<int> ctx{"Get_OutOfBounds", "Get(10)", {10, 20}};
    SCOPED_TRACE(ctx.Format());
    
    try {
        list.Get(10);
        FAIL() << ctx.Format() << "Expected IndexOutOfRangeException not thrown.\n";
    } catch (const IndexOutOfRangeException& e) {
        SUCCEED() << "Correctly caught IndexOutOfRangeException: " << e.what();
    } catch (const std::exception& e) {
        FAIL() << ctx.Format() << "Wrong exception type. Caught: " << typeid(e).name() 
               << " | Message: " << e.what() << "\n";
    }
}

TEST_F(LinkedListTest, InsertAt_OutOfBounds_ThrowsCorrectException) {
    LinkedList<int> list;
    list.Append(10);
    list.Append(20);
    
    ListContext<int> ctx{"InsertAt_OutOfBounds", "InsertAt(99, 10)", {10, 20}};
    SCOPED_TRACE(ctx.Format());
    
    try {
        list.InsertAt(99, 10);
        FAIL() << ctx.Format() << "Expected IndexOutOfRangeException not thrown.\n";
    } catch (const IndexOutOfRangeException& e) {
        SUCCEED() << "Correctly caught IndexOutOfRangeException: " << e.what();
    } catch (const std::exception& e) {
        FAIL() << ctx.Format() << "Wrong exception type. Caught: " << typeid(e).name() 
               << " | Message: " << e.what() << "\n";
    }
}

TEST_F(LinkedListTest, GetSubList_InvalidRange_ThrowsCorrectException) {
    LinkedList<int> list;
    list.Append(1);
    list.Append(2);
    list.Append(3);
    
    ListContext<int> ctx{"GetSubList_InvalidRange", "GetSubList(5, 2)", {1, 2, 3}};
    SCOPED_TRACE(ctx.Format());
    
    try {
        LinkedList<int>* sub = list.GetSubList(5, 2);
        delete sub;
        FAIL() << ctx.Format() << "Expected IndexOutOfRangeException not thrown.\n";
    } catch (const IndexOutOfRangeException& e) {
        SUCCEED() << "Correctly caught IndexOutOfRangeException: " << e.what();
    } catch (const std::exception& e) {
        FAIL() << ctx.Format() << "Wrong exception type. Caught: " << typeid(e).name() 
               << " | Message: " << e.what() << "\n";
    }
}

// плавки

TEST_F(LinkedListTest, Double_Precision) {
    LinkedList<double> list;
    list.Append(1.5);
    list.Append(2.7);
    list.Append(3.14159);
    
    ListContext<double> ctx{"Double_Precision", "Append(double)", {1.5, 2.7, 3.14159}};
    ctx.tolerance = 1e-9;
    ASSERT_TRUE(AssertListState(ctx, list));
}
