#include <cctype>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "vector.h"
#include "set.h"
#include "map.h"
#include "tokenscanner.h"
#include "gobjects.h"
#include "gwindow.h"
#include "gevents.h"
#include "gbufferedimage.h"
#include "simpio.h"
#include "recursion.h"
using namespace std;

// constants
const int FLOOD_FILL_NUM_SHAPES = 100;
const int FLOOD_FILL_MIN_RECT_SIZE = 20;
const int FLOOD_FILL_MAX_RECT_SIZE = 100;
const int FRACTALS_LEFT_X = 100;
const int FRACTALS_LEFT_Y = 80;
const int FRACTALS_SIZE = 400;

// high level helpers
void showSplash(GWindow & w);
void waitToReturnToMenu();
void invalidChoice();

// the commands
void demoDefinition();
void demoFractals(GWindow & w);
void demoExploration(GWindow & w);
void makePersonalCurriculum();
void generateProblem();

// more mundane helpers
Map<string, Vector<string>> loadPrereqMap(string fileName);

int main() {
    GWindow w(600, 500);

    while(true) {
        showSplash(w);
        cout << "Welcome to Meta Academy. Coming online soon..." << endl;
        cout << "1. Demo recursion by definition" << endl;
        cout << "2. Demo recursion for fractals" << endl;
        cout << "3. Demo recursion for exploration" << endl;
        cout << "4. Personal curriculum" << endl;
        cout << "5. Generate question" << endl;
        cout << "6. Exit" << endl;
        int choice = getInteger("What do you want? ");
        if(choice == 6) break;
        switch(choice) {
        case 1: demoDefinition(); break;
        case 2: demoFractals(w); break;
        case 3: demoExploration(w); break;
        case 4: makePersonalCurriculum(); break;
        case 5: generateProblem(); break;
        default: invalidChoice(); break;
        }
        cout << endl;
        clearConsole();
    }
    cout << "Have a nice day!" << endl;
    return 0;
}

/**
 * Function: Demo Definition
 * -------------------------
 * Teaches students about recursion by definition then shows an interactive
 * demo of Euclids GCD algorithm.
 */
void demoDefinition() {
    cout << endl;
    cout << "Some operations are much easier to define recursively. ";
    cout << "One amazing example of this is Euclid's Algorithm to ";
    cout << "calculate the greatest common divisor (gcd). In the algorithm ";
    cout << "Euclid famously shows that the gcd(a, b) is equal to ";
    cout << "gcd(b, r) where r is the remainder when you divide a by b. ";
    cout << "In the case where b is equal to 0, gcd(a, 0) is simply a. ";
    cout << "Since gcd is defined recursively, it is much easier to program ";
    cout << "using recursion. Let's calculate gcd.";
    cout << endl;

    int a = getInteger("Enter a: ");
    int b = getInteger("Enter b: ");

    int result = gcd(a, b);

    cout << "The greatest common divisor of " << a << " and " << b;
    cout << " is " << result << endl;

    waitToReturnToMenu();
}

/**
 * Function: Demo Fractals
 * -------------------------
 * Teaches students about fractal recursion then shows an interactive
 * demo of the Serpinskii triangle.
 */
void demoFractals(GWindow & w) {
    cout << endl;
    cout << "Recursion can also create incredible graphic designs! ";
    cout << "One example is the Sierpinski Triangle, named after the ";
    cout << "Polish mathematician Waclaw Sierpinski. Given a triangle, ";
    cout << "you can recursively generate subtriangles to produce an ";
    cout << "intricate fractal image. An order-1 Sierpinski Triangle is ";
    cout << "an equilateral triangle. To create an order-K Sierpinski ";
    cout << "Triangle, you draw three Sierpinski Triangles of order K-1 ";
    cout << "within the original. Watch the order grow and the simple ";
    cout << "triangle turn into a beautiful fractal!" << endl;
    w.clear();
    int leftX = FRACTALS_LEFT_X;
    int leftY = FRACTALS_LEFT_Y;
    for(int i = 1; i <= 4; i++) {
        cout << "Here is serpinskii order " << i << endl;
        serpinskii(w, leftX, leftY, FRACTALS_SIZE, i);
        getLine("Press enter to contine: ");
        w.clear();
    }

    cout << "Here is serpinskii order 7" << endl;
    serpinskii(w, leftX, leftY, FRACTALS_SIZE, 7);
    waitToReturnToMenu();
}


/**
 * Function: Demo Exploration
 * -------------------------
 * Teaches students about recursive exploration then shows an interactive
 * demo of flood fill.
 */
void demoExploration(GWindow &window) {
    cout << endl;
    cout << "One great use of recursion is when you want to explore! ";
    cout << "Often decision making is recursive. Once you make one decision ";
    cout << "you find yourself in a situation which is of the same format. ";
    cout << "Here we will show how recursion can be used to explore a region ";
    cout << "of pixels. We will recursively try all possible combinations of ";
    cout << "making the decisions: move up a pixel, move left, move right and ";
    cout << "move down." << endl;

    GObject::setAntiAliasing(false);

    Map<string, int> colorMap;
    colorMap["Red"]    = 0x8c1515;   // Stanford red
    colorMap["Yellow"] = 0xeeee00;   // yellow
    colorMap["Blue"]   = 0x0000cc;   // blue
    colorMap["Green"]  = 0x00cc00;   // green
    colorMap["Purple"] = 0xcc00cc;   // purple
    colorMap["Orange"] = 0xff8800;   // orange
    Vector<string> colorVector = colorMap.keys();

    GLabel fillLabel("Fill color:");
    GChooser colorList;
    for (string key : colorMap) {
        colorList.addItem(key);
    }
    GButton done("Done");
    window.addToRegion(&fillLabel, "SOUTH");
    window.addToRegion(&colorList, "SOUTH");
    window.addToRegion(&done, "SOUTH");

    // use buffered image to store individual pixels
    GBufferedImage floodFillPixels(
                /* x */ 0,
                /* y */ 0,
                /* width */ window.getWidth(),
                /* height */ window.getHeight(),
                /* rgb fill */ 0xffffff);

    for (int i = 0; i < FLOOD_FILL_NUM_SHAPES; i++) {
        double w = randomInteger(FLOOD_FILL_MIN_RECT_SIZE, FLOOD_FILL_MAX_RECT_SIZE);
        double h = randomInteger(FLOOD_FILL_MIN_RECT_SIZE, FLOOD_FILL_MAX_RECT_SIZE);
        double x = randomInteger(0, window.getWidth()  - w);
        double y = randomInteger(0, window.getHeight() - h - colorList.getHeight());
        int color = colorMap[colorVector[randomInteger(0, colorVector.size() - 1)]];
        floodFillPixels.fillRegion(x, y, w, h, color);
    }
    window.add(&floodFillPixels);

    // main event loop to process events as they happen
    while (true) {
        GEvent e = waitForEvent(MOUSE_EVENT | ACTION_EVENT);
        if (e.getEventClass() == MOUSE_EVENT) {
            if (e.getEventType() != MOUSE_CLICKED) { continue; }
            colorList.setEnabled(false);
            GMouseEvent mouseEvent(e);
            string colorStr = colorList.getSelectedItem();
            int color = colorMap[colorStr];
            int mx = (int) mouseEvent.getX();
            int my = (int) mouseEvent.getY();
            cout << "Flood fill (x=" << dec << mx << ", y=" << my << "),"
                 << " color " << hex << setw(6) << setfill('0') << color
                 << dec << ": ";
            cout.flush();
            int pixelsColored = floodFill(floodFillPixels, mx, my, color);
            cout << pixelsColored << " pixels colored." << endl;
            colorList.setEnabled(true);
        } else if(e.getEventClass() == ACTION_EVENT) {
            window.remove(&floodFillPixels);
            break;
        }
    }
    cout << resetiosflags(ios::fixed | ios::floatfield);

    cout << endl;
}

/**
 * Function: Load Grammar
 * -------------------------
 * A helper function that takes in a file name and loads in
 * the corresponding grammar, returning it in 
 * Map<string, Vector<string> > form. This grammar loader is build 
 * to be able to handle production rules that are multiple lines long.
 */
Map<string, Vector<string>> loadGrammar(string fileName) {
    Map<string, Vector<string>> grammar;
    ifstream fileStream;
    openFile(fileStream, fileName);
    string key, line, currExpansion;
    while(getline(fileStream, line)) {
        if(line == "-----") {
            grammar[key].add(currExpansion);
            key = "";
            currExpansion = "";
        }
        Vector<string> parts = stringSplit(line, ":");
        if(parts.size() == 2) {
            key = parts[0];
            string values = parts[1];
            Vector<string> expansions = stringSplit(values, "|");
            for(int i = 0; i < expansions.size() - 1; i++) {
                grammar[key].add(expansions[i]);
            }
            currExpansion = expansions[expansions.size() - 1];
        } else {
            currExpansion += "\n" + line;
        }


    }
    return grammar;
}

/**
 * Function: Generate Problem
 * -------------------------
 * A helper function that takes in a file name and loads in
 * the corresponding grammar, returning it in 
 * Map<string, Vector<string> > form. This grammar loader is build 
 * to be able to handle production rules that are multiple lines long.
 */
void generateProblem() {
    string fileName = getLine("What grammar? ");
    while(true) {
        string symbol = getLine("What symbol (empty string to exit): ");
        if(symbol == "") break;
        Map<string, Vector<string>> grammar = loadGrammar(fileName);
        cout << "grammar size " << grammar.size() << endl;
        string str = generate(grammar, symbol);
        cout << str << endl << endl;
    }
}

/**
 * Function: Get Prereq File Name
 * -------------------------
 * A helper function that asks the user for a course until it is
 * able to find the corresponding prereq file.
 */
string getPreReqFileName() {
    while(true) {
        string course = getLine("What course? ");
        string prereqFile = course + "-prereq.txt";
        if(fileExists(prereqFile)) {
            return prereqFile;
        }
        cout << "could not fine prereq file for course." << endl;
    }
}

/**
 * Function: Make Personal Curriculum
 * -----------------------------------
 * Asks the learner what course they are working on and their goal concept.
 * Outputs a list of concepts they should learn (in order) to get to learn
 * their goal concept.
 */
void makePersonalCurriculum() {
    string fileName = getPreReqFileName();
    Map<string, Vector<string>> prereqMap = loadPrereqMap(fileName);
    string goal;
    while(!prereqMap.containsKey(goal)){
        string prompt = "Enter the concept the student would like to learn ";
        prompt = prompt + "(or ? to list concepts): ";
        goal = getLine(prompt);
        if(goal == "?") {
            for(string key : prereqMap) {
                cout << key << endl;
            }
        } else if(!prereqMap.containsKey(goal)) {
            cout << "Invalid concept." << endl;
        }
    }
    cout << "The order you should learn concepts:" << endl;
    personalCurriculum(prereqMap, goal);
    cout << endl;
    waitToReturnToMenu();
}

/**
 * Function: Wait to Return
 * -----------------------------------
 * Makes the user wait until they hit the enter button to return to menu.
 */
void waitToReturnToMenu() {
    getLine("Press enter to return to menu. ");
}

/**
 * Function: Invalid Choice
 * -------------------------
 * Does what it says.
 */
void invalidChoice() {
    cout << "Invalid choice" << endl;
    waitToReturnToMenu();
}

/**
 * Function: Show Splash
 * ---------------------
 * Draws the picture on the graphics window (clearning anything that was
 * previously there).
 */
void showSplash(GWindow & w) {
    w.clear();
    GImage* splash = new GImage("meta.png");
    splash->scale(0.26);
    w.add(splash);
}

/**
 * Function: Load Prereq Map
 * -------------------------
 * Loads a prerequisite map from file.
 */
Map<string, Vector<string>> loadPrereqMap(string fileName) {
    Map<string, Vector<string>> prereqMap;
    ifstream fileStream;
    openFile(fileStream, fileName);
    string line;
    while(getline(fileStream, line)) {
        Vector<string> parts = stringSplit(line, ":");
        if(parts.size() != 2) continue;
        string key = parts[0];
        string values = parts[1];
        Vector<string> prereqs = stringSplit(values, ",");
        prereqMap.add(key, prereqs);
    }
    return prereqMap;
}
