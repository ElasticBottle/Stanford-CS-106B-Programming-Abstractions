// This is a .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include "ArrayPriorityQueue.h"

static int INITIAL_CAPACITY = 2;

ArrayPriorityQueue::ArrayPriorityQueue() {
   capacity = INITIAL_CAPACITY;
   elements = new PQEntry[capacity];
   currentSize = 0;
}

ArrayPriorityQueue::~ArrayPriorityQueue() {
    delete[] elements;
}

void ArrayPriorityQueue::changePriority(string value, int newPriority) {
    bool found = false;
    for (int i = 0; i < currentSize; i ++){
        if (elements[i].value == value) {
            found = true;
            if (elements[i].priority > newPriority) {
                elements[i].priority = newPriority;
                break;
            } else {
               throw elements[i].value;
            }
        }
    }

    if (!found) {
        throw "Value not found in the ArrayPriorityQueue!";
    }

}

void ArrayPriorityQueue::clear() {
    delete [] elements;
    elements = new PQEntry[capacity];
    currentSize = 0;
}

string ArrayPriorityQueue::dequeue() {
    if (currentSize > 0) {
        PQEntry mostUrgent = getTopPQEntry();
        moveElementsUp(mostUrgent);
        return mostUrgent.value;
    } else {
        throw "ArrayPriorityQueue is empty!";
    }
}

void ArrayPriorityQueue::moveElementsUp(PQEntry mostUrgent) {
    int pos = 0;
    for (int i = 0; i < currentSize; ++i) {
        if (mostUrgent == elements[i]){
            pos = i;
            break;
        }
    }
    for (int i = pos; i < currentSize - 1; ++i) {
       elements[i] = elements[i + 1];
    }
    currentSize --;
}

void ArrayPriorityQueue::enqueue(string value, int priority) {
    PQEntry newEntry (value, priority);

    if (currentSize < capacity) {
        elements[currentSize ++] = newEntry;
    } else {
       capacity *= 2;
       PQEntry* newElements = new PQEntry[capacity];
       for (int i = 0; i < currentSize; ++i) {
          newElements[i] = elements[i];
       }
       newElements [currentSize ++] = newEntry;
       PQEntry* thrash = elements;
       elements = newElements;
       newElements = elements;
       delete [] thrash;
       delete newElements;
    }
}

bool ArrayPriorityQueue::isEmpty() const {
    return currentSize == 0;
}

string ArrayPriorityQueue::peek() const {
    if (currentSize > 0) {
        PQEntry mostUrgent = getTopPQEntry();
        return mostUrgent.value;
    } else {
        throw "ArrayPriorityQueue is empty!";
    }
}

int ArrayPriorityQueue::peekPriority() const {
    if (currentSize > 0) {
        PQEntry mostUrgent = getTopPQEntry();
        return mostUrgent.priority;
    } else {
        throw "ArrayPriorityQueue is empty!";
    }
}

PQEntry ArrayPriorityQueue::getTopPQEntry() const {
    PQEntry mostUrgent = elements[0];

    for (int i = 0; i  < currentSize - 1; ++i ) {
        if (mostUrgent.priority > elements[i + 1].priority) {
            mostUrgent = elements[i + 1];
        } else if (mostUrgent.priority == elements[i + 1].priority) {
            if (mostUrgent.value > elements[i + 1].value) {
                mostUrgent = elements[i + 1];
            }
        }
    }

    return mostUrgent;
}

int ArrayPriorityQueue::size() const {
    return currentSize;
}

ostream& operator<<(ostream& out, const ArrayPriorityQueue& queue) {
    cout << "{";
    for (int i = 0; i < queue.size(); i++) {
        if (i) cout << ",";
        cout << queue.elements[i];
    }
    cout << "}";
    return out;
}
