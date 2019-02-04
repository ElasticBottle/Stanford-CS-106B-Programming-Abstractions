// This is a .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include "ExtraPriorityQueue.h"

ExtraPriorityQueue::ExtraPriorityQueue() {
    // TODO: implement

}

ExtraPriorityQueue::~ExtraPriorityQueue() {
    // TODO: implement

}

void ExtraPriorityQueue::changePriority(string value, int newPriority) {
    // TODO: implement

}

void ExtraPriorityQueue::clear() {
    // TODO: implement

}

string ExtraPriorityQueue::dequeue() {
    // TODO: implement
    return "";   // remove this
}

void ExtraPriorityQueue::enqueue(string value, int priority) {
    // TODO: implement

}

bool ExtraPriorityQueue::isEmpty() const {
    // TODO: implement
    return false;   // remove this
}

string ExtraPriorityQueue::peek() const {
    // TODO: implement
    return "";   // remove this
}

int ExtraPriorityQueue::peekPriority() const {
    // TODO: implement
    return 0;   // remove this
}

int ExtraPriorityQueue::size() const {
    // TODO: implement
    return 0;   // remove this
}

ostream& operator<<(ostream& out, const ExtraPriorityQueue& queue) {
    // TODO: implement
    return out;
}
