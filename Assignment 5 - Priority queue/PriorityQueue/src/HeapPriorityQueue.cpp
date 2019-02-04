// This is a .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include "HeapPriorityQueue.h"

HeapPriorityQueue::HeapPriorityQueue() {
    // TODO: implement

}

HeapPriorityQueue::~HeapPriorityQueue() {
    // TODO: implement

}

void HeapPriorityQueue::changePriority(string value, int newPriority) {
    // TODO: implement

}

void HeapPriorityQueue::clear() {
    // TODO: implement

}

string HeapPriorityQueue::dequeue() {
    // TODO: implement
    return "";   // remove this
}

void HeapPriorityQueue::enqueue(string value, int priority) {
    // TODO: implement

}

bool HeapPriorityQueue::isEmpty() const {
    // TODO: implement
    return false;   // remove this
}

string HeapPriorityQueue::peek() const {
    // TODO: implement
    return "";   // remove this
}

int HeapPriorityQueue::peekPriority() const {
    // TODO: implement
    return 0;   // remove this
}

int HeapPriorityQueue::size() const {
    // TODO: implement
    return 0;   // remove this
}

ostream& operator<<(ostream& out, const HeapPriorityQueue& queue) {
    // TODO: implement
    return out;
}
