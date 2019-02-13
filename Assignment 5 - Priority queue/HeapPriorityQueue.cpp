// This is a .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include "HeapPriorityQueue.h"

static int INITIAL_CAPACITY = 10;

HeapPriorityQueue::HeapPriorityQueue() {
    capacity = INITIAL_CAPACITY;
    elements = new PQEntry[capacity + 1];
    currentSize = 0;
}

HeapPriorityQueue::~HeapPriorityQueue() {
    delete[] elements;
}

void HeapPriorityQueue::changePriority(string value, int newPriority) {
    // TODO: implement

}

void HeapPriorityQueue::clear() {
    PQEntry* thrash = elements;
    capacity = INITIAL_CAPACITY;
    currentSize = 0;
    elements = new PQEntry[capacity + 1];
    delete thrash;
}

string HeapPriorityQueue::dequeue() {
    if (currentSize != 0) {
        string value = elements[1].value;
        elements[1] = elements[currentSize--];
        cout << elements[1] <<endl; <<endl;
        bubbleDown(1);
        return value;
    } else {
        throw "The queue does not contain any elements!";
    }

}

void HeapPriorityQueue::bubbleDown (int i) {
    PQEntry emptyVal;
    if (elements[i * 2] != emptyVal
            && elements[i].priority > elements[i * 2].priority) {
        swap(elements [i], elements[i * 2]);
        bubbleDown(i * 2);
    }
    else if (elements[(i * 2) + 1] != emptyVal
             && elements[i].priority > elements[(i * 2) + 1].priority) {
        swap(elements [i], elements[(i * 2) + 1]);
        bubbleDown((i * 2 ) + 1);
    }
    //TO-DO make it such that for parent node only swap with the most urgent
    //      of the two children and not both.
}

void HeapPriorityQueue::enqueue(string value, int priority) {
    PQEntry newEntry(value, priority);

    if (currentSize < capacity) {
        elements[++ currentSize] = newEntry;
        bubbleUp();
    } else {
        //Creating new array to store the data
        capacity *= 2;
        PQEntry* oldElements = elements;
        elements = new PQEntry[capacity + 1];

        for (int i = 1; i <= currentSize; ++i) {
            elements[i] = oldElements[i];
        }
        elements [++ currentSize] = newEntry;
        bubbleUp();
        delete [] oldElements;
    }
}

void HeapPriorityQueue::bubbleUp() {
    for (int i = currentSize; i > 1; i /= 2) {
        if (elements[i].priority < elements[i / 2].priority) {
            swap(elements[i], elements [i / 2]);
        }
    }

}

bool HeapPriorityQueue::isEmpty() const {
    return currentSize == 0;
}

string HeapPriorityQueue::peek() const {
    if (currentSize != 0) {
        return elements[1].value;
    } else {
       throw "The queue does not contain any elements!";
    }
}

int HeapPriorityQueue::peekPriority() const {
    if (currentSize != 0) {
        return elements[1].priority;
    } else {
        throw "The queue does not contain any elements!";
    }
}

int HeapPriorityQueue::size() const {
    return currentSize;
}

ostream& operator<<(ostream& out, const HeapPriorityQueue& queue) {
    cout << "{";
    for (int i = 1; i <= queue.size(); ++i) {
        if (i != 1) cout << ",";
        cout << queue.elements[i];
    }
    cout << "}";
    return out;
}
