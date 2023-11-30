#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

#include <iostream>

template <typename V>
class Node
{
public:
    Node(V value) : Node(value, nullptr) {}
    Node(V value, Node<V> *next) : value(value), next(next) {}
    V getValue() const { return value; }
    void setValue(V value) { this->value = value; }
    Node<V> *getNext() const { return next; }
    void setNext(Node<V> *next) { this->next = next; }

private:
    V value;
    Node<V> *next;
};

bool operator==(const Node<string> &lhs, const string &rhs){
return lhs.getValue() == rhs;

}

template <typename V>
class LinkedList
{
public:
    LinkedList() : LinkedList(nullptr, nullptr, 0) {}
    LinkedList(Node<V> *head, Node<V> *tail, int size) : head(head), tail(tail), size(size) {}
    ~LinkedList()
    {
        Node<V> *current = head;
        while (current != nullptr)
        {
            Node<V> *next = current->getNext();
            delete current;
            current = next;
        }
    }

    Node<V> *getHead() const { return head; }
    void setHead(Node<V> *head) { this->head = head; }
    Node<V> *getTail() const { return tail; }
    void setTail(Node<V> *tail) { this->tail = tail; }

    void add(V value)
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

    V get(V value)
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
        return nullptr;
    }

    void remove(V value)
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
    void print()
    {
        Node<V> *current = head;
        while (current != nullptr)
        {
            std::cout << current->getValue() << std::endl;
            current = current->getNext();
        }
    }

private:
    Node<V> *head;
    Node<V> *tail;
    int size;

    
};

#endif