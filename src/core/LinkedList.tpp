template <class T>
LinkedList<T>::LinkedList(T* items, size_t count) : head_(nullptr), tail_(nullptr) {
    for (size_t i = 0; i < count; ++i) Append(items[i]);
}

template <class T>
LinkedList<T>::LinkedList(const LinkedList<T>& other) : head_(nullptr), tail_(nullptr) {
    Node_* current = other.head_;
    while (current) {
        Append(current->data);
        current = current->next;
    }
}

template <class T>
LinkedList<T>::LinkedList(LinkedList&& other) 
    : head_(other.head_), tail_(other.tail_) {
    other.head_ = other.tail_ = nullptr;
}

template <class T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList& other) {
    if (this != &other) {
        clear();
        Node_* current = other.head_;
        while (current) {
            Append(current->data);
            current = current->next;
        }
    }
    return *this;
}

template <class T>
LinkedList<T>& LinkedList<T>::operator=(LinkedList&& other) {
    if (this != &other) {
        clear();
        head_ = other.head_;
        tail_ = other.tail_;
        other.head_ = other.tail_ = nullptr;
    }
    return *this;
}

template<class T>
T LinkedList<T>::GetFirst() const {
    if (!head_) throw IndexOutOfRangeException("LinkedList", "GetFirst", "empty list");
    return head_->data;
}

template<class T>
T LinkedList<T>::GetLast() const {
    if (!head_) throw IndexOutOfRangeException("LinkedList", "GetLast", "empty list");
    return tail_->data;
}

template<class T>
T LinkedList<T>::Get(size_t index) const {
    Node_* current = head_;
    for (size_t i = 0; i < index && current; ++i) current = current->next;
    if (!current) throw IndexOutOfRangeException("LinkedList", "Get", "index out of range");
    return current->data;
}

template<class T>
size_t LinkedList<T>::GetLength() const {
    size_t count = 0;
    Node_* current = head_;
    while (current) { ++count; current = current->next; }
    return count;
}

template<class T>
LinkedList<T>* LinkedList<T>::GetSubList(size_t startIndex, size_t endIndex) const {
    size_t len = GetLength();
    if (startIndex >= len || endIndex >= len || startIndex > endIndex)
        throw IndexOutOfRangeException("LinkedList", "GetSubList", "invalid range");
    LinkedList<T>* result = new LinkedList<T>();
    Node_* current = head_;
    for (size_t i = 0; i < startIndex; ++i) current = current->next;
    for (size_t i = startIndex; i <= endIndex; ++i) {
        result->Append(current->data);
        current = current->next;
    }
    return result;
}

template<class T>
void LinkedList<T>::Append(T item) {
    Node_* newNode = new Node_(item);
    if (!head_) head_ = tail_ = newNode;
    else {
        tail_->next = newNode;
        tail_ = newNode;
    }
}

template<class T>
void LinkedList<T>::Prepend(T item) {
    Node_* newNode = new Node_(item);
    if (!head_) head_ = tail_ = newNode;
    else {
        newNode->next = head_;
        head_ = newNode;
    }
}

template<class T>
void LinkedList<T>::InsertAt(T item, size_t index) {
    size_t len = GetLength();
    if (index > len) throw IndexOutOfRangeException("LinkedList", "InsertAt", "index > length");
    if (index == 0) { Prepend(item); return; }
    if (index == len) { Append(item); return; }
    Node_* current = head_;
    for (size_t i = 0; i < index - 1; ++i) current = current->next;
    Node_* newNode = new Node_(item);
    newNode->next = current->next;
    current->next = newNode;
}

template<class T>
LinkedList<T>* LinkedList<T>::Concat(LinkedList<T>* list) {
    if (!list) return new LinkedList<T>(*this);
    LinkedList<T>* result = new LinkedList<T>(*this);
    Node_* current = list->head_;
    while (current) {
        result->Append(current->data);
        current = current->next;
    }
    return result;
}