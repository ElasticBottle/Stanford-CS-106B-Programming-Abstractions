/* 
 * TODO: put your own comments here. Also you should leave comments on
 * each of your methods.
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

int floodFill(GBufferedImage& image, int x, int y, int newColor) {
    int oldColor = image.getRGB(x, y);
    cout << "[recursion flood fill called]" << endl;
    return floodFillHelper(image, x, y, newColor, oldColor);
}

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

void personalCurriculum(Map<string, Vector<string>> & prereqMap,string goal) {
    Set<string> prereqOutput;
    personalCurriculumHelper(prereqMap, goal, prereqOutput);
    cout << "[recursion personal curriculum called]" << endl;
}

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

string generate(Map<string, Vector<string> > & grammar, string symbol) {
    cout << "[recursion generate called]" << endl;
    string sentence = "";
    return generateHelper(grammar, symbol, sentence);
}


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

int getMaxInt (const Vector<string>& deriveRules) {
    return deriveRules.size() - 1;
}
