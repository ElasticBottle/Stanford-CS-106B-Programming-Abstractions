/*
 * FileName: ArrayPriorityQueue.cpp
 * ---------------------------------
 * Implements the PriorityQueue using an array
 * as its internal data structure
 */

#include "ArrayPriorityQueue.h"

static int INITIAL_CAPACITY = 2;

/**
 * @brief ArrayPriorityQueue::ArrayPriorityQueue Constructor that
 * initializes the initial array size to 2 and sets the currentsize
 * of array to 0.
 */
ArrayPriorityQueue::ArrayPriorityQueue() {
   capacity = INITIAL_CAPACITY;
   elements = new PQEntry[capacity];
   currentSize = 0;
}

/**
 * @brief ArrayPriorityQueue::~ArrayPriorityQueue Destructor that
 * frees the memeory associated with the array when the priority
 * queue is no longer needed
 */
ArrayPriorityQueue::~ArrayPriorityQueue() {
    delete[] elements;
}

/**
 * @brief ArrayPriorityQueue::changePriority changes the priority of an
 * existing element in the priority queue.
 * @param value provides the element value for which the priority is to be updated for.
 * An error will be thrown if the value given is not found within the array.
 * @param newPriority provides the new priority that is to be given to the element.
 * An error will be thrown if the new priority is of a higher value (less urgent)
 * than the existing priority value.
 */
void ArrayPriorityQueue::changePriority(string value, int newPriority) {
    bool found = false;
    for (int i = 0; i < currentSize; i ++){
        found = (elements[i].value == value);
        if (elements[i].priority > newPriority && found) {
            elements[i].priority = newPriority;
            break;
        } else if (found && elements[i].priority < newPriority) {
            throw elements[i].value;
        }
    }

    if (!found) {
        throw "Value not found in the ArrayPriorityQueue!";
    }

}

/**
 * @brief ArrayPriorityQueue::clear removes all the existing element of the array
 * and re-initializes the array to an empty array of size capacity.
 */
void ArrayPriorityQueue::clear() {
    delete [] elements;
    elements = new PQEntry[capacity];
    currentSize = 0;
}

/**
 * @brief ArrayPriorityQueue::dequeue seraches the array for the value of the PQEntry
 * with the most urgent priority (lowest value) and returns it.
 * Throws an error if attempting to dequeue and empty priority queue
 * @return the value of the PQEntry with the most urgent priority (lowest value).
 */
string ArrayPriorityQueue::dequeue() {
    if (currentSize > 0) {
        PQEntry mostUrgent = getTopPQEntry();
        moveElementsUp(mostUrgent);
        return mostUrgent.value;
    } else {
        throw "ArrayPriorityQueue is empty!";
    }
}

/**
 * @brief ArrayPriorityQueue::moveElementsUp Removes the mostUrgent PQEntry and
 * shifts all other PQEntry after the mostUrgent PQEntry up by one.
 * @param mostUrgent provides the PQEntry to be erased. Assumes that
 * the PQEntry provided already exists within the array.
 */
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

/**
 * @brief ArrayPriorityQueue::enqueue adds a new PQEntry to the array
 * A new array with double the capacity size is created if the new
 * PQEntry attempts to add itself to a full array. All the old elements
 * are then copied into the new array and memory is freed accrodingly.
 * @param value provides the values for the new PQEntry to be added
 * @param priority provides the priority for the new PQEntry to be added
 */
void ArrayPriorityQueue::enqueue(string value, int priority) {
    PQEntry newEntry (value, priority);

    if (currentSize < capacity) {
        elements[currentSize ++] = newEntry;
    } else {
       capacity *= 2;
       PQEntry* oldElements = elements;
       elements = new PQEntry[capacity];
       for (int i = 0; i < currentSize; ++i) {
          elements[i] = oldElements[i];
       }
       elements[currentSize ++] = newEntry;
       delete [] oldElements;
    }
}

/**
 * @brief ArrayPriorityQueue::isEmpty Checks to see if the array is empty
 * @return true if the array is empty (currentSize = 0)
 */
bool ArrayPriorityQueue::isEmpty() const {
    return currentSize == 0;
}

/**
 * @brief ArrayPriorityQueue::peek looks at the value of the element with the
 * highest priority (lowest value).
 * Throws an error if the mehtod is called on an empty array.s
 * @return the value of the most urgent PQEntry
 */
string ArrayPriorityQueue::peek() const {
    if (currentSize > 0) {
        PQEntry mostUrgent = getTopPQEntry();
        return mostUrgent.value;
    } else {
        throw "ArrayPriorityQueue is empty!";
    }
}

/**
 * @brief ArrayPriorityQueue::peekPriority checks to see the value of
 * the lowest value priority (most urgent).
 * Throws an error if the method is called on an empty array.
 * @return the priority of the mostUrgent PQEntry.
 */
int ArrayPriorityQueue::peekPriority() const {
    if (currentSize > 0) {
        PQEntry mostUrgent = getTopPQEntry();
        return mostUrgent.priority;
    } else {
        throw "ArrayPriorityQueue is empty!";
    }
}

/**
 * @brief ArrayPriorityQueue::getTopPQEntry Searches the array for the PQEntry with the lowest
 * priority (most urgent) if two priority values are tied, the one whose value is larger is
 * deemed to be more urgent.
 * @return the PQEntry of the most urgent element in the array.
 */
PQEntry ArrayPriorityQueue::getTopPQEntry() const {
    PQEntry mostUrgent = elements[0];

    for (int i = 0; i  < currentSize - 1; ++i ) {
        if (mostUrgent.priority > elements[i + 1].priority) {
            mostUrgent = elements[i + 1];
        } else if (mostUrgent.priority == elements[i + 1].priority
                   && mostUrgent.value > elements[i + 1].value) {
            mostUrgent = elements[i + 1];
        }
    }

    return mostUrgent;
}

/**
 * @brief ArrayPriorityQueue::size returns the number of elements in the array
 * @return the number of elements in the array.
 */
int ArrayPriorityQueue::size() const {
    return currentSize;
}

/**
 * @brief operator << Overloads the "<<" operator to allow the priority queue to
 * be printed to console like a variable
 * @param out provides the ostream to be returned at the end of the function
 * @param queue provides the array containing the elements to write out.
 * @return the ostream
 */
ostream& operator<<(ostream& out, const ArrayPriorityQueue& queue) {
    cout << "{";
    for (int i = 0; i < queue.size(); i++) {
        if (i) cout << ",";
        cout << queue.elements[i];
    }
    cout << "}";
    return out;
}
