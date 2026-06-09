template<class T>
Sequence<T>* ListSequence<T>::GetSubsequence(size_t startIndex, size_t endIndex) const {
    if (startIndex > endIndex)
        throw IndexOutOfRangeException("ListSequence", "GetSubsequence", "invalid range");
    LinkedList<T>* subList = data_->GetSubList(startIndex, endIndex);
    Sequence<T>* result = this->Clone();
    delete static_cast<ListSequence<T>*>(result)->data_;
    static_cast<ListSequence<T>*>(result)->data_ = subList;
    return result;
}

template<class T>
void ListSequence<T>::AppendInternal(T item) {
    data_->Append(item);
}

template<class T>
Sequence<T>* ListSequence<T>::Append(T item) {
    Sequence<T>* target = this->Instance();
    static_cast<ListSequence<T>*>(target)->AppendInternal(item);
    return target;
}

template<class T>
void ListSequence<T>::PrependInternal(T item) {
    data_->Prepend(item);
}

template<class T>
Sequence<T>* ListSequence<T>::Prepend(T item) {
    Sequence<T>* target = this->Instance();
    static_cast<ListSequence<T>*>(target)->PrependInternal(item);
    return target;
}

template<class T>
void ListSequence<T>::InsertAtInternal(T item, size_t index) {
    data_->InsertAt(item, index);
}

template<class T>
Sequence<T>* ListSequence<T>::InsertAt(T item, size_t index) {
    Sequence<T>* target = this->Instance();
    static_cast<ListSequence<T>*>(target)->InsertAtInternal(item, index);
    return target;
}

template<class T>
void ListSequence<T>::ConcatInternal(Sequence<T>* smth) {
    if (smth == nullptr) throw InvalidArgumentException("ListSequence", "Concat", "null sequence");
    size_t len = smth->GetLength();
    for (size_t i = 0; i < len; ++i) {
        data_->Append(smth->Get(i));
    }
}

template<class T>
Sequence<T>* ListSequence<T>::Concat(Sequence<T>* smth) {
    Sequence<T>* target = this->Clone();
    static_cast<ListSequence<T>*>(target)->ConcatInternal(smth);
    return target;
}

template<class T>
Sequence<T>* ListSequence<T>::RemoveAt(size_t index) {
    if (index >= data_->GetLength())
        throw IndexOutOfRangeException("ListSequence", "RemoveAt", "index out of range");
    Sequence<T>* target = this->Instance();
    ListSequence<T>* listTarget = static_cast<ListSequence<T>*>(target);
    LinkedList<T>* newList = new LinkedList<T>();
    size_t len = listTarget->data_->GetLength();
    for (size_t i = 0; i < len; ++i) {
        if (i != index)
            newList->Append(listTarget->data_->Get(i));
    }
    delete listTarget->data_;
    listTarget->data_ = newList;
    return target;
}