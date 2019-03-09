/*
 * CS 106B Trailblazer
 * This file implements code related to colors for the assignment.
 * See Color.h for documentation of each public function.
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * @author Marty Stepp, based on past code by Keith Schwarz
 * @version 2014/11/19 (initial version for 14au)
 */

#include "Color.h"
#include <iomanip>
#include <sstream>

// definitions of the color constants
const Color UNCOLORED = 0;
const Color WHITE = 1;
const Color GRAY = 2;
const Color YELLOW = 3;
const Color GREEN = 4;
const Color RED = 5;

/*
 * Constants reflecting the colors that are used to highlight cells different
 * colors.  This is an array of arrays, where each array contains RGB triplets
 * for the collors.  The ordering here should be consistent with the ordering
 * of the Color type.
 */
static const int COLOR_MULTIPLIERS[6][3] = {
    {   0,   0,   0 },   // UNCOLORED
    { 255, 255, 255 },   // WHITE
    { 192, 192, 192 },   // GRAY
    { 255, 255,   0 },   // YELLOW
    {   0, 255,   0 },   // GREEN
    { 255,   0,   0 }    // RED
};

void colorToRGB(int color, int& r, int& g, int& b) {
    if (color >= UNCOLORED && color <= RED) {
        r = COLOR_MULTIPLIERS[color][0];
        g = COLOR_MULTIPLIERS[color][1];
        b = COLOR_MULTIPLIERS[color][2];
    }
}

std::string colorToString(int color) {
    if (color >= UNCOLORED && color <= RED) {
        int r, g, b;
        colorToRGB(color, r, g, b);
        return rgbToColor(r, g, b);
    } else {
        return "";
    }
}

int grayscaleToRGB(double brightness) {
    if (brightness < 1.0) {
        brightness = 0.8 * brightness + 0.2;
    }
    int rgb = (int) (brightness * 255);
    return rgb << 16 | rgb << 8 | rgb;
}

std::string grayscaleToString(double brightness) {
    if (brightness < 1.0) {
        brightness = 0.8 * brightness + 0.2;
    }
    int rgb = (int) (brightness * 255);
    return rgbToColor(rgb, rgb, rgb);
}

std::string rgbToColor(int r, int g, int b) {
    std::ostringstream hexValue;
    hexValue << '#' << std::hex << std::setfill('0');
    hexValue << std::setw(2) << r << std::setw(2) << g << std::setw(2) << b;
    return hexValue.str();
}

int rgbToRGB(int r, int g, int b) {
    return r << 16 | g << 8 | b;
}

void scaleBrightness(double brightness, int& r, int& g, int& b) {
    // To make non-gray colors show up better, we artificially cap the lowest
    // possible intensity at 0.2, rather than 0.0.  Then we do a linear
    // remapping of the range [0, 1] to [0.2, 1].
    if (brightness > 0.0) {
        brightness = 0.8 * brightness + 0.2;
    }
    r = (int) (brightness * r);
    g = (int) (brightness * g);
    b = (int) (brightness * b);
}
