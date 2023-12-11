#include "../../common/messages/Message.h"
#include "./Node.h"

template <typename V>
Node<V>::Node(V value) : Node(value, nullptr) {}

template <typename V>
Node<V>::Node(V value, Node<V> *next) : value(value), next(next) {}

template <typename V>
V Node<V>::getValue() const { return value; }

template <typename V>
void Node<V>::setValue(V value) { this->value = value; }

template <typename V>
Node<V> *Node<V>::getNext() const { return next; }

template <typename V>
void Node<V>::setNext(Node<V> *next) { this->next = next; }

template class Node<int>;
template class Node<Message>;