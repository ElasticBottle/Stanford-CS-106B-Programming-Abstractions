/*
 * File Name: boggleplay.cpp
 * ----------------------------
 * implements the methods needed to play one game of Boggle
 */

#include "lexicon.h"
#include "Boggle.h"
#include "bogglegui.h"
#include "simpio.h"
#include "console.h"

//Size of the boggle board
static int BOARD_SIZE = 16;

string getBoardText ();

void displayBoardAndStats(Boggle& board);
void checkValidityOfWord(Boggle& board, string word);
void printAndDisplayMsg(string msg);
void computerTurn(Boggle& board);

void playOneGame(Lexicon& dictionary) {

    string boardText = "";
    if (!getYesOrNo("Do you want to generate a random board?")) {
        boardText = getBoardText();
    }
    Boggle board (dictionary, boardText);
    BoggleGUI::labelAllCubes(board.getBoardtext());

    clearConsole();
    cout << "It's your turn!" << endl;

    while(true) {
        displayBoardAndStats(board);
        string word = getLine("Type a word (or Enter to stop): ");
        if (word == "") {
            break;
        }

        checkValidityOfWord(board, word);
    }

    computerTurn(board);
}

/**
 * @brief getBoardText prompts the user of a string of 16 letters.
 * Re-prompts the user for a string if the characters entered are not valid
 * or the length is too short/long
 * @return a string containing a 16 valid letters
 */
string getBoardText() {
    string boardletters = "";
    while (true) {
        boardletters = getLine("enter 16 letter for the board");
        if (boardletters.length() == BOARD_SIZE) {
            bool accepted = true;
            for (int i = 0; i < BOARD_SIZE; ++i) {
                accepted = isalpha(boardletters[i]);
            }
            if (accepted) {
                return boardletters;
            }
        }
        cout << "Please enter a valid 16-letter string of alphabets" << endl;
    }
}

/**
 * @brief displayBoardAndStats displays the human words found and score
 * @param board contains the Boggle object in which the human words found and score are contain within
 */
void displayBoardAndStats(Boggle& board) {
    cout << board << endl;
    cout << "Your words(" << board.getTotalHumanWordsFound() << "): " << board.getFoundHumanWords() << endl;
    cout << "Your score: " << board.getScoreHuman() <<endl;
}

/**
 * @brief checkValidityOfWord A word is valid if it can be found in the Boggle object
 * and is longer than or greater than 4 characters. Error messages are displayed if the word is no valid
 * @param board provides the Boggle object in which the word is checked against
 * @param word provides the string which is searched for in board
 */
void checkValidityOfWord(Boggle& board, string word) {
    if (board.checkWord(word)) {
        if (board.humanWordSearch(word)) {
            board.setNewFoundHumanWord(word);
            string msg = "You found a new word! \"" + toUpperCase(word) + "\"";
            printAndDisplayMsg(msg);
        } else {
            string msg = "the current word is not found in the board!";
            printAndDisplayMsg(msg);
        }
    } else {
        string msg = "You must enter an unfound 4+ letter word from the dictionary.";
        printAndDisplayMsg(msg);
    }
}

/**
 * @brief printAndDisplayMsg clears the console and prints the msg. Updates GUI to show msg too.
 * @param msg provides the string to be displayed on the console and GUI
 */
void printAndDisplayMsg(string msg) {
    clearConsole();
    cout << msg << endl;
    BoggleGUI::setStatusMessage(msg);
}

/**
 * @brief computerTurn runs a search on the Boggle board object to find remaining words.
 * @param board provides the Boggle object in which the search is run on
 * to search for all valid words that the user did not find
 */
void computerTurn(Boggle& board) {
    cout << endl << "It's my turn!" << endl;
    Set<string> computerWords = board.computerWordSearch();
    cout << "My words(" << computerWords.size() << "): " << computerWords << endl;
    cout << "My score: " << board.getScoreComputer() <<endl;
    string msg = "Ha ha ha, I destroyed you. Better luck next time, puny human!";
    cout << msg << endl;
    BoggleGUI::setStatusMessage(msg);
}
