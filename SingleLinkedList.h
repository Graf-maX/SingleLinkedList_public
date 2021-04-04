#pragma once

#include <cassert>
#include <cstddef>
#include <utility>
#include <iterator>

template <typename Type>
class SingleLinkedList {
    // Узел списка
    struct Node;

    // Класс итератора
    template <typename ValueType>
    class BasicIterator;

public:
    // Конструкторы/деструкторы
    SingleLinkedList() = default;                           // Конструктор по умолчанию
    SingleLinkedList(std::initializer_list<Type> values);   // Конструктор на основе initializer_list
    SingleLinkedList(const SingleLinkedList& other);        // Копирующий конструктор
    ~SingleLinkedList();                                    // Деструктор

    // Методы класса
    [[nodiscard]] size_t GetSize() const noexcept;  // Возвращает количество элементов в списке за время O(1)
    [[nodiscard]] bool IsEmpty() const noexcept;    // Сообщает, пустой ли список за время O(1)
    void PushFront(const Type& value);              // Вставляет элемент value в начало списка за время O(1)
    void Clear() noexcept;                          // Очищает список за время O(N)
    void PopFront() noexcept;                       // Удаляет первый элемент списка
    void swap(SingleLinkedList& other) noexcept;    // Обменивает содержимое списков за время O(1)

    // Перегрузка операторов
    SingleLinkedList& operator=(const SingleLinkedList& rhs);

    // Объявление итераторов
    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;


    [[nodiscard]] Iterator begin() noexcept {return Iterator(head_.next_node);}
    [[nodiscard]] Iterator end() noexcept   {return Iterator{nullptr};}

    // Константные версии begin/end для обхода списка без возможности модификации его элементов
    [[nodiscard]] ConstIterator begin() const noexcept {return ConstIterator(head_.next_node);}
    [[nodiscard]] ConstIterator end() const noexcept {return ConstIterator{nullptr};}

    // Методы для удобного получения константных итераторов у неконстантного контейнера
    [[nodiscard]] ConstIterator cbegin() const noexcept {return ConstIterator(head_.next_node);}
    [[nodiscard]] ConstIterator cend() const noexcept {return ConstIterator{nullptr};}

    // Возвращают константный итератор, указывающий на позицию перед первым элементом односвязного списка.
    [[nodiscard]] Iterator before_begin() noexcept {return Iterator(&head_);}

    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        Node* temp = const_cast<Node*>(&head_);
        return ConstIterator(temp);
    }

    [[nodiscard]] ConstIterator before_begin() const noexcept {
        Node* temp = const_cast<Node*>(&head_);
        return ConstIterator(temp);
    }

    // Методы класса с возвратом итератора
    // Вставка элемента после pos
    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        Node* insert_node = new Node(value, pos.node_->next_node);
        pos.node_->next_node = insert_node;
        ++size_;
        return Iterator(insert_node);
    }

    // Удаление элемента после pos
    Iterator EraseAfter(ConstIterator pos) noexcept {
        Node* temp = pos.node_->next_node;
        pos.node_->next_node = temp->next_node;
        delete temp;
        --size_;
        return Iterator(pos.node_->next_node);
    }

private:
    // Фиктивный узел, используется для вставки "перед первым элементом"
    Node head_ = {};
    size_t size_ = 0;

    template<typename TList>
    void CopyList(TList& other);
};


// Конструктор класса SingleLinkedList на основе initializer_list
template <typename Type>
SingleLinkedList<Type>::SingleLinkedList(std::initializer_list<Type> values) {
    CopyList(values);
}

// Копирующий конструктор SingleLinkedList
template <typename Type>
SingleLinkedList<Type>::SingleLinkedList(const SingleLinkedList& other) {
    assert(size_ == 0 && head_.next_node == nullptr);
    CopyList(other);
}

// Деструктор SingleLinkedList
template <typename Type>
SingleLinkedList<Type>::~SingleLinkedList() {
    Clear();
}

// Возвращает количество элементов в списке за время O(1)
template <typename Type>
[[nodiscard]] size_t SingleLinkedList<Type>::GetSize() const noexcept {
    return size_;
}

// Сообщает, пустой ли список за время O(1)
template <typename Type>
[[nodiscard]] bool SingleLinkedList<Type>::IsEmpty() const noexcept {
    return size_ == 0;
}

// Вставляет элемент value в начало списка за время O(1)
template <typename Type>
void SingleLinkedList<Type>::PushFront(const Type& value) {
    head_.next_node = new Node(value, head_.next_node);
    ++size_;
}

// Очищает список за время O(N)
template <typename Type>
void SingleLinkedList<Type>::Clear() noexcept {
    while (head_.next_node != nullptr) {
        PopFront();
    }
}

// Удалить первый элемент
template <typename Type>
void SingleLinkedList<Type>::PopFront() noexcept {
    EraseAfter(before_begin());
}

// Обменивает содержимое списков за время O(1)
template <typename Type>
void SingleLinkedList<Type>::swap(SingleLinkedList& other) noexcept {
    std::swap(head_.next_node, other.head_.next_node);
    std::swap(size_, other.size_);
}

// Перегрузка оператора присвоения
template <typename Type>
SingleLinkedList<Type>& SingleLinkedList<Type>::operator=(const SingleLinkedList<Type>& rhs) {
    if (this == &rhs) {
        return *this;
    }
    SingleLinkedList tmp(rhs);
    swap(tmp);

    return *this;
}

template <typename Type>
template <typename TList>
void SingleLinkedList<Type>::CopyList(TList& other) {
    SingleLinkedList tmp;
    Iterator node_it = tmp.before_begin();
    for (const auto& val : other) {
        ++node_it = InsertAfter(node_it, val);
        ++tmp.size_;
    }
    swap(tmp);
}

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(rhs < lhs);
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return rhs < lhs;
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs < rhs);
}

// Узел списка
template <typename Type>
struct SingleLinkedList<Type>::Node {
    Node() = default;
    Node(const Type& val, Node* next)
        : value(val)
        , next_node(next) {
    }

    Type value;
    Node* next_node = nullptr;
};

template <typename Type>
template <typename ValueType>
class SingleLinkedList<Type>::BasicIterator {
    // Класс списка объявляется дружественным, чтобы из методов списка
    // был доступ к приватной области итератора
    friend class SingleLinkedList<Type>;

    // Конвертирующий конструктор итератора из указателя на узел списка
    explicit BasicIterator(Node* node) : node_(node) {
    }

public:
    // Объявленные ниже типы сообщают стандартной библиотеке о свойствах этого итератора

    // Категория итератора - forward iterator
    // (итератор, который поддерживает операции инкремента и многократное разыменование)
    using iterator_category = std::forward_iterator_tag;
    // Тип элементов, по которым перемещается итератор
    using value_type = Type;
    // Тип, используемый для хранения смещения между итераторами
    using difference_type = std::ptrdiff_t;
    // Тип указателя на итерируемое значение
    using pointer = ValueType*;
    // Тип ссылки на итерируемое значение
    using reference = ValueType&;

    // Конструкторы
    BasicIterator() = default;
    BasicIterator(const BasicIterator<Type>& other) noexcept;

    // Перегрузка операторов
    // Чтобы компилятор не выдавал предупреждение об отсутствии оператора = при наличии
    // пользовательского конструктора копирования, явно объявим оператор = и
    // попросим компилятор сгенерировать его за нас.
    BasicIterator& operator=(const BasicIterator& rhs) = default;
    [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept;
    [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept;
    [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept;
    [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept;

    // Оператор прединкремента. После его вызова итератор указывает на следующий элемент списка
    // Возвращает ссылку на самого себя
    // Инкремент итератора, не указывающего на существующий элемент списка, приводит к неопределённому поведению
    BasicIterator& operator++() noexcept {
        assert(node_ != nullptr);
        node_ = node_->next_node;
        return *this;
    }

    // Оператор постинкремента. После его вызова итератор указывает на следующий элемент списка.
    // Возвращает прежнее значение итератора
    // Инкремент итератора, не указывающего на существующий элемент списка,
    // приводит к неопределённому поведению
    BasicIterator operator++(int) noexcept {
        assert(node_ != nullptr);
        auto old_value(*this);
        ++(*this);
        return old_value;
    }

    // Операция разыменования. Возвращает ссылку на текущий элемент
    // Вызов этого оператора, у итератора, не указывающего на существующий элемент списка,
    // приводит к неопределённому поведению
    [[nodiscard]] reference operator*() const noexcept {
        return node_->value;
    }

    // Операция доступа к члену класса. Возвращает указатель на текущий элемент списка.
    // Вызов этого оператора, у итератора, не указывающего на существующий элемент списка,
    // приводит к неопределённому поведению
    [[nodiscard]] pointer operator->() const noexcept {
        Type* element = &(this->node_->value);
        return element;
    }

private:
    Node* node_ = nullptr;
};

// Конвертирующий конструктор/конструктор копирования BasicIterator
// При ValueType, совпадающем с Type, играет роль копирующего конструктора
// При ValueType, совпадающем с const Type, играет роль конвертирующего конструктора
template <typename Type>
template <typename ValueType>
SingleLinkedList<Type>::BasicIterator<ValueType>::BasicIterator(const BasicIterator<Type>& other) noexcept {
    node_ = other.node_;
}

// Оператор сравнения итераторов (в роли второго аргумента выступает константный итератор)
// Два итератора равны, если они ссылаются на один и тот же элемент списка, либо на end()
template <typename Type>
template <typename ValueType>
[[nodiscard]] bool SingleLinkedList<Type>::BasicIterator<ValueType>::operator==
              (const BasicIterator<const Type>& rhs) const noexcept {
    return node_ == rhs.node_;
}

// Оператор, проверки итераторов на неравенство
// Противоположен !=
template <typename Type>
template <typename ValueType>
[[nodiscard]] bool SingleLinkedList<Type>::BasicIterator<ValueType>::operator!=
              (const BasicIterator<const Type>& rhs) const noexcept {
    return node_ != rhs.node_;
}

// Оператор сравнения итераторов (в роли второго аргумента итератор)
// Два итератора равны, если они ссылаются на один и тот же элемент списка, либо на end()
template <typename Type>
template <typename ValueType>
[[nodiscard]] bool SingleLinkedList<Type>::BasicIterator<ValueType>::operator==
              (const BasicIterator<Type>& rhs) const noexcept {
    return node_ == rhs.node_;
}

// Оператор, проверки итераторов на неравенство
// Противоположен !=
template <typename Type>
template <typename ValueType>
[[nodiscard]] bool SingleLinkedList<Type>::BasicIterator<ValueType>::operator!=
              (const BasicIterator<Type>& rhs) const noexcept {
    return node_ != rhs.node_;
}
