
#include <vector>
#include <iostream>
using namespace std;



// not sure of names of var yet
int search(Linkedlist linkedList, string val){

vector<int>::iterator it;
Node* current = linkedList->getHead();
string currentString = linkedList->getData();

for (*it = 0; it < linkedList->getSize(); it++){
    if (currentString == val){
        cout << "value found at " << *it;
        return *it;
    
    }
    else{
        current = current.getNext();
        currentString = current->getData();
    }
    
}
cout << "value not found";
    return 0;
}