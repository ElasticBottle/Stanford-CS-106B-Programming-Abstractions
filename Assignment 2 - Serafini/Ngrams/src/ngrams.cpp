/*
 * FileName: ngrams.cpp
 * ---------------------
 * reads an input file and uses it to build a data structure of word groups
 * as a basis for randomly generating new text that sounds like it came
 * from the same author as that file.
*/

//#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "map.h"
#include "vector.h"
#include "queue.h"
#include "strlib.h"
#include "simpio.h"
#include "random.h"

using namespace std;

/*Constants*/
const string FILE_PROMPT = "Input file name? ";
const string INT_PROMPT = "Value of N? ";
const string GEN_PROMPT = "# of random words to generate (0 to quit)? ";
const int SENTINEL = 0;

/*function prototypes*/
void printIntroText();
ifstream getNGramFile(const string prompt, string & nGramFilename);
Map <Queue<string>, Vector<string>> buildNGramMap(ifstream & input, const int NVal, const string nGramFilename, const int totalWords);
int getWordCount (ifstream & input, const string nGramFilename);
void generateRandomText(const Map <Queue<string>, Vector<string>> & NGramMap, const int wordsToGen);
void print (Queue<string> & seedKey);

int main() {
    printIntroText();

    ifstream input;
    string nGramFilename;
    input = getNGramFile(FILE_PROMPT, nGramFilename);
    int totalWords = getWordCount(input, nGramFilename);
    int NVal;
    while (true) {
        NVal = getInteger(INT_PROMPT);
        if (NVal < totalWords) break;
        cout << "NVal is too large!" << endl;
    }

    Map <Queue<string>, Vector<string>> NGramMap = buildNGramMap(input, NVal, nGramFilename, totalWords);

    while (true) {
        int wordsToGen;
        // prompts user for the number of words they want to generate
        while (true) {
            wordsToGen = getInteger(GEN_PROMPT);
            if (wordsToGen > NVal || wordsToGen == 0) break;
            cout << "The number of words you want to generate has to be more than NVal!" <<endl;
        }

        //way to exit out of loop
        if (wordsToGen == SENTINEL) break;

        //prints the wordsToGen
        generateRandomText(NGramMap, wordsToGen);
        cout << endl <<endl;
    }

    cout << "Exiting." << endl;
    return 0;
}

/**
 * @brief printIntroText uses an ifstream to read the contents of introText.txt to be printed onto console
 */
void printIntroText(){
    ifstream input;
    input.open("introText.txt");
    string line;
    while (getline(input, line)){
        cout << line << endl;
    }
    input.close();
}

/**
 * @brief getNGramFile pormpts user for a valid name to the nGram file that will be used for this console session.
 * The user is re-prompted id the filename provided is not valid.
 * @param prompt provides the text that will be outputted to the console when prompting the user for a filename
 * @param nGramFilename is the string variable that the user's input will be read into. Passed by reference to allow the
 * same file to be referenced again later on.
 * @return an ifstream opened to the nGram file to be used for the particular console session
 */
ifstream getNGramFile(const string prompt, string & nGramFilename) {
    cout << endl;
    ifstream input;
    while (true) {
        cout << prompt;
        getline(cin, nGramFilename);
        input.open(nGramFilename);
        if (!input.fail()) break;
        cout << "Unable to open that file.  Try again." << endl;
    }
    return input;
}

/**
 * @brief buildNGramMap creates a Map<Queue<string>, Vector<string>> which maps Queue<string> containing
 * NVal number of words to the word(s) that come after that phrase of words.
 * @param input provides the reader opened to the file in which the NGramMap will be built out off
 * @param NVal provides the value for the size of the queue. Longer NVal will result in NGramMaps
 * that provide phrasing closer to the text itself
 * @param nGramFilename correspond to the filename which allows the fileStream to be reopened again
 * if it reaches the end
 * @param totalWords provides the total number of words in the file
 * @return a Map<Queue<string>, Vector<string>>.
 */
Map <Queue<string>, Vector<string>> buildNGramMap(ifstream & input
                                                  , const int NVal
                                                  , const string nGramFilename
                                                  , const int totalWords) {
    Map <Queue<string>, Vector<string>> NGramMap;
    Queue <string> key;
    for (int i = 0; i < NVal - 1; ++i) {
       string word;
       input >> word;
       key.enqueue(word);
    }
    for (int i = 0; i < totalWords; i ++) {
        string wordRead;
        if (!(input >> wordRead)) {
            input.close();
            input.open(nGramFilename);
            input >> wordRead;
        }

        if (NGramMap.containsKey(key)) {
            Vector<string> toUpdate = NGramMap.get(key);
            toUpdate.add(wordRead);
            NGramMap.put(key, toUpdate);
        } else {
            Vector<string> values;
            values.add(wordRead);
            NGramMap.put(key, values);
        }

        key.dequeue();
        key.enqueue(wordRead);
    }



    return NGramMap;
    /*
     *Thoughts on how to build an NGram:
     *  Figure out the total number of words in the whole text. store that value in totalWord
     *  create a map to prep for storage of values
     *  read NVal-1 words into a queue as the first key
     *  for totalWord number of times:
     *      if !possible to retrieve a possible string value.
     *          reset NGram file
     *          get string value
     *
     *      if the map already contains the key, get the vector from it and add the string value to it.
     *      update the map to reflect the new value associated with the key.
     *
     *      otherwise, store the string value in a new vector.
     *      Add the new key, value pair to the map.
     *
     *      pop the one value off the queue and push in the string value.
    */
}

/**
 * @brief getWordCount reads through the specified nGram file
 * word by word to count the total number of words as delineated by whitespaces
 * @param input provides the ifstream opened to the nGramFile whose words will be counted
 * @param nGramFilename allows the file to be reseted
 * after counting by closing input and reopening it to the same fileName.
 * @return an int which represents the number of words counted in the specified file.
 */
int getWordCount (ifstream & input, const string nGramFilename) {
    int wordCount = 0;
    string word;
    while (input >> word) {
        wordCount++;
    }
    input.close();
    input.open(nGramFilename);
    return wordCount;
}

/**
 * @brief generateRandomText creates random text based on an NGramMap
 * @param NGramMap provides the NGramMap to generate random text from
 * @param wordsToGen provides the number of random words to generate
 */
void generateRandomText(const Map <Queue<string>, Vector<string>> & NGramMap, const int wordsToGen) {
    cout << "...";
    Vector<Queue<string>> keySet = NGramMap.keys();
    int seedVal = randomInteger(0, keySet.size() - 1);
    Queue<string> seedKey = keySet.get(seedVal);
    print(seedKey);
    for (int i = 0; i < wordsToGen - seedKey.size(); ++i) {
       Vector<string> possibleNextWords =  NGramMap.get(seedKey);
       int nextWordVal = randomInteger( 0, possibleNextWords.size() - 1);
       string nextWord = possibleNextWords.get(nextWordVal);
       cout << nextWord << " ";

       seedKey.dequeue();
       seedKey.enqueue(nextWord);
    }
    cout << "...";

 /*
  *Thoughts on how to generate random text:
  * print "... "
  * choose a random key from the map and store it into a queue
  * print the key queue
  * For wordsToGen - NVal - 1 number of times:
  *     Using the key queue, choose a random value for the key from the map.
  *     print the value with a space.
  *
  *     pop off one value from the queue and push the value on.
  * print "..."
 */
}

/**
 * @brief print prints a Queue<string> in the format of "word_" for the length of the queue
 * @param seedKey provides the queue containing the string to print.
 */
void print (Queue<string> & seedKey) {
    int size = seedKey.size();
    for (int i = 0; i < size; ++i) {
       string word = seedKey.dequeue();
       cout << word << " ";
       seedKey.enqueue(word);
    }
}
