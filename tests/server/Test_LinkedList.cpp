#include "./Test_LinkedList.h"

TEST(LinkedList, getHead)
{
    LinkedList<int> list;
    list.add(1);
    list.add(2);
    list.add(3);
    EXPECT_EQ(list.getHead()->getValue(), 1);
}

TEST(LinkedList, getTail)
{
    LinkedList<int> list;
    list.add(1);
    list.add(2);
    list.add(3);
    EXPECT_EQ(list.getTail()->getValue(), 3);
}

TEST(LinkedList, add)
{
    LinkedList<int> list;
    list.add(1);
    list.add(2);
    list.add(3);
    EXPECT_EQ(list.getHead()->getValue(), 1);
    EXPECT_EQ(list.getTail()->getValue(), 3);
}

TEST(LinkedList, remove)
{
    LinkedList<int> list;
    list.add(1);
    list.add(2);
    list.add(3);
    list.remove(2);
    EXPECT_EQ(list.getHead()->getValue(), 1);
    EXPECT_EQ(list.getTail()->getValue(), 3);
}

TEST(LinkedList, removeHead)
{
    LinkedList<int> list;
    list.add(1);
    list.add(2);
    list.add(3);
    list.remove(1);
    EXPECT_EQ(list.getHead()->getValue(), 2);
    EXPECT_EQ(list.getTail()->getValue(), 3);
}

TEST(LinkedList, removeTail)
{
    LinkedList<int> list;
    list.add(1);
    list.add(2);
    list.add(3);
    list.remove(3);
    EXPECT_EQ(list.getHead()->getValue(), 1);
    EXPECT_EQ(list.getTail()->getValue(), 2);
}

TEST(LinkedList, removeNonExistent)
{
    LinkedList<int> list;
    list.add(1);
    list.add(2);
    list.add(3);
    list.remove(4);
    EXPECT_EQ(list.getHead()->getValue(), 1);
    EXPECT_EQ(list.getTail()->getValue(), 3);
}