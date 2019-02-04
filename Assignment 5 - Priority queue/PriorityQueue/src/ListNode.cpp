#include "ListNode.h"

ListNode::ListNode(string value, int priority, ListNode* next) {
    this->value = value;
    this->priority = priority;
    this->next = next;
}

bool operator <(const ListNode& node1, const ListNode& node2) {
    return node1.priority < node2.priority ||
            (node1.priority == node2.priority && node1.value < node2.value);
}

bool operator <=(const ListNode& node1, const ListNode& node2) {
    return node1 < node2 || node1 == node2;
}

bool operator ==(const ListNode& node1, const ListNode& node2) {
    return node1.priority == node2.priority && node1.value == node2.value;
}

bool operator !=(const ListNode& node1, const ListNode& node2) {
    return !(node1 == node2);
}

bool operator >(const ListNode& node1, const ListNode& node2) {
    return !(node1 <= node2);
}

bool operator >=(const ListNode& node1, const ListNode& node2) {
    return !(node1 < node2);
}

ostream& operator <<(ostream& out, const ListNode& node) {
    out << "\"" << node.value << "\":" << node.priority;
    return out;
}
