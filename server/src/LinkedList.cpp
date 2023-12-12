#include "../../common/messages/Message.h"
#include "./LinkedList.h"
#include <iostream>

/**
 * Default constructor for LinkedList.
 * Initializes an empty LinkedList.
 */
template <typename V>
LinkedList<V>::LinkedList() : LinkedList(nullptr, nullptr, 0) {}

/**
 * Constructor for LinkedList with specified head, tail, and size.
 * Initializes the LinkedList with the provided head, tail, and size.
 *
 * @param head Pointer to the first node of the list.
 * @param tail Pointer to the last node of the list.
 * @param size The size of the list.
 */
template <typename V>
LinkedList<V>::LinkedList(Node<V> *head, Node<V> *tail, int size) : head(head), tail(tail), size(size) {}

/**
 * Destructor for LinkedList.
 * Deletes all nodes in the list to free up memory.
 */
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

/**
 * Gets the head node of the list.
 *
 * @return Pointer to the first node of the list.
 */
template <typename V>
Node<V> *LinkedList<V>::getHead() const { return head; }

/**
 * Sets the head node of the list.
 *
 * @param head Pointer to the new first node of the list.
 */
template <typename V>
void LinkedList<V>::setHead(Node<V> *head) { this->head = head; }

/**
 * Gets the tail node of the list.
 *
 * @return Pointer to the last node of the list.
 */
template <typename V>
Node<V> *LinkedList<V>::getTail() const { return tail; }

/**
 * Sets the tail node of the list.
 *
 * @param tail Pointer to the new last node of the list.
 */
template <typename V>
void LinkedList<V>::setTail(Node<V> *tail) { this->tail = tail; }

/**
 * Adds a new value to the end of the list.
 *
 * @param value The value to add to the list.
 */
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

/**
 * Retrieves a value from the list.
 *
 * @param value The value to search for in the list.
 * @return The value if found, otherwise returns a default-constructed value.
 */
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

/**
 * Removes a value from the list.
 *
 * @param value The value to remove from the list.
 */
template <typename V>
void LinkedList<V>::remove(V value)
{
    Node<V> *current = head;
    Node<V> *previous = nullptr;
    while (current != nullptr)
    {
        if (current->getValue() == value)
        {
            // If removing the head node
            if (previous == nullptr)
            {
                head = current->getNext();
                // If the list becomes empty after removing the head, update the tail to nullptr
                if (head == nullptr)
                {
                    tail = nullptr;
                }
            }
            else
            {
                previous->setNext(current->getNext());
                // If removing the last node, update the tail to the previous node
                if (current->getNext() == nullptr)
                {
                    tail = previous;
                }
            }
            size--;
            delete current;
            break;
        }
        previous = current;
        current = current->getNext();
    }
}

/**
 * Prints the values in the list to the standard output.
 */
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

// Explicit template instantiation
template class LinkedList<int>;
template class LinkedList<Message>;