// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own, along with
// comments on every function and on complex code sections.
// TODO: remove this comment header

#include "encoding.h"

Map<int, int> buildFrequencyTable(istream& input) {    
    Map<int, int> freqTable;
    char c;

    while (input.get(c)) {
        freqTable[c] += 1;
    }
    freqTable[PSEUDO_EOF] += 1;

    return freqTable;
}

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

PriorityQueue<HuffmanNode*> enqueueTableToPQ(const Map<int, int>& freqTable
                                             , PriorityQueue<HuffmanNode*> priority) {
    for (int ch : freqTable) {
        HuffmanNode* toEnqueue = new HuffmanNode(ch, freqTable[ch]);
        priority.enqueue(toEnqueue, toEnqueue->count);
    }

    return priority;
}


Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    Map<int, string> encodingMap;
    traverseEncodingTree(encodingTree, encodingMap, "");
    freeTree(encodingTree);
    return encodingMap;
}

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

void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {
    char ch;
    while (input.get(ch)) {
        string toWrite = encodingMap[ch];
        writeBitToFile(toWrite, output);
    }
    writeBitToFile(encodingMap[PSEUDO_EOF ], output);
}

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

void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    int toPut = NOT_A_CHAR;
    HuffmanNode* head = encodingTree;
    while (toPut != PSEUDO_EOF) {
        toPut = getCharInt(input, head);
        output.put(toPut);
    }
}

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

void compress(istream& input, obitstream& output) {
    Map<int, int> freqTable = buildFrequencyTable(input);
    output << freqTable;
    HuffmanNode* encodingTree = buildEncodingTree(freqTable);
    Map<int, string> encodingMap = buildEncodingMap(encodingTree);

    rewindStream(input);
    encodeData(input, encodingMap, output);
    freeTree(encodingTree);
}

void decompress(ibitstream& input, ostream& output) {
    Map<int, int> freqTable;

    input >> freqTable;
    HuffmanNode* encodingtree = buildEncodingTree(freqTable);

    decodeData(input, encodingtree, output);
    freeTree(encodingtree);
}

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
