/*
 * CS 106B Huffman Encoding
 * This file declares the functions that you will need to write in this
 * assignment for your Huffman Encoder in huffmanencoding.cpp.
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * Author : Marty Stepp
 * Version: Thu 2013/11/14
 */

#ifndef _encoding_h
#define _encoding_h

#include <iostream>
#include <string>
#include "bitstream.h"
#include "HuffmanNode.h"
#include "map.h"
using namespace std;

/*
 * See huffmanencoding.cpp for documentation of these functions
 * (which you are supposed to write, based on the spec).
 */
Map<int, int> buildFrequencyTable(istream& input);
HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable);
Map<int, string> buildEncodingMap(HuffmanNode* encodingTree);
void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output);
void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output);
void compress(istream& input, obitstream& output);
void decompress(ibitstream& input, ostream& output);
void freeTree(HuffmanNode* node);

#endif
