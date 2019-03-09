/*
 * FileName: Boggle.cpp
 * ---------------------
 * Implements the search algorithms for finding individual words
 * and for finding all words as well as various setters and getters
 * for accessing and writing data within the Boggle class
 */

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

/**
 * @brief Boggle::Boggle constructor for the Boggle game
 * @param dictionary provides the Lexicon collection in
 * which the instance variable dict is initialised too
 * @param boardText OPTIONAL allows the user to predefine
 * the letters on the Boggle cubes
 */
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

/**
 * @brief Boggle::getBoardtext returns the letters on a board formatted aas a string
 * @return a string representation of the letter on the board
 */
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

/**
 * @brief Boggle::getLetter retrieves the letter at a particular spot on the board
 * @param row provides the y coordinate for the letter to be retieved
 * @param col provides the x coordinate for the letter to be retieved
 * @return the letter at the position col, row
 */
char Boggle::getLetter(int row, int col) {
    if (row >= WIDTH || row < 0) {
        throw row;
    } else if (col >= WIDTH || col < 0) {
        throw col;
    } else {
       return stringToChar(board.get((row * WIDTH) + col));
    }
}

/**
 * @brief Boggle::checkWord verifies and ensure that the word being search for is a valid boggle word
 * @param word provides the string word to be checked on
 * @return trueif the word found is longer than or equal to four letters and it has not been found before
 */
bool Boggle::checkWord(string word) {
    //suitable to search for: that is, whether it is in the dictionary, long enough to be a valid Boggle word, and has not already been found.
    return dict.contains(toLowerCase(word)) && word.length() >=4 && !foundWords.contains(toUpperCase(word));
}

/**
 * @brief Boggle::humanWordSearch attempts to find a given word on the boggle board. Utilizes humanWordSearchHelper
 * @param word provides the string word that the algorithm will attempt to find on the board
 * @return true if the given word can be found on the boggle board
 */
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

/**
 * @brief Boggle::humanWordSearchHelper is a helper function that attempts to find the string word
 * by recursively exploring the various paths and backtracking when a path cannot be found.
 * @param word provides the word to be searched for in the boggle baord
 * @param chosen represents the current string that is formed so far
 * @param row provides the y coordinate to search for a particular letter
 * @param col provides the x coordinate to search for a particular letter
 * @param usedCoordinates contains a set of coorddinates which have been searched before
 * @return true if the given string word can be found on the boggle board
 */
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

/**
 * @brief Boggle::inBound checks if a particular coordinate is valid
 * @param row provides the y coordinate for the letter to be retieved
 * @param col provides the x coordinate for the letter to be retieved
 * @return true if both row and col is larger than or equal to 0 and less
 * than the width of the boggle board
 */
bool Boggle::inBound(int row, int col) {
    return row >= 0 && row < WIDTH && col >= 0 && col < WIDTH;
}

/**
 * @brief Boggle::getHumanScore updates the score of the human in the display and returns
 * the score of the human.
 * @return an int representing the score of the human
 */
int Boggle::getScoreHuman() {
    BoggleGUI::setScore(humanScore, BoggleGUI::HUMAN);
    return humanScore;
}

/**
 * @brief Boggle::setNewFoundHumanWord updates the foundWords set to contain the new word
 * Also updates the human score (plus 1 for a word of length 4, 1 additional point for every
 * letter after that)
 * @param word provides the word that has been found to be added to the set
 */
void Boggle::setNewFoundHumanWord(string word) {
    foundWords.add(toUpperCase(word));
    humanScore += (word.length() - 3);
}

/**
 * @brief Boggle::getFoundHumanWords returns a HashSet<string> containing all the found human words
 * @return a HashSet<string> containing all the found human words
 */
HashSet<string> Boggle::getFoundHumanWords() {
    return foundWords;
}

/**
 * @brief Boggle::getTotalHumanWordsFound returns an int representing the number of words found
 * @return an int representing the number of words found
 */
int Boggle::getTotalHumanWordsFound () {
    return foundWords.size();
}

/**
 * @brief Boggle::computerWordSearch utilizes a helper function called computerWordSearchHelper to find
 * all words on the board not found by the human
 * @return a set<string> containing all the remaining words on the boggle board
 */
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

/**
 * @brief Boggle::computerWordSearchHelper is a helper function. Utilizes recursive backtracking to
 * find all remaining valid words on the boggle board.
 * @param results contains the list of words found by the computer.
 * @param chosen contains the current string of words that the computer is working on
 * @param row represents the y coordinate of the board that is currently being explored
 * @param col represents the x coordinate of the board that is currentl ybeign explored
 * @param usedCoordinates is a HashSet containing the coordinates that have been explored
 * @return a set<String> of words that contains all the valid words on the baord not found by the humuan
 */
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

/**
 * @brief Boggle::getScoreComputer updates the score of the computer in the display and returns
 * the score of the computer.
 * @return an int representing the score of the computer
 */
int Boggle::getScoreComputer() {
    BoggleGUI::setScore(ComScore, BoggleGUI ::COMPUTER);
    return ComScore;
}

/**
 * @brief operator << overloads the "<<" to allow for printing of board with the "<<" operator
 */
ostream& operator<<(ostream& out, Boggle& boggle) {
    string boardText = boggle.getBoardtext();
    cout << boardText;

    return out;
}
