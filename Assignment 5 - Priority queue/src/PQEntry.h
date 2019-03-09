/*
 * CS 106B Priority Queue
 * This file contains the implementation of the PQEntry structure.
 * Each PQEntry structure represents a single element in an array or vector for
 * a priority queue, storing a string of data and an int of priority.
 *
 * PQEntries know how to compare themselves to each other using standard
 * relational operators like <, >=, ==, !=.  They can also be printed to the
 * console using the << operator.
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * Version: Sat 2014/02/14
 * Author : Marty Stepp
 */

#ifndef _pqentry_h
#define _pqentry_h

#include <string>
#include <iostream>
using namespace std;

struct PQEntry {
public:
    string value;
    int priority;

    PQEntry(string value = "", int priority = 0);
};

bool operator <(const PQEntry& pqe1, const PQEntry& pqe2);
bool operator <=(const PQEntry& pqe1, const PQEntry& pqe2);
bool operator ==(const PQEntry& pqe1, const PQEntry& pqe2);
bool operator !=(const PQEntry& pqe1, const PQEntry& pqe2);
bool operator >(const PQEntry& pqe1, const PQEntry& pqe2);
bool operator >=(const PQEntry& pqe1, const PQEntry& pqe2);
ostream& operator <<(ostream& out, const PQEntry& pqe);

#endif
