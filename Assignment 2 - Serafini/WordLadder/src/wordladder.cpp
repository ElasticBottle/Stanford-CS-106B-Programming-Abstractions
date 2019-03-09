/*
 * File Name: Wordladder.cpp
 * --------------------------
 * Prompts user for two english words and
 * figures out the shortest path from one word english word to another
*/

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "set.h"
#include "stack.h"
#include "queue.h"
#include "filelib.h"

using namespace std;

const int FIRST_WORD = 1;
const int SECOND_WORD = 2;
const int ALPHABET_LENGTH = 26;

void printIntroText();
void promptForFile(ifstream & input);
void initializeDict (ifstream & input, Set<string> & dictionary);
string getWord (const int & wordNumber);
bool checkForValidity (const string & beginWord, const string & endWord, const Set<string> & dictionary);
Stack<string> findWordLadder (const string & beginWord, const string & endWord, const Set<string> & dictionary);
bool inStack(const string word, Stack<string> currentStack);
void print(Stack<string> & answer);

int main() {
    //prints the premise of the program
    printIntroText();

    //Prompts user for the dictionary file to be used
    ifstream input;
    promptForFile(input);

    //initializes the dictionary file into a set
    Set<string> dictionary;
    initializeDict(input, dictionary);

    while (true) {
        string beginWord, endWord;
        while(true){
            //get the first word while ensuring it's a valid english word
            beginWord = getWord(FIRST_WORD);
            if(beginWord == "") break;

            //get the second word while ensuring it's a valid english word and same length to first word
            endWord = getWord(SECOND_WORD);
            if(endWord == "") break;

            //checks the validity of the two words, quits if they are valid
            if (checkForValidity(beginWord, endWord, dictionary)) break;
        }
        if(beginWord == "" || endWord == "") break;

        //computing the word ladder between the two words
        cout << "a ladder from " << endWord << " back to " << beginWord <<":" << endl;
        Stack<string> answer = findWordLadder(beginWord, endWord, dictionary);
        print(answer);
        cout <<endl <<endl;
    }

    cout << "Have a nice day." << endl;
    return 0;
}

/**
 * @brief printIntroText prints out the introductory text into the program by reading them from the file introText.txt
 */
void printIntroText() {
    ifstream inputReader;
    inputReader.open("introText.txt");
    string line;
    while (getline(inputReader, line)) {
        cout << line << endl;
    }
    inputReader.close();
    cout << endl;
}

/**
 * @brief promptForFile prompts user for a file to be used as the dictionary for this run of the program
 * @param input provides the ifstream reader to be used in the opening of specified file.
 * prints an error message if the filename specified is not an actual filename and re-prompts the user for a file.
 */
void promptForFile(ifstream & input) {
    while (true) {
        string fileName;
        cout << "Dictionary file name? ";
        getline (cin, fileName);
        input.open(fileName.c_str());

        if (!input.fail()) {
            cout << endl;
            break;
        }

        cout << "Unable to open that file. Try again." << endl;
    }
}

/**
 * @brief initializeDict Reaads the word from a specified dicitonary file into a Set data structure
 * @param input provides the ifstream reader opened to the specified dictionary file from promptForFile method
 * to allow contents from the file to be read line by line
 * @param dictionary provides the Set data structure in which the read words from input can be stored into.
 */
void initializeDict (ifstream & input, Set<string> & dictionary) {
   string word;
   while (getline(input, word)) {
       dictionary.add(word);
   }
   input.close();
}

/**
 * @brief getWord prompts the user for a word
 * @param wordNumber specifies the word that is being asked i.e. 1 or 2
 * @return the word read from the user's input
 */
string getWord (const int & wordNumber){
    string word;
    cout << "Word #" << wordNumber << " (or Enter to quit): ";
    getline (cin, word);
    return word;
}

/**
 * @brief checkForValidity checks the valididty of the two words parsed by the ifstream reader from the user.
 * @param beginWord provides the value of word#1 for comparison against endWord
 * @param endWord provides the value for word#2 for comparison against beginWord
 * @param dictionary provides the values of accepted words in the dictionary specified
 * @return true only if the follwing conditions are met:
 * -The two words are of same length
 * -The two words are not the exact same thign
 * -The two words appear in the specified dictionary earlier
 * Otherwise, the appropriate error messages are printed, and false is returned
 */
bool checkForValidity (const string & beginWord, const string & endWord, const Set<string> & dictionary) {
    if (beginWord.length() != endWord.length()) {
        cout << "The two words must be the same length" <<endl <<endl;
        return false;
    } else if (beginWord == endWord) {
        cout <<"The two words must be different" <<endl <<endl;
        return false;
    } else if (dictionary.contains(toLowerCase(beginWord)) && dictionary.contains(toLowerCase(endWord))) {
        return true;
    } else {
        cout << "The two words must be found in the dictionary." << endl <<endl;
        return false;
    }
}

/**
 * @brief findWordLadder conduct a breadth first search to enumerate over the possible words
 * @param beginWord provides the word at the start to begin the search from
 * @param endWord provides the word at the end to let the program know when to terminate
 * @param dictionary provides the Set<string> to allow the program to check for memebership of the
 * words that have been enumerated over.
 * @return Stack<string> which contains the stack of word from the beginWord to the endWord,
 * changing one letter at a time.
 */
Stack<string> findWordLadder (const string & beginWord, const string & endWord, const Set<string> & dictionary) {
    //creating the queue and pushing the first stack into it
    Stack<string> firstSearch;
    firstSearch.push(toLowerCase(beginWord));
    Queue<Stack<string>> wordLadder;
    wordLadder.enqueue(firstSearch);

    //runs the Breadth First Sarch
    while(!wordLadder.isEmpty()) {
        //
        Stack<string> currentStack = wordLadder.dequeue();
        string currentWord = currentStack.peek();
        Set<string> usedWords;

        //iterates over each of the alphabet in currentWord, changing the alphabet of the word.
        for (int i = 0; i < currentWord.size(); ++i) {
            for (int j = 0; j < ALPHABET_LENGTH; j++) {
                string toModify = currentWord;
                toModify[i] = ((toModify[i] - 'a' + j) % 26) + 'a';
                if(dictionary.contains(toModify) && !inStack(toModify, currentStack)) {
                    //cout << toModify<<endl;
                    if (toModify == endWord) {
                        currentStack.push(endWord);
                        return currentStack;
                    } else if (!usedWords.contains(toModify)){
                        Stack<string> copy = currentStack;
                        copy.push(toModify);
                        wordLadder.enqueue(copy);
                        usedWords.add(toModify);
                        //cout <<wordLadder<<endl;
                    }
                }
            }
        }
    }
    return firstSearch;
}

/**
 * @brief inStack checks to see if a given string is in a stack
 * @param word provides the word to check the stack for
 * @param comparisonStack provides the Stack<string> for word to be checked against
 * @return true if word is found in comparisonStack
 */
bool inStack(const string word, Stack<string> comparisonStack) {
    int size = comparisonStack.size();
    for (int i = 0; i < size; ++i) {
       string comparison = comparisonStack.pop();
       if (word == comparison) {
           return true;
       }
    }
    return false;
}

/**
 * @brief print puts the answer onto the string starting with the second word that the user provided
 * @param answer provides the Stack data structure for the method to print
 */
void print(Stack<string> & answer) {
    int size = answer.size();
    for (int i = 0; i < size; ++i) {
       string word = answer.pop();
       if (i) cout << " ";
       cout << word;
    }
}
