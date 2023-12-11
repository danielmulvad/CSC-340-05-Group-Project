#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

#include "./Node.h"

template <typename V>
class LinkedList
{
public:
    LinkedList();
    LinkedList(Node<V> *head, Node<V> *tail, int size);
    ~LinkedList();

    Node<V> *getHead() const;
    void setHead(Node<V> *head);
    Node<V> *getTail() const;
    void setTail(Node<V> *tail);

    void add(V value);
    V get(V value);
    void remove(V value);
    void print();

private:
    Node<V> *head;
    Node<V> *tail;
    int size;
};

#endif // _LINKEDLIST_H_
