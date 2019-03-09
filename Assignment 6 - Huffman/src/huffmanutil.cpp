/*
 * CS 106B Huffman Encoding
 * This file defines various utility functions used by the main client program.
 * See huffmanutil.h for documentation of each function.
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * Author : Marty Stepp
 * Version: Thu 2014/02/24
 * - removed some functions that are now part of Stanford C++ library
 * - updated to fix compiler errors with opening files on older compilers
 */

#include "huffmanutil.h"
#include "bitstream.h"
#include "filelib.h"
#include "simpio.h"

string bitsToBytes(string text) {
    istringbitstream input(text);
    ostringstream out;
    while (true) {
        int bit = input.readBit();
        if (input.fail()) {
            break;
        }
        out.put(bit == 1 ? '1' : '0');
    }
    return out.str();
}

string bytesToBits(string text) {
    ostringbitstream out;
    for (int i = 0; i < (int) text.length(); i++) {
        out.writeBit(text[i] == '1' ? 1 : 0);
    }
    return out.str();
}

bool confirmOverwrite(string filename) {
    if (!fileExists(filename)) {
        return true;
    } else {
        return getYesOrNo(filename + " already exists.  Overwrite? (y/n) ");
    }
}

int fileSize(string filename) {
    ifstream input;
    input.open(filename.c_str(), ifstream::binary);
    input.seekg(0, ifstream::end);
    return (int) input.tellg();
}

void printBits(string text) {
    istringbitstream input(text);
    int i = 0;
    while (true) {
        i++;
        int bit = input.readBit();
        if (input.fail()) break;
        cout << bit;
        if (i > 0 && i % 8 == 0) {
            cout << " ";
        }
        if (i > 0 && i % 64 == 0) {
            cout << endl;
        }
    }
    cout << endl;
}

string promptForExistingFileName(string prompt) {
    while (true) {
        string filename = getLine(prompt);
        if (fileExists(filename)) {
            return filename;
        } else {
            cout << "That file does not exist; please try again." << endl;
        }
    }
    return "";
}

string readEntireFileText(string filename) {
    ifstream input;
    input.open(filename.c_str());
    return readEntireFileText(input);
}

string readEntireFileText(istream& input) {
    ostringstream out;
    while (true) {
        int ch = input.get();
        if (input.fail()) {
            break;
        }
        out << (char) ch;
    }
    return out.str();
}

/*
 * Returns a printable string for the given character.  See bitstream.h.
 */
string toPrintableChar(int ch) {
    if (ch == '\n') {
        return "'\\n'";
    } else if (ch == '\t') {
        return "'\\t'";
    } else if (ch == '\r') {
        return "'\\r'";
    } else if (ch == '\f') {
        return "'\\f'";
    } else if (ch == '\b') {
        return "'\\b'";
    } else if (ch == '\0') {
        return "'\\0'";
    } else if (ch == ' ') {
        return "' '";
    } else if (ch == (int) PSEUDO_EOF) {
        return "EOF";
    } else if (ch == (int) NOT_A_CHAR) {
        return "NONE";
    } else if (!isgraph(ch)) {
        return "???";
    } else {
        return string("'") + (char) ch + string("'");
    }
}
