#ifndef _NODE_H_
#define _NODE_H_

template <typename V>
class Node
{
public:
    Node(V value);
    Node(V value, Node<V> *next);
    V getValue() const;
    void setValue(V value);
    Node<V> *getNext() const;
    void setNext(Node<V> *next);

private:
    V value;
    Node<V> *next;
};

#endif // _NODE_H_