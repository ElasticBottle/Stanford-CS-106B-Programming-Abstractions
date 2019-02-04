/*
 * CS 106B Priority Queue
 * This file contains the implementation of the PQEntry structure.
 * See PQEntry.h for documentation of each member.
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * Version: Sat 2014/02/14
 * Author : Marty Stepp
 */

#include "PQEntry.h"

PQEntry::PQEntry(string value, int priority) {
    this->value = value;
    this->priority = priority;
}

bool operator <(const PQEntry& pqe1, const PQEntry& pqe2) {
    return pqe1.priority < pqe2.priority ||
            (pqe1.priority == pqe2.priority && pqe1.value < pqe2.value);
}

bool operator ==(const PQEntry& pqe1, const PQEntry& pqe2) {
    return pqe1.priority == pqe2.priority && pqe1.value == pqe2.value;
}

bool operator !=(const PQEntry& pqe1, const PQEntry& pqe2) {
    return !(pqe1 == pqe2);
}

bool operator <=(const PQEntry& pqe1, const PQEntry& pqe2) {
    return pqe1 < pqe2 || pqe1 == pqe2;
}

bool operator >(const PQEntry& pqe1, const PQEntry& pqe2) {
    return !(pqe1 <= pqe2);
}

bool operator >=(const PQEntry& pqe1, const PQEntry& pqe2) {
    return !(pqe1 < pqe2);
}

ostream& operator <<(ostream& out, const PQEntry& pqe) {
    out << "\"" << pqe.value << "\":" << pqe.priority;
    return out;
}
