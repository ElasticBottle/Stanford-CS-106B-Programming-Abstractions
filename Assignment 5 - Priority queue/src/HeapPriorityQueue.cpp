/*
 * FileName: HeapPriorityQueue.cpp
 * ---------------------------------
 * Implements Priority queue using a heap data structure
 */

#include "HeapPriorityQueue.h"

static int INITIAL_CAPACITY = 10;

/**
 * @brief HeapPriorityQueue::HeapPriorityQueue Constructor that
 * initializes the initial array size to 10 + 1 (due to offset that slot 0 is never used
 * and sets the currentsize of array to 0.
 */
HeapPriorityQueue::HeapPriorityQueue() {
    capacity = INITIAL_CAPACITY;
    elements = new PQEntry[capacity + 1];
    currentSize = 0;
}

/**
 * @brief HeapPriorityQueue::~HeapPriorityQueue Destructor that
 * frees the memeory associated with the array when the priority
 * queue is no longer needed
 */
HeapPriorityQueue::~HeapPriorityQueue() {
    delete[] elements;
}

/**
 * @brief HeapPriorityQueue::changePriority changes the priority of an
 * existing element in the priority queue.
 * @param value provides the element value for which the priority is to be updated for.
 * An error will be thrown if the value given is not found within the array.
 * @param newPriority provides the new priority that is to be given to the element.
 * An error will be thrown if the new priority is of a higher value (less urgent)
 * than the existing priority value.
 */
void HeapPriorityQueue::changePriority(string value, int newPriority) {
    bool found = false;
    for (int i = 1; i <= currentSize; ++i) {
        found = elements[i].value == value;
        if (found) {
            if (elements[i].priority > newPriority) {
                elements[i].priority = newPriority;
                bubbleUp(i);
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

/**
 * @brief ArrayPriorityQueue::clear removes all the existing element of the array
 * and re-initializes the array to an empty array of size capacity.
 */
void HeapPriorityQueue::clear() {
    delete [] elements;
    currentSize = 0;
    elements = new PQEntry[capacity + 1];
}

/**
 * @brief HeapPriorityQueue::dequeue removes the first element of the array (most urgent)
 * and returns its value.
 * Throws an error if attempting to dequeue and empty priority queue
 * @return the value of the PQEntry with the most urgent priority (lowest value).
 */
string HeapPriorityQueue::dequeue() {
    if (currentSize != 0) {
        string value = elements[1].value;
        elements[1] = elements[currentSize--];
        bubbleDown(1);
        return value;
    } else {
        throw "The queue does not contain any elements!";
    }

}

/**
 * @brief HeapPriorityQueue::bubbleDown compares a parent element to its two child.
 * A swap is made with the child if it is of higher priority (lower priority number
 * or same priority number but value is larger).
 * If both the child are higher priority, swap with the highest priority child.
 * @param i provides the index in which the element being bubbled down is tracked
 */
void HeapPriorityQueue::bubbleDown (int i) {
    PQEntry emptyVal;
    if (elements[i * 2] != emptyVal) {
        // Parent element has two child
        if (elements[(i * 2) + 1] != emptyVal) {

            // Getting the highest priority child
            int highPriority = getHigherPriority(i * 2, (i * 2) + 1);

            // Determining if the highest priority child or the parent has
            // a higher priority. Swap is made if the parent does not have
            // a higher priority
            int highestPriority = getHigherPriority(highPriority, i);
            if (elements[highestPriority] != elements[i]) {
                swap(elements[highestPriority], elements[i]);
                bubbleDown(highestPriority);
            }
        }
        //Parent element only has one child
        else {
            int highestPriority = getHigherPriority(i * 2, i);
            if (elements[highestPriority] != elements[i]) {
                swap(elements[highestPriority], elements[i]);
                bubbleDown(highestPriority);
            }
        }
    }
}

/**
 * @brief HeapPriorityQueue::getHigherPriority checks two index location in the array
 * and returns the index location of the array location with the higher priority
 * @param index1 location of one element in the array
 * @param index2 locatio of the second element in the array
 * @return the index location in the array of the element with the heigher priority
 */
int HeapPriorityQueue::getHigherPriority (int index1, int index2) {
    if (elements[index1].priority > elements[index2].priority
            || (elements[index1].priority == elements[index2].priority
                && elements[index1].value < elements[index2].value)) {
        return  index2;
    }
    return index1;
}

/**
 * @brief HeapPriorityQueue::enqueue adds a new PQEntry to the array
 * A new array with double the capacity size is created if the new
 * PQEntry attempts to add itself to a full array. All the old elements
 * are then copied into the new array and memory is freed accrodingly
 * @param value provides the values for the new PQEntry to be added
 * @param priority provides the priority for the new PQEntry to be added
 */
void HeapPriorityQueue::enqueue(string value, int priority) {
    PQEntry newEntry(value, priority);

    if (currentSize < capacity) {
        elements[++ currentSize] = newEntry;
        bubbleUp(currentSize);
    } else {
        //Creating new array to store the data
        PQEntry* oldElements = elements;
        capacity *= 2;
        elements = new PQEntry[capacity + 1];

        for (int i = 1; i <= currentSize; ++i) {
            elements[i] = oldElements[i];
        }

        delete [] oldElements;
        elements [++ currentSize] = newEntry;
        bubbleUp(currentSize);
    }
}

/**
 * @brief HeapPriorityQueue::bubbleUp compares the newest child element (last element)
 * in the heap to its parent element at the index half of itself and keeps swapping
 * position with its parents if it satisfy some conditions.
 * The conditions are:
 *  -If the priority of the last element if lesser than its parent element (more urgent)
 *  -If both child and parent element has the same priority but the child has a larger value.
 */
void HeapPriorityQueue::bubbleUp(int index) {
    for (int i = index; i > 1; i /= 2) {
        int highestPriority = getHigherPriority(i , i / 2);

        //Checking to see if the highest priority is the child.
        //If so, the parent and child swap positions
        if (elements[highestPriority] != elements[i / 2]) {
            swap(elements[highestPriority], elements[i / 2]);
        } else {
            break;
        }
    }

}

/**
 * @brief HeapPriorityQueue::isEmpty Checks to see if the array is empty
 * @return true if the array is empty (currentSize = 0)
 */
bool HeapPriorityQueue::isEmpty() const {
    return currentSize == 0;
}

/**
 * @brief HeapPriorityQueue::peek looks at the value of the element with the
 * highest priority (lowest value).
 * Throws an error if the method is called on an empty array.
 * @return the value of the most urgent PQEntry
 */
string HeapPriorityQueue::peek() const {
    if (currentSize != 0) {
        return elements[1].value;
    } else {
       throw "The queue does not contain any elements!";
    }
}

/**
 * @brief HeapPriorityQueue::peekPriority checks to see the value of
 * the lowest value priority (most urgent).
 * Throws an error if the method is called on an empty array.
 * @return the priority of the mostUrgent PQEntry.
 */
int HeapPriorityQueue::peekPriority() const {
    if (currentSize != 0) {
        return elements[1].priority;
    } else {
        throw "The queue does not contain any elements!";
    }
}

/**
 * @brief HeapPriorityQueue::size returns the number of elements in the array
 * @return the number of elements in the array.
 */
int HeapPriorityQueue::size() const {
    return currentSize;
}

/**
 * @brief operator << Overloads the "<<" operator to allow the priority queue to
 * be printed to console like a variable
 * @param out provides the ostream to be returned at the end of the function
 * @param queue provides the array containing the elements to write out.
 * @return the ostream
 */
ostream& operator<<(ostream& out, const HeapPriorityQueue& queue) {
    cout << "{";
    for (int i = 1; i <= queue.size(); ++i) {
        if (i != 1) cout << ",";
        cout << queue.elements[i];
    }
    cout << "}";
    return out;
}
