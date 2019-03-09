/* 
 * FileName: recursion.cpp
 * -------------------------
 * implements the various recursive functions that Meta Academy calls
 */

#include <math.h>
#include "recursion.h"
#include "map.h"
#include "vector.h"
#include "set.h"
#include "gwindow.h"
#include "gobjects.h"
#include "tokenscanner.h"
#include "random.h"
#include "recursion.h"
using namespace std;

int floodFillHelper(GBufferedImage& image, int x, int y, int newColor, const int& oldColor);
void personalCurriculumHelper(Map< string, Vector<string>>& prereqMap, string goal, Set<string>& prereqOutput);
string generateHelper(Map<string, Vector<string> > & grammar, string symbol, string& sentence);
int getMaxInt (const Vector<string>& deriveRules);

/**
 * @brief gcd calculates the greatest common divisor between two integers
 * @param a is first integer to calculate gcd for
 * @param b is the second integer to calculate gcd for
 * @return the gcd between a and b
 */
int gcd(int a, int b) {
    if (b == 0) {
        cout << "gcd(" <<a << ", " << b << ") = " << a <<endl;
        return a;
    } else {
        cout << "gcd(" <<a << ", " << b << ") = gcd(" << b << ", " << a % b << ")" <<endl;;
        return gcd(b, a % b);
    }


    cout << "[recursion gcd called]" << endl;
    return 0;
}

/**
 * @brief serpinskii creates a triangle fractal pattern
 * @param w provides the GWindow in which the lines are drawn on
 * @param leftX provides the left most x coordinate in which the drawings cannot go past
 * @param leftY provides the top most y coordinate in whcih the drawings cannot go past
 * @param size provides the dimensions of the drawings
 * @param order provides the level of fractal-ness of the drawing
 */
void serpinskii(GWindow &w, int leftX, int leftY, int size, int order) {
    if (order < 0) {
        throw "Order cannot be Negative";
    } else if ( order == 1) {
        w.drawLine(leftX, leftY, leftX + size, leftY);
        w.drawLine(leftX, leftY, leftX + (size / 2), leftY + size);
        w.drawLine(leftX + size, leftY,  leftX + (size / 2), leftY + size);
    } else if (order > 1) {
        serpinskii(w, leftX, leftY, size / 2, order - 1);
        serpinskii(w, leftX + (size / 2), leftY, size / 2, order - 1);
        serpinskii(w, leftX + (size / 4), leftY + (size / 2), size / 2, order - 1);
    }
    cout << "[recursion serpinskii called]" << endl;
}

/**
 * @brief floodFill changes all connected region of one said color to newColor by calling floodFillHelper
 * @param image provides the image in which connect clicked on color region will be changed
 * @param x provides the x coordinate of the mouse click
 * @param y provides the y coordinate of the mouse click
 * @param newColor provides the newColor in which the connected color region of the image
 * clicked on by the mouse will be changed too.
 * @return the number of pixels whose color have been changed.
 */
int floodFill(GBufferedImage& image, int x, int y, int newColor) {
    int oldColor = image.getRGB(x, y);
    cout << "[recursion flood fill called]" << endl;
    return floodFillHelper(image, x, y, newColor, oldColor);
}

/**
 * @brief floodFillHelper is a helper function that changes all connected region of one said color to newColor
 * @param image provides the image in which connect clicked on color region will be changed
 * @param x provides the x coordinate of the mouse click
 * @param y provides the y coordinate of the mouse click
 * @param newColor provides the newColor in which the connected color region of the image
 * clicked on by the mouse will be changed too.
 * @param oldColor provides the color which the pixes are being changed from
 * @return the number of pixels whose color have been changed.
 */
int floodFillHelper(GBufferedImage& image, int x, int y, int newColor, const int& oldColor) {
    if (x >= 0 && x < image.getWidth()
            && y >= 0 && y < image.getHeight()) {
        if (image.getRGB(x, y) == oldColor) {
            image.setRGB(x, y, newColor);
            return 1 + floodFillHelper(image, x + 1, y, newColor, oldColor)
                    + floodFillHelper(image, x - 1, y, newColor, oldColor)
                    + floodFillHelper(image, x, y + 1, newColor, oldColor)
                    + floodFillHelper(image, x, y - 1, newColor, oldColor);

        }
    }
    return 0;
}

/**
 * @brief personalCurriculum creates a list of items that have to be studied
 * in order to reach a particular goal by calling the personalCurriculumHelper
 * @param prereqMap provides the map, mapping a goal to a list of direct pre-requisites
 * @param goal provides the goal that the user wants to achieve
 */
void personalCurriculum(Map<string, Vector<string>> & prereqMap,string goal) {
    Set<string> prereqOutput;
    personalCurriculumHelper(prereqMap, goal, prereqOutput);
    cout << "[recursion personal curriculum called]" << endl;
}

/**
 * @brief personalCurriculumHelper is a helper function that creates a list of items that have to be studied
 * in order to reach a particular goal
 * @param prereqMap provides the map, mapping a goal to a list of direct pre-requisites
 * @param goal provides the goal that the user wants to achieve
 * @param prereqOutput contains the Set<string> of all the pre-req that is required to achieve a particular goal
 */
void personalCurriculumHelper(Map< string, Vector<string>>& prereqMap, string goal, Set<string>& prereqOutput) {
    Vector<string> directPrereq = prereqMap.get(goal);
    for (string key : directPrereq) {
        personalCurriculumHelper(prereqMap, key, prereqOutput);
    }
    if (! prereqOutput.contains(goal)) {
        cout << goal << endl;
    }
    prereqOutput.add(goal);
}

/**
 * @brief generate creates a question by calling generatehelper
 * @param grammar provides the Map of grammer components to its direct constituent parts
 * @param symbol provides the starting grammer component
 * @return a string containing the question generated randomly by the algorithm
 */
string generate(Map<string, Vector<string> > & grammar, string symbol) {
    cout << "[recursion generate called]" << endl;
    string sentence = "";
    return generateHelper(grammar, symbol, sentence);
}

/**
 * @brief generateHelper is a helper function to generate which creates questions by
 * recursively breaking down grammer components until it correspond to actual words
 * @param grammar provides the Map of grammer components to its direct constituent parts
 * @param symbol provides the starting grammer component
 * @param sentence contains the question that is being generated
 * @return a string containing the question generated randomly by the algorithm
 */
string generateHelper(Map<string, Vector<string> > & grammar, string symbol, string& sentence) {
    if (! grammar.containsKey(symbol)) {
        return sentence += symbol;
    } else {
        Vector<string> deriveRules = grammar.get(symbol);
        int chosen = randomInteger(0, getMaxInt(deriveRules));

        TokenScanner scanner(deriveRules[chosen]);
        while (scanner.hasMoreTokens()) {
           string token = scanner.nextToken();
           generateHelper(grammar, token, sentence);
        }
    }
    return sentence;
}

/**
 * @brief getMaxInt returns the size of the deriveRules vector
 * @param deriveRules provides the vector in which the size is determined
 * @return the size of the deriveRules Vector
 */
int getMaxInt (const Vector<string>& deriveRules) {
    return deriveRules.size() - 1;
}
