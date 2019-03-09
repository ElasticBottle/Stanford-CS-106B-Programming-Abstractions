/*
 * File Name: encoding.cpp
 * ----------------------------
 * Implements the Huffman encryption algorithm
 */

#include "encoding.h"

/**
 * @brief buildFrequencyTable creates a map, mapping individual words
 * to the frequency in which they appear in a text
 * @param input provides the ifstream opened to the file in which a frequency table
 * is to be created for
 * @return a map containing the character (stored as an int) mapped to the number
 * of times that it appeared in the file
 */
Map<int, int> buildFrequencyTable(istream& input) {    
    Map<int, int> freqTable;
    char c;

    while (input.get(c)) {
        freqTable[c] += 1;
    }
    freqTable[PSEUDO_EOF] += 1;

    return freqTable;
}

/**
 * @brief buildEncodingTree Creates an encoding tree by storingn freq table into a
 * priority queue and then dequeueing the two highests priority item and setting them as
 * children of a new node whose count is the sum of the frequency of the children
 * @param freqTable provides the frequency table in which the encoding tree is build out off
 * @return a HuffmanNode* pointing to the root of the encoding tree.
 */
HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {
    PriorityQueue<HuffmanNode*> priority;
    priority = enqueueTableToPQ(freqTable, priority);

    while (priority.size() >= 2) {
        HuffmanNode* parent = new HuffmanNode();
        HuffmanNode* left = priority.dequeue();
        HuffmanNode* right = priority.dequeue();

        parent->count = left->count + right->count;
        parent->zero =  left;
        parent->one = right;

        priority.enqueue(parent, parent->count);
    }

    return priority.dequeue();
}

/**
 * @brief enqueueTableToPQ enqueues a frequency table into a priority queue
 * @param freqTable provides the frequency table for the values and priority to be enqueued
 * @param priority provides the priority enqueue for the data to be enqueued into
 * @return the priority queue containing the frequency table values with the frequency of each
 * character as the priority.
 */
PriorityQueue<HuffmanNode*> enqueueTableToPQ(const Map<int, int>& freqTable
                                             , PriorityQueue<HuffmanNode*> priority) {
    for (int ch : freqTable) {
        HuffmanNode* toEnqueue = new HuffmanNode(ch, freqTable[ch]);
        priority.enqueue(toEnqueue, toEnqueue->count);
    }

    return priority;
}

/**
 * @brief buildEncodingMap creates a map for each character to its Huffman encoding value from a
 * Huffman encoding tree.
 * Utilizes traverseEncodingTree function to generate Huffman encoding value
 * @param encodingTree provides the Huffman encoding tree in which the encoding map is built out off
 * @return the map for each character value (stored as int) to its Huffman Encoding value.
 */
Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    Map<int, string> encodingMap;
    traverseEncodingTree(encodingTree, encodingMap, "");
    freeTree(encodingTree);
    return encodingMap;
}

/**
 * @brief traverseEncodingTree recursively traverses a Huffman encoding tree, adding 0 whenever it traverses left
 * and 1 whenever it traverses right to the code. The resulting code after the function reaches a leaf node
 * is the Huffman value for that particular charater that is stored in the leaf node and it is added to the map
 * @param encodingTree provides the Huffman encodiing tree that is being traverse
 * @param encodingMap prvies the Huffman map that maps each character to its Huffman encoign value
 * @param code contains the string of 1s and 0s that accumulates as the function traverses the tree towards a leaf node.
 */
void traverseEncodingTree(const HuffmanNode* encodingTree, Map<int, string>& encodingMap, string code) {
    if (encodingTree != NULL) {
        if (encodingTree->isLeaf()) {
            encodingMap[encodingTree->character] = code;
        } else {
            traverseEncodingTree(encodingTree->zero, encodingMap, code + "0");
            traverseEncodingTree(encodingTree->one, encodingMap,  code + "1");
        }
    }
}

/**
 * @brief encodeData writes the Huffman encoded data into an obitstream called output
 * @param input provides the input from which one character is read at a time.
 * @param encodingMap provides the mapping for the individual character to the Huffman encoding value
 * @param output provides the obitstream in which characters are written one bit at a time.
 */
void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {
    char ch;
    while (input.get(ch)) {
        string toWrite = encodingMap[ch];
        writeBitToFile(toWrite, output);
    }
    writeBitToFile(encodingMap[PSEUDO_EOF], output);
}

/**
 * @brief writeBitToFile takes a string containing only 1s and 0s and writes it to output, 1 bit at a time
 * @param toWrite provides the string that is to be written to the file, consisting
 * only of 1s and 0s
 * @param output contains the obitstream in which one bit is written to it at any one time
 */
void writeBitToFile(string toWrite, obitstream& output) {
    for (int i = 0; i < toWrite.length(); ++i) {
        switch (toWrite[i]) {
        case '1':
            output.writeBit(1);
            break;
        case '0':
            output.writeBit(0);
            break;
        }
    }
}

/**
 * @brief decodeData converts the data from the Huffman encoding value back into the
 * original text by using the Huffman encoding to traverse down the encodingTree
 * @param input provides the ifstream opened to the Huffman encoding bits.
 * @param encodingTree provides the encoding tree in which traversal occurs to find the orginal characters
 * @param output provides the ostream to output characters.
 */
void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    int toPut = NOT_A_CHAR;
    HuffmanNode* head = encodingTree;
    while (toPut != PSEUDO_EOF) {
        toPut = getCharInt(input, head);
        output.put(toPut);
    }
}

/**
 * @brief getCharInt traverses the tree until it reaches the node and returns
 * the value associated with the leaf node.
 * @param input provides the ibitstream in which one bit is read at a time.
 * @param encodingTree provides the Huffman encoding tree which is traversed
 * Everytime a 1 is read from input, the right branch of the tree is taken.
 * Everytime a 0 is read from input, the left branch of the tree is taken
 * @return the int assiciated at a leaf node in the encodingTree
 */
int getCharInt (ibitstream& input, HuffmanNode* encodingTree) {
    if (encodingTree != NULL){
        if (encodingTree->isLeaf()) {
            int output = encodingTree->character;
            return output;
        } else {
            int readBit = input.readBit();
            switch (readBit) {
            case 0:
                return getCharInt(input, encodingTree->zero);
            case 1:
                return getCharInt(input, encodingTree->one);
            }
        }
    }
    return 0;
}

/**
 * @brief compress takes an input and encodes that data, writing it into an output file
 * @param input provides the ifstream reader opened to the file that is intended for encoding.
 * @param output provides the obitstream for which the output can be written too.
 */
void compress(istream& input, obitstream& output) {
    Map<int, int> freqTable = buildFrequencyTable(input);
    output << freqTable;
    HuffmanNode* encodingTree = buildEncodingTree(freqTable);
    Map<int, string> encodingMap = buildEncodingMap(encodingTree);

    rewindStream(input);
    encodeData(input, encodingMap, output);
    freeTree(encodingTree);
}

/**
 * @brief decompress reads a file containing Huffman encoding and decrypts it
 * @param input provides the ifstream reader opened to the Huffman encoded file for decryption
 * @param output provides the ostream reader opened to write the decoded data too.
 */
void decompress(ibitstream& input, ostream& output) {
    Map<int, int> freqTable;

    input >> freqTable;
    HuffmanNode* encodingtree = buildEncodingTree(freqTable);

    decodeData(input, encodingtree, output);
    freeTree(encodingtree);
}

/**
 * @brief freeTree clears the memory associated with a particular node and all the children under it
 * @param node provides the node to be freed from the memory
 */
void freeTree(HuffmanNode* node) {
    if (node != NULL) {
        if (node->isLeaf()) {
            delete node;
            node = NULL;
        } else {
            freeTree(node->zero);
            freeTree(node->one);
        }
    }
}
