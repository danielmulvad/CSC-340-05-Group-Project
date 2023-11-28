
#include <iostream>
using namespace std;


#include "Algorithm.h"


Node* search(Linkedlist *linkedList, string val){


Node* current = linkedList->getHead();
string currentString = linkedList->getData();

for (int i =0; i < linkedList->getSize(); i++){
    if (currentString == val){
        cout << "value found at " << i;
        return current;
    
    }
    else{
        current = current->getNext();
        currentString = current->getData();
    }
    
}
}