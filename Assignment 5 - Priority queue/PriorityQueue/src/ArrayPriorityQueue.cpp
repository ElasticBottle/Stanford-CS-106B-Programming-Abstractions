// This is a .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include "ArrayPriorityQueue.h"

ArrayPriorityQueue::ArrayPriorityQueue() {
    // TODO: implement

}

ArrayPriorityQueue::~ArrayPriorityQueue() {
    // TODO: implement

}

void ArrayPriorityQueue::changePriority(string value, int newPriority) {
    // TODO: implement

}

void ArrayPriorityQueue::clear() {
    // TODO: implement

}

string ArrayPriorityQueue::dequeue() {
    // TODO: implement
    return "";   // remove this
}

void ArrayPriorityQueue::enqueue(string value, int priority) {
    // TODO: implement

}

bool ArrayPriorityQueue::isEmpty() const {
    // TODO: implement
    return false;   // remove this
}

string ArrayPriorityQueue::peek() const {
    // TODO: implement
    return "";   // remove this
}

int ArrayPriorityQueue::peekPriority() const {
    // TODO: implement
    return 0;   // remove this
}

int ArrayPriorityQueue::size() const {
    // TODO: implement
    return 0;   // remove this
}

ostream& operator<<(ostream& out, const ArrayPriorityQueue& queue) {
    // TODO: implement
    return out;
}
