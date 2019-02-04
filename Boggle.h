// This is a .h file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>
#include "lexicon.h"
#include "vector.h"
#include "hashset.h"

using namespace std;

class Boggle {
public:
    Boggle(Lexicon& dictionary, string boardText = "");
    char getLetter(int row, int col);
    string getBoardtext();
    bool checkWord(string word);
    bool humanWordSearch(string word);
    void setNewFoundHumanWord(string word);
    HashSet<string> getFoundHumanWords();
    int getTotalHumanWordsFound();
    Set<string> computerWordSearch();
    int getScoreHuman();
    int getScoreComputer();

    // TODO: add any other member functions/variables necessary
    friend ostream& operator<<(ostream& out, Boggle& boggle);

private:
    Lexicon dict;
    Vector<string> board;
    HashSet<string> foundWords;
    int humanScore = 0;
    int ComScore = 0;

    bool humanWordSearchHelper(string word, string& chosen, int row, int col, HashSet<Vector<int>>& usedCoordinates);
    bool inBound(int row, int col);
    Set<string> computerWordSearchHelper(Set<string>& results, string chosen, int row, int col, HashSet<Vector<int>>& usedCoordinates);
};

#endif // _boggle_h
