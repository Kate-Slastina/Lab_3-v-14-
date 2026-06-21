#include <gtest/gtest.h>
#include <vector>
#include <sstream>
#include <type_traits>
#include <cmath>
#include "DynamicArray.hpp"

// инфраструктура(пояти замок собянина)

template <typename T>
struct ArrayContext {
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
::testing::AssertionResult AssertArrayState(const ArrayContext<T>& ctx, const DynamicArray<T>& arr) {
    if (arr.GetSize() != ctx.expected_data.size()) {
        return ::testing::AssertionFailure() 
            << "[SIZE MISMATCH]\n" << ctx.Format() 
            << "Actual Size: " << arr.GetSize() << "\n";
    }
    
    for (size_t i = 0; i < ctx.expected_data.size(); ++i) {
        T actual = arr.Get(i);
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

//фикса

class DynamicArrayTest : public ::testing::Test {
protected:
    void SetUp() override {
        ::testing::FLAGS_gtest_stack_trace_depth = 10;
    }
};

//констры

TEST_F(DynamicArrayTest, Constructor_Default) {
    DynamicArray<int> arr;
    ArrayContext<int> ctx{"Constructor_Default", "Constructor()", {}};
    ASSERT_TRUE(AssertArrayState(ctx, arr));
}

TEST_F(DynamicArrayTest, Constructor_FromSize) {
    DynamicArray<int> arr(5);
    ArrayContext<int> ctx{"Constructor_FromSize", "Constructor(5)", {0, 0, 0, 0, 0}};
    ASSERT_TRUE(AssertArrayState(ctx, arr));
    
    arr.Set(0, 10);
    arr.Set(4, 20);
    ArrayContext<int> ctx2{"Constructor_FromSize", "Set(0,10), Set(4,20)", {10, 0, 0, 0, 20}};
    ASSERT_TRUE(AssertArrayState(ctx2, arr));
    
    SCOPED_TRACE(ctx.Format());
    EXPECT_NO_THROW(arr.Get(0));
    EXPECT_NO_THROW(arr.Get(4));
}

TEST_F(DynamicArrayTest, Constructor_FromArray) {
    int items[] = {1, 2, 3};
    DynamicArray<int> arr(items, 3);
    ArrayContext<int> ctx{"Constructor_FromArray", "Constructor(arr, 3)", {1, 2, 3}};
    ASSERT_TRUE(AssertArrayState(ctx, arr));
}

TEST_F(DynamicArrayTest, Constructor_Copy) {
    DynamicArray<int> original(3);
    original.Set(0, 5);
    original.Set(1, 7);
    original.Set(2, 9);
    
    DynamicArray<int> copy(original);
    ArrayContext<int> ctx{"Constructor_Copy", "CopyConstructor()", {5, 7, 9}};
    ASSERT_TRUE(AssertArrayState(ctx, copy));
}

TEST_F(DynamicArrayTest, Constructor_Move) {
    DynamicArray<int> original(2);
    original.Set(0, 100);
    original.Set(1, 200);
    
    DynamicArray<int> moved(std::move(original));
    ArrayContext<int> ctx{"Constructor_Move", "MoveConstructor()", {100, 200}};
    ASSERT_TRUE(AssertArrayState(ctx, moved));
    
    // После перемещения оригинал должен быть пуст
    ArrayContext<int> ctx_original{"Constructor_Move", "Original after move", {}};
    ASSERT_TRUE(AssertArrayState(ctx_original, original));
}

//доступ

TEST_F(DynamicArrayTest, GetSet) {
    DynamicArray<int> arr(3);
    arr.Set(0, 42);
    arr.Set(2, 99);
    
    ArrayContext<int> ctx{"GetSet", "Set(0,42), Set(2,99)", {42, 0, 99}};
    ASSERT_TRUE(AssertArrayState(ctx, arr));
    
    SCOPED_TRACE(ctx.Format());
    EXPECT_THROW(arr.Get(3), IndexOutOfRangeException);
    EXPECT_THROW(arr.Set(3, 100), IndexOutOfRangeException);
}

//изменялки

TEST_F(DynamicArrayTest, Append) {
    DynamicArray<int> arr;
    
    arr.Append(1);
    ASSERT_TRUE(AssertArrayState({"Append", "Append(1)", {1}}, arr));
    
    arr.Append(2);
    ASSERT_TRUE(AssertArrayState({"Append", "Append(2)", {1, 2}}, arr));
    
    arr.Append(3);
    ASSERT_TRUE(AssertArrayState({"Append", "Append(3)", {1, 2, 3}}, arr));
}

TEST_F(DynamicArrayTest, Resize) {
    DynamicArray<int> arr(2);
    arr.Set(0, 10);
    arr.Set(1, 20);
    
    arr.Resize(5);
    ArrayContext<int> ctx{"Resize", "Resize(5)", {10, 20, 0, 0, 0}};
    ASSERT_TRUE(AssertArrayState(ctx, arr));
    
    arr.Resize(1);
    ArrayContext<int> ctx2{"Resize", "Resize(1)", {10}};
    ASSERT_TRUE(AssertArrayState(ctx2, arr));
}

TEST_F(DynamicArrayTest, EnsureCapacity) {
    DynamicArray<int> arr;
    arr.EnsureCapacity(10);
    
    for (int i = 0; i < 10; ++i) arr.Append(i);
    
    ArrayContext<int> ctx{"EnsureCapacity", "EnsureCapacity(10) + Append(0..9)", {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}};
    ASSERT_TRUE(AssertArrayState(ctx, arr));
}

// НИГАтивные тесты

TEST_F(DynamicArrayTest, Get_OutOfBounds_ThrowsCorrectException) {
    DynamicArray<int> arr(2);
    arr.Set(0, 10);
    arr.Set(1, 20);
    
    ArrayContext<int> ctx{"Get_OutOfBounds", "Get(10)", {10, 20}};
    SCOPED_TRACE(ctx.Format());
    
    try {
        arr.Get(10);
        FAIL() << ctx.Format() << "Expected IndexOutOfRangeException not thrown.\n";
    } catch (const IndexOutOfRangeException& e) {
        SUCCEED() << "Correctly caught IndexOutOfRangeException: " << e.what();
    } catch (const std::exception& e) {
        FAIL() << ctx.Format() << "Wrong exception type. Caught: " << typeid(e).name() 
               << " | Message: " << e.what() << "\n";
    }
}

TEST_F(DynamicArrayTest, Set_OutOfBounds_ThrowsCorrectException) {
    DynamicArray<int> arr(2);
    arr.Set(0, 10);
    arr.Set(1, 20);
    
    ArrayContext<int> ctx{"Set_OutOfBounds", "Set(10, 100)", {10, 20}};
    SCOPED_TRACE(ctx.Format());
    
    try {
        arr.Set(10, 100);
        FAIL() << ctx.Format() << "Expected IndexOutOfRangeException not thrown.\n";
    } catch (const IndexOutOfRangeException& e) {
        SUCCEED() << "Correctly caught IndexOutOfRangeException: " << e.what();
    } catch (const std::exception& e) {
        FAIL() << ctx.Format() << "Wrong exception type. Caught: " << typeid(e).name() 
               << " | Message: " << e.what() << "\n";
    }
}

// доп приколы

TEST_F(DynamicArrayTest, Double_Precision) {
    DynamicArray<double> arr;
    arr.Append(1.5);
    arr.Append(2.7);
    arr.Append(3.14159);
    
    ArrayContext<double> ctx{"Double_Precision", "Append(double)", {1.5, 2.7, 3.14159}};
    ctx.tolerance = 1e-9;
    ASSERT_TRUE(AssertArrayState(ctx, arr));
}
