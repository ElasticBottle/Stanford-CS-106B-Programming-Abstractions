/*
 * FileName: LinkedPriorityQueue.cpp
 * ---------------------------------
 * Implements Priority queue using a LinkedList data structure
 */

#include "LinkedPriorityQueue.h"

/**
 * @brief LinkedPriorityQueue::LinkedPriorityQueue constructor that
 * initializes the front of the pq to a nullptr.
 */
LinkedPriorityQueue::LinkedPriorityQueue() {
    front = nullptr;
}

/**
 * @brief LinkedPriorityQueue::~LinkedPriorityQueue deconstructor that
 * frees the memory associated with the priority queue when the priority
 * queue is no longer needed
 */
LinkedPriorityQueue::~LinkedPriorityQueue() {
    removeAllElements();
}

/**
 * @brief LinkedPriorityQueue::changePriority changes the priority of an
 * existing element in the priority queue.
 * @param value provides the element value for which the priority is to be updated for.
 * An error will be thrown if the value given is not found within the linked list.
 * @param newPriority provides the new priority that is to be given to the element.
 * An error will be thrown if the new priority is of a higher value (less urgent)
 * than the existing priority value.
 */
void LinkedPriorityQueue::changePriority(string value, int newPriority) {
    ListNode* current = front;
    bool found = false;

    while (current != nullptr) {
        found = (current->value == value);
        if (found
                && current->priority > newPriority) {
            removeEntry(value);
            enqueue(value, newPriority);
            break;
        } else if (found
                   && current->priority < newPriority) {
           throw current->value;
        }
        current = current->next;
    }

    if (!found) {
        throw "Value was not found in the priority queue";
    }
}

/**
 * @brief LinkedPriorityQueue::removeEntry removes an entry from the linked list.
 * Assumes that the entry can already be foudn within the list.
 * @param value provides the value of the LIstNode to be removed.
 */
void LinkedPriorityQueue::removeEntry(string value) {
    ListNode* current = front;
    //if value is found at the start of the linked list
    if (current->value == value) {
        front = front->next;
        delete current;
        return;
    }

    //For values found elsewhere in the linked list
    while (current->next != nullptr) {
        if (current->next->value == value) {
            ListNode* thrash = current->next;
            current->next = current->next->next;
            delete thrash;
            return;
        }
        current = current->next;
    }
}

/**
 * @brief LinkedPriorityQueue::clear removes all the existing element of the linked
 * list afront of the linked list point to nullptr once again.
 */
void LinkedPriorityQueue::clear() {
    removeAllElements();
}
/**
 * @brief LinkedPriorityQueue::removeAllElements goes through the linked list sequentially and
 * frees the memory associated with each of the ListNode until there is nothing left to free.
 */
void LinkedPriorityQueue::removeAllElements() {
    while(front != nullptr) {
        ListNode* thrash = front;
        front = front->next;
        delete thrash;
    }
}

/**
 * @brief LinkedPriorityQueue::dequeue removes the ListNode entry with the highest priority
 * which sits at the front of the linked list.
 * Throws an error if attempting to dequeue and empty priority queue
 * @return the value of the highest priority ListNode which got removed from the Linkedlist
 */
string LinkedPriorityQueue::dequeue() {
    if (front != nullptr) {
        ListNode* topPriority = front;
        front = front->next;
        return topPriority->value;
    } else {
        throw "Queue does not contain any elements!";
    }
}

/**
 * @brief LinkedPriorityQueue::enqueue Adds a new ListNode entry to the linked list.
 * Linked list is stored in a sorted order so the lowest value priority (most urgent)
 * is located at the front of the linked list.
 * @param value provides the value of the new ListNode.
 * @param priority provides the priority of the new ListNode.
 */
void LinkedPriorityQueue::enqueue(string value, int priority) {
    ListNode* newEntry = new ListNode(value, priority, nullptr);

    //Insertion for empty list
    if (front == nullptr) {
        cout << endl << "nullptr entry executed" <<endl;
        front = newEntry;
        return;
    }

    //Insertion for non-empty list at appropriate place based on index
    int index = getIndexForInsertion(newEntry);

    if (index == 0) {
        insert(newEntry, front);
        return;
    }

    //Creates a placeFinder ListNode* and moves it to the node just before
    //the value for which newEntry is to be inserted in front off.
    ListNode* placeFinder = front;
    for (int i = 0; i < index - 1; ++i) {
        placeFinder = placeFinder->next;
    }
    insert(newEntry, placeFinder->next);
}

/**
 * @brief LinkedPriorityQueue::getIndexForInsertion Walks through the LinkedList
 * to search for the appropriate place for insertion. Assumes the the LinkedList
 * is not empty.
 * An Appropriate place for insertion is one where:
 *  -The ListNode before has a lower priority and the listNode after (if any) has a higher priority
 *  -The ListNodes before and after have the same priority but the ListNode in front have
 *   a value which is larger while the ListNode after(if any) have a value that is smaller
 *  -A mixture of the two conditions above
 * @param newEntry provides the ListNode* for which the LInkedLIst can be compared against
 * to find for the appropriate place for insertion
 * @return the zero-based index for where the newEntry should be inserted at.
 */
int LinkedPriorityQueue::getIndexForInsertion(ListNode* newEntry) {
    ListNode* placeFinder = front;
    int index = 0;
    //Finds the index for insertion
    while (placeFinder != nullptr) {
        if (placeFinder->priority > newEntry->priority
                || (newEntry->priority == placeFinder->priority
                    && newEntry->value > placeFinder->value)) {
            break;
        }
        index++;
        placeFinder = placeFinder->next;
    }
    return index;
}

/**
 * @brief LinkedPriorityQueue::insert adds one ListNode in front of the other.
 * @param toInsert is the listNode being inserted. It will be the ListNode in front.
 * @param existing is the listNode that is being re-reouted as a result of the
 * insertion. It will come after toInsert
 */
void LinkedPriorityQueue::insert (ListNode* & toInsert, ListNode* & existing ) {
    toInsert->next = existing;
    existing = toInsert;
}

/**
 * @brief LinkedPriorityQueue::isEmpty checks to see if the LinkedList is empty
 * @return true if the list is empty
 */
bool LinkedPriorityQueue::isEmpty() const {
    return front == nullptr;
}

/**
 * @brief LinkedPriorityQueue::peek checks the value of the most urgent element.
 * Throws an exception if the LinkedList is empty
 * @return the value fo the most urgent element
 */
string LinkedPriorityQueue::peek() const {
    if (front != nullptr) {
        return front->value;
    } else {
        throw "Queue does not contain any elements!";
    }
}

/**
 * @brief LinkedPriorityQueue::peekPriority checks to see the value of
 * the lowest value priority (most urgent).
 * @return the priority of the mostUrgent ListNode.
 */
int LinkedPriorityQueue::peekPriority() const {
    if (front != nullptr) {
        return front->priority;
    } else {
        throw "Queue does not contain any elements!";
    }
}

/**
 * @brief LinkedPriorityQueue::size returns the number of elements in the array
 * @return the number of elements in the array.
 */
int LinkedPriorityQueue::size() const {
    int size = 0;
    ListNode* counter = front;
    while (counter != nullptr) {
        size ++;
        counter = counter->next;
    }
    return size;
}

/**
 * @brief operator << Overloads the "<<" operator to allow the priority queue to
 * be printed to console like a variable
 * @param out provides the ostream to be returned at the end of the function
 * @param queue provides the LinkedList containing the elements to write out
 * @return the ostream
 */
ostream& operator<<(ostream& out, const LinkedPriorityQueue& queue) {
    ListNode* current = queue.front;
    cout << "{";
    while (current != nullptr) {
        cout << *current;
        current = current->next;
    }
    cout << "}";
    return out;
}

