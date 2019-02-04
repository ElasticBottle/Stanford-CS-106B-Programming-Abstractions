/*
 * CS 106B Huffman Encoding
 * This file declares various utility functions used by the main client program.
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * Author : Marty Stepp
 * Version: Thu 2014/02/24
 * - removed some functions that are now part of Stanford C++ library
 */

#ifndef _huffmanutil_h
#define _huffmanutil_h

#include <iostream>
#include <string>
using namespace std;

/*
 * Takes a string of 0 and 1 binary bits and unpacks it so that it actually
 * stores them with each "bit" being its own ASCII character of '0' or '1'.
 */
string bitsToBytes(string text);

/*
 * Takes a string of '0' and '1' characters and packs it so that it actually
 * stores them as bits rather than each "bit" being its own ASCII character.
 */
string bytesToBits(string text);

/*
 * Checks whether the given file exists; if it does, prompts the user whether
 * they want to overwrite the file.  Returns true if the user does want to
 * overwrite, and false if not.
 */
bool confirmOverwrite(string filename);

/*
 * Returns the size of the given file in bytes.
 */
int fileSize(string filename);

/*
 * Displays a detailed dump of every bit and byte of the given string.
 * It prints every 8 bits (one byte) followed by a space, 8 bytes per line.
 * e.g. 10010010 10110011 10100010 00011101 ...
 */
void printBits(string text);

/*
 * Repeatedly asks the user to type a file name using the given prompt message
 * until the user types the name of a file that exists, then returns that file's name.
 */
string promptForExistingFileName(string prompt);

/*
 * Reads the entire contents of the given input source and returns them as a string.
 */
string readEntireFileText(istream& input);

/*
 * Reads the entire contents of the given input file and returns them as a string.
 */
string readEntireFileText(string filename);

/*
 * Returns a string that represents the given character.
 * For most standard ASCII characters, this is just the character itself.
 * But for whitespace, escape characters, and extended ASCII, it returns an
 * expanded representation like "\\n" for '\n' or "???" for a non-printable char.
 * It also returns "EOF" when passed PSEUDO_EOF and NOT when passed NOT_A_CHAR.
 */
string toPrintableChar(int ch);

#endif
