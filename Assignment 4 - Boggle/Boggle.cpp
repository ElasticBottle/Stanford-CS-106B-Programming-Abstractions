// This is a .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include "Boggle.h"
#include "random.h"
#include "shuffle.h"
#include "simpio.h"
#include "hashset.h"
#include "bogglegui.h"


//Size of the boggle board
static int BOARD_SIZE = 16;

//Row/Column in the boggle board
static int WIDTH = 4;

//Size of the boggle board
static int DELAY = 100;

// letters on all 6 sides of every cube
static string CUBES[16] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

// letters on every cube in 5x5 "Big Boggle" version (extension)
static string BIG_BOGGLE_CUBES[25] = {
   "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
   "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
   "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
   "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
   "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};

Boggle::Boggle(Lexicon& dictionary, string boardText) {
    dict = dictionary;
    if (boardText == "") {
        for (int i = 0; i < BOARD_SIZE; ++i) {
           char cubeLetter = CUBES[i][randomInteger(0, 5)];
           board.add(charToString(cubeLetter));
        }
        shuffle(board);
    } else {
        boardText = toUpperCase(boardText);
        for (int i = 0; i < boardText.size(); ++i) {
           board.add(charToString(boardText[i]));
        }
    }

}

string Boggle::getBoardtext() {
    string boardText = "";
    for (int i = 0; i < WIDTH; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
           boardText += charToString(getLetter(i, j));
        }
        boardText += "\n";
    }
    return boardText;
}

char Boggle::getLetter(int row, int col) {
    if (row >= WIDTH || row < 0) {
        throw row;
    } else if (col >= WIDTH || col < 0) {
        throw col;
    } else {
       return stringToChar(board.get((row * 4) + col));
    }
}

bool Boggle::checkWord(string word) {
    //suitable to search for: that is, whether it is in the dictionary, long enough to be a valid Boggle word, and has not already been found.
    return dict.contains(toLowerCase(word)) && word.length() >=4 && !foundWords.contains(toUpperCase(word));
}

bool Boggle::humanWordSearch(string word) {
    HashSet<Vector<int>> usedCoordinates;
    string chosen = "";
    word = toUpperCase(word);
    BoggleGUI::clearHighlighting();

    bool answer = false;
    for (int i = 0; i < WIDTH; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            answer = humanWordSearchHelper(word, chosen, i, j, usedCoordinates);
            if (answer) {
                return answer;
            }
        }
    }
    return answer;
}

bool Boggle::humanWordSearchHelper(string word,
                                   string& chosen,
                                   int row ,
                                   int col,
                                   HashSet<Vector<int>>& usedCoordinates) {
    bool answer = false;
    Vector<int> coordinates;
    coordinates.add(row);
    coordinates.add(col);


    if (chosen == word) {
        BoggleGUI::recordWord(chosen, BoggleGUI::HUMAN);
        return true;
    } else if (getLetter(row , col) == word[chosen.length()]) {
        BoggleGUI::setHighlighted(row, col, true);
        BoggleGUI::setAnimationDelay(DELAY);
        chosen += getLetter(row , col);
        usedCoordinates.add(coordinates);

        for (int i = row - 1; i <= row + 1; ++i) {
            for (int j = col - 1; j <= col + 1; ++j) {
                Vector<int> newCoordinates;
                newCoordinates.add(i);
                newCoordinates.add(j);
                if (inBound(i , j)
                        && (!usedCoordinates.contains(newCoordinates)
                            || word.length() == chosen.length())) {

                    answer = humanWordSearchHelper(word, chosen, i, j, usedCoordinates);
                    if (answer) return answer;

                }
            }
        }

        chosen.erase(chosen.length() - 1);
        usedCoordinates.remove(coordinates);
    }

    BoggleGUI::setHighlighted(row, col, true);
    BoggleGUI::setAnimationDelay(DELAY);
    BoggleGUI::setHighlighted(row, col, false);
    return answer;
}

bool Boggle::inBound(int row, int col) {
    return row >= 0 && row < WIDTH && col >= 0 && col < WIDTH;
}


int Boggle::getScoreHuman() {
    BoggleGUI::setScore(humanScore, BoggleGUI::HUMAN);
    return humanScore;
}

void Boggle::setNewFoundHumanWord(string word) {
    foundWords.add(toUpperCase(word));
    humanScore += (word.length() - 3);
}

HashSet<string> Boggle::getFoundHumanWords() {
    return foundWords;
}

int Boggle::getTotalHumanWordsFound () {
    return foundWords.size();
}

Set<string> Boggle::computerWordSearch() {
    Set<string> results;
    string chosen = "";
    HashSet<Vector<int>> usedCoordinates;

    for (int i = 0; i < WIDTH; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            results += computerWordSearchHelper(results, chosen, i, j, usedCoordinates);
        }
    }

    return results;
}

Set<string> Boggle::computerWordSearchHelper(Set<string>& results, string chosen, int row, int col, HashSet<Vector<int>>& usedCoordinates) {
    Vector<int> coordinates;
    coordinates.add(row);
    coordinates.add(col);

    if (checkWord(chosen)) {
        foundWords.add(chosen);
        results.add(chosen);
        ComScore += chosen.length() - 3;
        BoggleGUI::recordWord(chosen, BoggleGUI::COMPUTER);
        return results;
    } else if (dict.containsPrefix(chosen) && !usedCoordinates.contains(coordinates)) {
        //choose
        chosen += getLetter(row , col);
        usedCoordinates.add(coordinates);

        //explore
        for (int i = row - 1; i <= row + 1; ++i) {
            for (int j = col - 1; j <= col + 1; ++j) {
                if (inBound(i, j)) {
                    computerWordSearchHelper(results, chosen, i, j, usedCoordinates);
                }
            }
        }

        //unchoose
        chosen.erase(chosen.length() - 1);
        usedCoordinates.remove(coordinates);
    }
    return results;
}

int Boggle::getScoreComputer() {
    BoggleGUI::setScore(ComScore, BoggleGUI ::COMPUTER);
    return ComScore;
}

ostream& operator<<(ostream& out, Boggle& boggle) {
    string boardText = boggle.getBoardtext();
    cout << boardText;

    return out;
}
