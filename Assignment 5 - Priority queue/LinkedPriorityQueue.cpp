// implementation fo the functions in LinkedPriority Queue

#include "LinkedPriorityQueue.h"

LinkedPriorityQueue::LinkedPriorityQueue() {
    front = nullptr;
}

LinkedPriorityQueue::~LinkedPriorityQueue() {
    removeAllElements();
    delete front;
}

void LinkedPriorityQueue::changePriority(string value, int newPriority) {
    ListNode* current = front;
    bool found = false;

    while (current != nullptr) {
        if (current->value == value) {
            found = true;
            if (current->priority > newPriority) {
                enqueue(value, newPriority);
                delete current;
                //TO-DO Re-enqueue and remove old node without affecting lnked list
            } else {
               throw current->value;
            }
        }
        current = current->next;
    }

    if (!found) {
        throw "Value was not found in the priority queue";
    }
}

void LinkedPriorityQueue::clear() {
    removeAllElements();
}

void LinkedPriorityQueue::removeAllElements() {
    while(front != nullptr) {
        ListNode* thrash = front;
        front = front->next;
        delete thrash;
    }
}

string LinkedPriorityQueue::dequeue() {
    if (front != nullptr) {
        ListNode* topPriority = front;
        front = front->next;
        return topPriority->value;
    } else {
        throw "Queue does not contain any elements!";
    }
}

void LinkedPriorityQueue::enqueue(string value, int priority) {
    ListNode* entry = new ListNode(value, priority, nullptr);
    if (front == nullptr) {
        cout << endl << "nullptr entry executed" <<endl;
        front = entry;
    }
    else {
        ListNode* placeFinder = front;
        if (*entry < *placeFinder) {
            cout << endl << "new front element for a big group!" << endl;
            insert(entry, front);
        } else {
            bool inserted = false;
            while (placeFinder->next != nullptr) {
                if (*entry > *placeFinder && *entry < *placeFinder->next) {
                    insert(entry, placeFinder->next);
                    inserted = true;
                    break;
                }
                placeFinder = placeFinder->next;
            }
            if (!inserted && *entry < *placeFinder) {
                cout << endl << "new second last element" << endl;
                insert(entry, placeFinder);
            } else if (!inserted) {
                if (size() == 1) {
                    front->next = entry;
                } else {
                    placeFinder->next = entry;
                }
            }
        }
    }
}

void LinkedPriorityQueue::insert (ListNode* & toInsert, ListNode* & existing ) {
    toInsert->next = existing;
    existing = toInsert;
}

bool LinkedPriorityQueue::isEmpty() const {
    return front == nullptr;
}

string LinkedPriorityQueue::peek() const {
    if (front != nullptr) {
        return front->value;
    } else {
        throw "Queue does not contain any elements!";
    }
}

int LinkedPriorityQueue::peekPriority() const {
    if (front != nullptr) {
        return front->priority;
    } else {
        throw "Queue does not contain any elements!";
    }
}


int LinkedPriorityQueue::size() const {
    int size = 0;
    ListNode* counter = front;
    while (counter != nullptr) {
        size ++;
        counter = counter->next;
    }
    return size;
}

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

