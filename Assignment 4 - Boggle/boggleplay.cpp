// This is a .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

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

void displayBoardAndStats(Boggle& board) {
    cout << board << endl;
    cout << "Your words(" << board.getTotalHumanWordsFound() << "): " << board.getFoundHumanWords() << endl;
    cout << "Your score: " << board.getScoreHuman() <<endl;
}

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

void printAndDisplayMsg(string msg) {
    clearConsole();
    cout << msg << endl;
    BoggleGUI::setStatusMessage(msg);
}

void computerTurn(Boggle& board) {
    cout << endl << "It's my turn!" << endl;
    Set<string> computerWords = board.computerWordSearch();
    cout << "My words(" << computerWords.size() << "): " << computerWords << endl;
    cout << "My score: " << board.getScoreComputer() <<endl;
    string msg = "Ha ha ha, I destroyed you. Better luck next time, puny human!";
    cout << msg << endl;
    BoggleGUI::setStatusMessage(msg);
}
