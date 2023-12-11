#include "../../common/messages/Message.h"
#include "./LinkedList.h"
#include <iostream>

template <typename V>
LinkedList<V>::LinkedList() : LinkedList(nullptr, nullptr, 0) {}

template <typename V>
LinkedList<V>::LinkedList(Node<V> *head, Node<V> *tail, int size) : head(head), tail(tail), size(size) {}

template <typename V>
LinkedList<V>::~LinkedList()
{
    Node<V> *current = head;
    while (current != nullptr)
    {
        Node<V> *next = current->getNext();
        delete current;
        current = next;
    }
}

template <typename V>
Node<V> *LinkedList<V>::getHead() const { return head; }

template <typename V>
void LinkedList<V>::setHead(Node<V> *head) { this->head = head; }

template <typename V>
Node<V> *LinkedList<V>::getTail() const { return tail; }

template <typename V>
void LinkedList<V>::setTail(Node<V> *tail) { this->tail = tail; }

template <typename V>
void LinkedList<V>::add(V value)
{
    Node<V> *node = new Node<V>(value);
    if (head == nullptr)
    {
        head = node;
        tail = node;
    }
    else
    {
        tail->setNext(node);
        tail = node;
    }
    size++;
}

template <typename V>
V LinkedList<V>::get(V value)
{
    Node<V> *current = head;
    while (current != nullptr)
    {
        if (current->getValue() == value)
        {
            return current->getValue();
        }
        current = current->getNext();
    }
    return current->getValue();
}

template <typename V>
void LinkedList<V>::remove(V value)
{
    Node<V> *current = head;
    Node<V> *previous = nullptr;
    while (current != nullptr)
    {
        if (current->getValue() == value)
        {
            if (previous == nullptr)
            {
                head = current->getNext();
            }
            else
            {
                previous->setNext(current->getNext());
            }
            delete current;
            size--;
            return;
        }
        previous = current;
        current = current->getNext();
    }
}

template <typename V>
void LinkedList<V>::print()
{
    Node<V> *current = head;
    while (current != nullptr)
    {
        std::cout << current->getValue() << std::endl;
        current = current->getNext();
    }
}

template class LinkedList<int>;
template class LinkedList<Message>;