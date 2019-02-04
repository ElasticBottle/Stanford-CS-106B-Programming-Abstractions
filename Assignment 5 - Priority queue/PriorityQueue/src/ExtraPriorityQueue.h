// This is an optional .h file that you might edit and turn in.
// We have provided a skeleton for you,
// and you may finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#ifndef _extrapriorityqueue_h
#define _extrapriorityqueue_h

#include <iostream>
#include <string>
#include "PQEntry.h"
using namespace std;

/*
 * For documentation of each member, see VectorPriorityQueue.h.
 */
class ExtraPriorityQueue {
public:
    ExtraPriorityQueue();
    ~ExtraPriorityQueue();
    void changePriority(string value, int newPriority);
    void clear();
    string dequeue();
    void enqueue(string value, int priority);
    bool isEmpty() const;
    string peek() const;
    int peekPriority() const;
    int size() const;
    friend ostream& operator <<(ostream& out, const ExtraPriorityQueue& queue);

private:
    // TODO: add any other member functions/variables necessary

};

#endif
