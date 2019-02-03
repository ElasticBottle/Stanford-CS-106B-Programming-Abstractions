// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own.
// TODO: remove this comment header!

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "grid.h"
#include "gwindow.h"
#include "simpio.h"
#include "lifegui.h"
#include "stack.h"
#include "vector.h"
using namespace std;

/*CONSTANTS*/
const string PROMPT= "Grid input file name? ";
const int DELAY = 50;

//void promptUserForFile1(ifstream & input, string prompt);
void printIntroText();
int getNumber(ifstream& input, string& toConvert);
void readGrid(Grid<char> & world, ifstream & inputReader);
void displayGrid(const Grid<char> & world);
int getNumFrames();
void animate (const int frames, Grid<char> & world);
void updateForNextGen(Grid<char> & world);
Stack<Vector<int>> checkForUpdate(const Grid<char> & world);
bool checkUpdateOneSpot(const int i, const int j, const Grid<char> & world);
void updateWorld(Grid<char> & world, Stack<Vector<int>> & toUpdate);

int main() {
    printIntroText();
    ifstream inputReader;
    promptUserForFile(inputReader, PROMPT);

    int rows, columns;
    string toConvert;
    rows = getNumber(inputReader, toConvert);
    columns = getNumber(inputReader, toConvert);

    Grid<char> world (rows, columns);
    readGrid(world, inputReader);
    inputReader.close();
    displayGrid(world);

    string choice;
    while (true){
        choice = getLine ("a)nimate, t)ick, q)uit? ");
//        cout << "a)nimate, t)ick, q)uit? ";             alternate method
//        getline(cin, choice);

        if (toLowerCase(choice) == "q" ) break;
        if (toLowerCase(choice) == "a") {
           int frames = getNumFrames();
            animate(frames, world);
        } else if (toLowerCase(choice) == "t") {
            updateForNextGen(world);
        } else {
            cout << "Invalid choice; please try again." << endl;
        }
    }

    cout << "Have a nice Life!" << endl;
    return 0;
}

//attempt to implement my own version of Stanford's promptUserForFile function to understand how it works
//void promptUserForFile1(ifstream & input, string prompt) {
//    while (true) {
//        cout << prompt;
//        string filename;
//        getline (cin, filename);
//        input.open(filename.c_str());
//        if (!input.fail()) break;
//        cout << "Invalud filename, try again" << endl;
//    }
//}

/**
 * @brief printIntroText prints the introductory text to the program, explaining the rules to the user
 * Reads the introText.txt file for text.
 */
void printIntroText() {
    ifstream input;
    input.open("introText.txt");
    string line;
    while (getline(input, line)) {
        cout << line << endl;
    }
    cout << endl;
}

/**
 * @brief getNumber is used to read the first two lines from the Grid input file, which are numbers,
 * and converts them from the string to an int
 * @param input provides the ifstream needed to read the needed number from the file
 * @param toConvert provides a string var to store the read line which is then converted to an int
 * @return returns toConvert after parsing and converting it to an int
 */
int getNumber(ifstream& input, string& toConvert) {
    getline(input, toConvert);
    return stringToInteger(toConvert);
}

/**
 * @brief readGrid reads the grid input file for the appropriate grid and stores them into a grid within the program
 * @param world provides the grid in which the contents of the grid file are read into.
 * @param inputReader provides the ifstream needed to read the file
 */
void readGrid(Grid<char> & world, ifstream & inputReader) {
    for (int i = 0; i < world.numRows(); ++i) {
        for (int j = 0; j < world.numCols(); ++j) {
           world[i][j] = inputReader.get();
        }
        inputReader.get();
    }
}

/**
 * @brief displayGrid prints a Grid onto the console
 * @param world rpovides the Grid to be printd onto the console
 */
void displayGrid(const Grid<char> & world) {
    for (int i = 0; i < world.numRows(); ++i) {
        for (int j = 0; j < world.numCols(); ++j) {
            cout << world[i][j];
        }
        cout << endl;
    }
}

/**
 * @brief getNumFrames gets the number of ticks to be carried out be the program in succession
 * @return the number of ticks that the user wants
 */
int getNumFrames() {
    int frames = getInteger("How many frames? ");
    return frames;
}

/**
 * @brief animate displays a certain number of ticks consecutively based on frames param specified
 * by clearing the console and printing the grid at intervals of 50ms as specified by the DELAY constant
 * @param frames specifies the numbers of ticks to be carreid out
 * @param world provides the Grid which will be iterated. Each iteration will be displayed as a tick.
 */
void animate (const int frames, Grid<char> & world) {
    for (int i = 0; i < frames; ++i) {
       clearConsole();
       updateForNextGen(world);
       pause(DELAY);
    }
}

/**
 * @brief updateForNextGen updates the grid displayed on the console with the next iteration based on the rules
 * @param world porvides the Grid for the rules of the Game of LIfe to be applied on so that the next iteration can be determined
 */
void updateForNextGen(Grid<char> & world) {
    Stack<Vector<int>> toUpdate = checkForUpdate(world);
    updateWorld(world, toUpdate);
    displayGrid(world);
}

/**
 * @brief checkForUpdate checks to see if the individual coordinates of a Grid requires updating based on the rules of the Game of Life
 * @param world provides the Grid for which the rules will be applied on to determine the next iteration
 * @return a Stack containing the coordinates within the grid for which changes have to made
 */
Stack<Vector<int>> checkForUpdate(const Grid<char> & world){
    Stack<Vector<int>> toUpdate;

    for (int i = 0; i < world.numRows(); ++i) {
        for (int j = 0; j < world.numCols(); ++j) {
            if (checkUpdateOneSpot(i, j, world)) {
                Vector<int> coordinate;
                coordinate.add(i);
                coordinate.add(j);
                toUpdate.push(coordinate);
            }
        }
        //cout << endl;
    }
    //cout << toUpdate << endl;

    return toUpdate;
}

/**
 * @brief checkUpdateOneSpot checks the adjacent cells of a coordinate and counts the number of 'X'.
 * Aything with 0, 1, 4 or more 'X' will result in the cell being '-' in the next iteration.
 * ANything with 3 'X' will result in the cell having a 'X' in the next iteration
 * @param rowNum the specific row within the Grid being examined
 * @param colNum the specific column within the Grid being examined
 * @param world provides the Grid in which the specific row and column exist to be examined
 * @return true if the coordinate on the Grid in question requires updating
 */
bool checkUpdateOneSpot(const int rowNum, const int colNum, const Grid<char> & world) {
    int counter = 0;

    //counts the number of 'X' adjacent to the cell in question, including itself.
    for (int i = rowNum - 1; i < rowNum + 2; ++i) {
        for (int j = colNum - 1; j < colNum + 2; ++j) {
                if (world.inBounds(i, j) && world.get(i , j) == 'X') {
                    counter ++;
            }
        }
    }

    //reduces counter by 1 if the cell in question contains an 'X'
    if (world.get(rowNum, colNum) == 'X') {
        counter --;
    }

    //cout << counter;

    //Returning of values depending on the four rules for the Game of Life.
    if (counter == 0 || counter == 1 || counter >= 4) {
        return world.get(rowNum, colNum) == 'X';
    } else if (counter == 3) {
        return world.get(rowNum, colNum) == '-';
    } else {
        return false;
    }
}

/**
 * @brief updateWorld udpates the Grid by inverting the coordinates the needs updating.
 * A grid with 'X' become '-', while a grid with '-' becomes 'X'
 * @param world provides the Grid in which the updating will be performed on
 * @param toUpdate provides the Stack of coordinates that requires updating.
 */
void updateWorld(Grid<char> & world, Stack<Vector<int>> & toUpdate){
    int size = toUpdate.size();
    for (int i = 0; i < size; ++i) {
        Vector<int> coordinate = toUpdate.pop();
        int row = coordinate.get(0);
        int col = coordinate.get(1);
        if (world.get(row,col) == '-') {
            world.set(row, col, 'X');
        } else {
            world.set(row, col, '-');
        }
    }
}
