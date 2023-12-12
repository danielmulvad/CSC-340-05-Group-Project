#include "../../common/messages/Message.h"
#include "./Node.h"

/**
 * Constructor for Node.
 * Initializes a Node with a value and sets the next node to nullptr.
 *
 * @param value The value stored in the node.
 */
template <typename V>
Node<V>::Node(V value) : Node(value, nullptr) {}

/**
 * Constructor for Node with a next node.
 * Initializes a Node with a value and a pointer to the next node.
 *
 * @param value The value stored in the node.
 * @param next Pointer to the next Node in the linked list.
 */
template <typename V>
Node<V>::Node(V value, Node<V> *next) : value(value), next(next) {}

/**
 * Gets the value stored in the node.
 *
 * @return The value of the node.
 */
template <typename V>
V Node<V>::getValue() const { return value; }

/**
 * Sets the value of the node.
 *
 * @param value The new value to set in the node.
 */
template <typename V>
void Node<V>::setValue(V value) { this->value = value; }

/**
 * Gets the pointer to the next node.
 *
 * @return Pointer to the next Node in the linked list.
 */
template <typename V>
Node<V> *Node<V>::getNext() const { return next; }

/**
 * Sets the pointer to the next node.
 *
 * @param next Pointer to the new next Node in the linked list.
 */
template <typename V>
void Node<V>::setNext(Node<V> *next) { this->next = next; }

// Explicit template instantiation
template class Node<int>;
template class Node<Message>;