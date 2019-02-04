/*
 * CS 106B Huffman Encoding
 * This file contains the main program and user interface for running your
 * Huffman Encoder.  It contains a text menu to allow you to test all of the
 * various functions of your program for encoding and decoding data.
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * Author : Marty Stepp
 * Version: Thu 2013/11/21
 */

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "random.h"
#include "simpio.h"
#include "strlib.h"
#include "HuffmanNode.h"
#include "encoding.h"
#include "huffmanutil.h"
using namespace std;

const bool SHOW_TREE_ADDRESSES = false;   // set to true to debug tree pointer issues
const string DEFAULT_COMPRESSED_FILE_EXTENSION = ".huf";
const string DEFAULT_DECOMPRESSED_FILE_EXTENSION = ".txt";

// function prototype declarations; see definitions below for documentation
void intro();
string menu();
void test_buildFrequencyTable(Map<int, int>& freqTable, string& data, bool& isFile);
void test_buildEncodingTree(Map<int, int>& freqTable, HuffmanNode*& encodingTree);
void test_buildEncodingMap(HuffmanNode*& encodingTree, Map<int, string>& encodingMap);
void test_encodeData(Map<int, string>& encodingMap, string& data, bool& isFile);
void test_decodeData(HuffmanNode* encodingTree);
void test_compress();
void test_decompress();
void test_freeTree(HuffmanNode* encodingTree);
void test_binaryFileViewer();
void test_textFileViewer();
void test_sideBySideComparison();
istream* openInputStream(string data, bool isFile, bool isBits = false);
istream* openStringOrFileInputStream(string& data, bool& isFile, bool isBits = false);

int main() {
    intro();

    // these variables maintain state between steps 1-4
    string data;
    bool isFile = false;
    HuffmanNode* encodingTree = NULL;
    Map<int, int> freqTable;
    Map<int, string> encodingMap;

    // prompt user for options repeatedly
    while (true) {
        string choice = menu();
        if (choice == "Q") {
            break;
        } else if (choice == "1") {
            test_buildFrequencyTable(freqTable, data, isFile);
        } else if (choice == "2") {
            test_buildEncodingTree(freqTable, encodingTree);
        } else if (choice == "3") {
            test_buildEncodingMap(encodingTree, encodingMap);
        } else if (choice == "4") {
            test_encodeData(encodingMap, data, isFile);
        } else if (choice == "5") {
            test_decodeData(encodingTree);
        } else if (choice == "C") {
            test_compress();
        } else if (choice == "D") {
            test_decompress();
        } else if (choice == "B") {
            test_binaryFileViewer();
        } else if (choice == "T") {
            test_textFileViewer();
        } else if (choice == "S") {
            test_sideBySideComparison();
        } else if (choice == "F") {
            test_freeTree(encodingTree);
            encodingTree = NULL;
        }
    }

    cout << "Exiting." << endl;
    return 0;
}

/*
 * Sets up the output console and explains the program to the user.
 */
void intro() {
    cout << "Welcome to CS 106B Shrink-It!" << endl;
    cout << "This program uses the Huffman coding algorithm for compression." << endl;
    cout << "Any file can be compressed by this method, often with substantial" << endl;
    cout << "savings. Decompression will faithfully reproduce the original." << endl;
}

/*
 * Prints a menu of choices for the user and reads/returns the user's response.
 */
string menu() {
    cout << endl;
    cout << "1) build character frequency table" << endl;
    cout << "2) build encoding tree" << endl;
    cout << "3) build encoding map" << endl;
    cout << "4) encode data" << endl;
    cout << "5) decode data" << endl;
    cout << endl;
    cout << "C) compress file" << endl;
    cout << "D) decompress file" << endl;
    cout << "F) free tree memory" << endl;
    cout << endl;
    cout << "B) binary file viewer" << endl;
    cout << "T) text file viewer" << endl;
    cout << "S) side-by-side file comparison" << endl;
    cout << "Q) quit" << endl;

    cout << endl;
    string choice = toUpperCase(trim(getLine("Your choice? ")));
    return choice;
}

/*
 * Tests the buildFrequencyTable function.
 * Prompts the user for a string of data or input file to read,
 * then builds a frequency map of its characters and prints that map's contents.
 * Stores the built map in the given output parameter freqTable.
 * Also stores output parameters for the text input read, and whether the input
 * came from a string of text or a file.  This is reused later by main.
 *
 */
void test_buildFrequencyTable(Map<int, int>& freqTable, string& data, bool& isFile) {
    istream* input = openStringOrFileInputStream(data, isFile);
    cout << "Building frequency table ..." << endl;
    freqTable = buildFrequencyTable(*input);
    for (int ch : freqTable) {
        cout << "    " << setw(3) << ch
             << ": " << setw(4) << toPrintableChar(ch) << "  => "
             << setw(7) << freqTable[ch] << endl;
    }
    cout << freqTable.size() << " character frequencies found." << endl;
    delete input;
}

/*
 * Tests the buildEncodingTree function.
 * Accepts a frequency table map as a parameter, presumably the one generated
 * previously in step 1 by buildFrequencyTable.
 * Then prints the encoding tree in an indented sideways format.
 * Stores the built tree in the given output parameter encodingTree.
 */
void test_buildEncodingTree(Map<int, int>& freqTable, HuffmanNode*& encodingTree) {
    if (freqTable.isEmpty()) {
        cout << "Can't build tree; character frequency table is empty or uninitialized." << endl;
    } else {
        cout << "Building encoding tree ..." << endl;
        encodingTree = buildEncodingTree(freqTable);
        printSideways(encodingTree, SHOW_TREE_ADDRESSES);
    }
}

/*
 * Tests the buildEncodingMap function.
 * Accepts an encoding tree as a parameter, presumably the one generated
 * previously in step 2 by buildEncodingTree.
 * Then prints the encoding map of characters to binary encodings.
 * Stores the built map in the given output parameter encodingMap.
 */
void test_buildEncodingMap(HuffmanNode*& encodingTree, Map<int, string>& encodingMap) {
    if (encodingTree == NULL) {
        cout << "Can't build map; encoding tree is NULL." << endl;
    } else {
        cout << "Building encoding map ..." << endl;
        encodingMap = buildEncodingMap(encodingTree);
        for (int ch : encodingMap) {
            cout << "    " << setw(3) << ch
                 << ": " << setw(4) << toPrintableChar(ch) << "  => "
                 << encodingMap[ch] << endl;
        }
        cout << encodingMap.size() << " character encodings found." << endl;
    }
}

/*
 * Tests the encodeData function.
 * Accepts as a parameter a map of encodings, presumably the one generated
 * previously in step 3 by buildEncodingMap.
 * Allows the user to encode the same data from the original file/string,
 * or new data that is typed / data from a file.
 * Once encoding is done, prints the bits of the encoded data.
 */
void test_encodeData(Map<int, string>& encodingMap, string& data, bool& isFile) {
    if (encodingMap.isEmpty()) {
        cout << "Can't encode data; encoding map is empty or uninitialized." << endl;
    } else {
        istream* input = NULL;
        bool reuse = getYesOrNo("Reuse your previous string/file data for encoding? ");
        if (reuse) {
            input = openInputStream(data, isFile);
        } else {
            input = openStringOrFileInputStream(data, isFile);
        }

        ostringbitstream output;
        cout << "Encoding data ..." << endl;
        encodeData(*input, encodingMap, output);
        output.flush();
        string text = output.str();
        cout << "Here is the binary encoded data (" << text.length() << " bytes):" << endl;
        printBits(text);
        delete input;
    }
}

/*
 * Tests the decodeData function.
 * Uses the given encoding tree, presumably one encoded previously in step 2
 * by buildEncodingTree.
 * Prompts for a file or string of binary input data and decodes it into a
 * string output stream, then prints the text on the console.
 */
void test_decodeData(HuffmanNode* encodingTree) {
    if (encodingTree == NULL) {
        cout << "Can't decode; encoding tree is NULL." << endl;
    } else {
        string data;
        bool isFile;
        ibitstream* input = (ibitstream*) openStringOrFileInputStream(data, isFile, /* isBits */ true);
        ostringstream output;

        cout << "Decoding data ..." << endl;
        decodeData(*input, encodingTree, output);
        output.flush();

        string decoded = output.str();
        cout << "Here is the decoded data ("
             << decoded.length() << " bytes):" << endl;
        cout << decoded << endl;

        delete input;
    }
}

/*
 * Tests the compress function.
 * Prompts for input/output file names and opens streams on those files.
 * Then calls your compress function and displays information about how many
 * bytes were written, if any.
 */
void test_compress() {
    string inputFileName = promptForExistingFileName("Input file name: ");
    ifstream input;
    ofbitstream output;
    string defaultOutputFileName = getRoot(inputFileName) + DEFAULT_COMPRESSED_FILE_EXTENSION;
    string outputFileName = trim(getLine("Output file name (Enter for "
                                    + defaultOutputFileName + "): "));
    if (outputFileName == "") {
        outputFileName = defaultOutputFileName;
    }
    if (inputFileName == outputFileName) {
        cout << "You cannot specify the same file as both the input file" << endl;
        cout << "and the output file.  Aborting." << endl;
        return;
    }
    if (!confirmOverwrite(outputFileName)) {
        return;
    }

    int inputFileSize = fileSize(inputFileName);
    cout << "Reading " << inputFileSize << " uncompressed bytes." << endl;
    input.open(inputFileName.c_str(), ifstream::binary);
    output.open(outputFileName.c_str());
    cout << "Compressing ..." << endl;
    compress(input, output);
    input.close();
    output.flush();
    output.close();

    if (fileExists(outputFileName)) {
        cout << "Wrote " << fileSize(outputFileName) << " compressed bytes." << endl;
    } else {
        cout << "Compressed output file was not found; perhaps there was an error." << endl;
    }
}

/*
 * Tests the decompress function.
 * Prompts for input/output file names and opens streams on those files.
 * Then calls your decompress function and displays information about how many
 * bytes were written, if any.
 */
void test_decompress() {
    string inputFileName = promptForExistingFileName("Input file name: ");
    ifbitstream input;
    ofstream output;
    string defaultOutputFileName = getRoot(inputFileName) + "-out" + DEFAULT_DECOMPRESSED_FILE_EXTENSION;
    string outputFileName = trim(getLine("Output file name (Enter for "
                                    + defaultOutputFileName + "): "));
    if (outputFileName == "") {
        outputFileName = defaultOutputFileName;
    }
    if (inputFileName == outputFileName) {
        cout << "You cannot specify the same file as both the input file" << endl;
        cout << "and the output file.  Aborting." << endl;
        return;
    }
    if (!confirmOverwrite(outputFileName)) {
        return;
    }

    int inputFileSize = fileSize(inputFileName);
    cout << "Reading " << inputFileSize << " compressed bytes." << endl;
    input.open(inputFileName.c_str());
    output.open(outputFileName.c_str(), ofstream::binary);
    cout << "Decompressing ..." << endl;
    decompress(input, output);
    input.close();
    output.flush();
    output.close();

    if (fileExists(outputFileName)) {
        cout << "Wrote " << fileSize(outputFileName) << " decompressed bytes." << endl;
    } else {
        cout << "Decompressed output file was not found; perhaps there was an error." << endl;
    }
}

/*
 * Tests the freeTree function by freeing the given encoding tree.
 * If the tree is NULL, your freeTree function is supposed to have no effect.
 */
void test_freeTree(HuffmanNode* encodingTree) {
    cout << "Freeing memory for encoding tree ..." << endl;
    freeTree(encodingTree);
}

/*
 * Binary file viewer function.
 * Prompts the user for a file name and then prints all bits/bytes of that file.
 */
void test_binaryFileViewer() {
    string filename = promptForExistingFileName("File name to display: ");
    ifbitstream input;
    input.open(filename.c_str());
    string fileText = readEntireFileText(input);
    input.close();
    cout << "Here is the binary encoded data (" << fileText.length() << " bytes):" << endl;
    printBits(fileText);
}

/*
 * Text file viewer function.
 * Prompts the user for a file name and prints all text in that file.
 */
void test_textFileViewer() {
    string filename = promptForExistingFileName("File name to display: ");
    ifstream input;
    ostringstream output;
    input.open(filename.c_str(), ifstream::binary);
    while (true) {
        int ch = input.get();
        if (input.fail()) {
            break;
        }
        output.put(ch);
    }
    string fileText = output.str();
    cout << "Here is the text data (" << fileText.length() << " bytes):" << endl;
    cout << fileText << endl;
    input.close();
}

/*
 * Side-by-side file comparison function.
 * Prompts for two file names and then checks their contents,
 * printing information about differences between the two.
 * Most of this code is written by Keith Schwarz.
 */
void test_sideBySideComparison() {
    string filename1 = promptForExistingFileName("First file name: ");
    string filename2 = promptForExistingFileName("Second file name: ");
    string fileText1 = readEntireFileText(filename1);
    string fileText2 = readEntireFileText(filename2);

    // compare the two sequences to find a mismatch
    pair<string::const_iterator, string::const_iterator> diff =
        mismatch(fileText1.begin(), fileText1.end(), fileText2.begin());
    if (diff.first != fileText1.end()) {
        ptrdiff_t offset = diff.first - fileText1.begin();
        cout << "File data differs at byte offset " << offset << ":" << endl;
        cout << setw(16) << filename1 << " has value " << setw(3) << (int) (*diff.first)  << " ("
             << toPrintableChar(*diff.first)  << ")" << endl;
        cout << setw(16) << filename2 << " has value " << setw(3) << (int) (*diff.second) << " ("
             << toPrintableChar(*diff.second) << ")" << endl;
        int size1 = fileSize(filename1);
        int size2 = fileSize(filename2);
        if (size1 != size2) {
            cout << "File sizes differ! " << size1 << " vs. " << size2 << " bytes." << endl;
        }
    } else {
        cout << "Files match!" << endl;
    }
}

/*
 * Opens an input stream based on the given parameters and returns a pointer
 * to the stream that was opened.
 * If isFile is true, treats data as a file name and opens that file.
 * If isFile is false, treats data as a string of data and opens a string stream
 * over that data.
 * If isBits is true, opens the equivalent bit input stream rather than byte based.
 */
istream* openInputStream(string data, bool isFile, bool isBits) {
    if (isFile) {
        if (isBits) {
            return new ifbitstream(data);
        } else {
            ifstream* input = new ifstream;
            input->open(data.c_str(), ifstream::binary);
            return input;
        }
    } else {
        if (isBits) {
            return new istringbitstream(bytesToBits(data));
        } else {
            return new istringstream(data);
        }
    }
}

/*
 * Prompts the user to choose between reading from a string or file.
 * If the user wants to read from a string, asks the user to type said string.
 * If the user wants to read from a file, asks the user for the file name.
 * Then opens an input stream for the appropriate type of data and returns
 * a pointer to the stream.
 * The memory for the stream must be freed by the caller.
 */
istream* openStringOrFileInputStream(string& data, bool& isFile, bool isBits) {
    while (true) {
        string choice = toLowerCase(trim(getLine("Read from a s)tring or f)ile? ")));
        if (startsWith(choice, 's')) {
            isFile = false;
            data = getLine("Type the string to process: ");
            if (isBits) {
                // strip spaces because user may have copy/pasted from printBits output
                data = stringReplace(data, " ", "");
                data = stringReplace(data, "\t", "");
            }
            break;
        } else if (startsWith(choice, 'f')) {
            isFile = true;
            data = promptForExistingFileName("File name to process: ");
            break;
        }
    }
    return openInputStream(data, isFile, isBits);
}
