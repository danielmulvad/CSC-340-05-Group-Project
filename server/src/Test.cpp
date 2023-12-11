#include "./LinkedList.h"
#include  "gtest/gtest.h"
#include <iostream>

using namespace std;

TEST(LinkedList, getHead){
    LinkedList<string>* linkedList = new LinkedList<string>();
    Node<string> *node = new Node<string>("head");
    Node<string> *node1 = new Node<string>("tail");

    linkedList->setHead(node);
    linkedList->setTail(node1);

    EXPECT_EQ(linkedList->getHead(), node);
    EXPECT_EQ(linkedList->getTail(), node1);

}

int testMain(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  cout << "worked";
  return RUN_ALL_TESTS();
}