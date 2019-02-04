/*
 * CS 106B Priority Queue
 * This file declares and implements the ListNode structure.
 * Each ListNode structure represents a single node in a linked list for a
 * priority queue, storing a string of data, an int of priority, and
 * a pointer to a next node (NULL if none).
 *
 * ListNodes know how to compare themselves to each other using standard
 * relational operators like <, >=, ==, !=.  Note that comparing nodes is NOT
 * the same as comparing pointers to nodes, which simply compares the memory
 * addresses as integers and ignores the objects' inner state.
 * Nodes can also be printed to the console using the << operator.  Again, this
 * is not the same as printing a pointer to a node, which would just print its
 * memory address as an integer.
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * Version: Sat 2014/02/14
 * Author : Marty Stepp
 */

#ifndef _listnode_h
#define _listnode_h

#include <iostream>
#include <string>
#include "PQEntry.h"
using namespace std;

struct ListNode {
public:
    string value;
    int priority;
    ListNode* next;

    ListNode(string value = "", int priority = 0, ListNode* next = NULL);
};

bool operator <(const ListNode& node1, const ListNode& node2);
bool operator <=(const ListNode& node1, const ListNode& node2);
bool operator ==(const ListNode& node1, const ListNode& node2);
bool operator !=(const ListNode& node1, const ListNode& node2);
bool operator >(const ListNode& node1, const ListNode& node2);
bool operator >=(const ListNode& node1, const ListNode& node2);
ostream& operator <<(ostream& out, const ListNode& node);

#endif
