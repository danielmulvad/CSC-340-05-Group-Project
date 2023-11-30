
#include <iostream>
using namespace std;


#include "Algorithm.h"
#include "./LinkedList.h"


Node<string>* search(LinkedList<string> *linkedList, string val){


Node<string>* current = linkedList->getHead();

int i =0;
while (current->getNext() != nullptr){
    i++;
    if (*current == val){
        cout << "value found at " << i;
        return current;
    
    }
    else{
        current = current->getNext();
    }
    
}
}