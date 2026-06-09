#pragma once
#include <Exceptions.hpp>

template <class T>
class LinkedList {
private:
    struct Node_ {
        T data;
        Node_* next = nullptr;
        Node_(T val) : data(val) {}
    };
    Node_* head_;
    Node_* tail_;

    void clear() {
        Node_* current = head_;
        while (current) {
            Node_* next = current->next;
            delete current;
            current = next;
        }
        head_ = tail_ = nullptr;
    }

public:
    LinkedList() : head_(nullptr), tail_(nullptr) {}
    LinkedList(T* items, size_t count);
    LinkedList(const LinkedList<T>& other);
    LinkedList(LinkedList&& other);
    ~LinkedList() { clear(); }

    LinkedList& operator=(const LinkedList& other);
    LinkedList& operator=(LinkedList&& other);

    T GetFirst() const;
    T GetLast() const;
    T Get(size_t index) const;
    size_t GetLength() const;
    LinkedList<T>* GetSubList(size_t startIndex, size_t endIndex) const;

    void Append(T item);
    void Prepend(T item);
    void InsertAt(T item, size_t index);
    LinkedList<T>* Concat(LinkedList<T>* list);
};

#include "LinkedList.tpp"