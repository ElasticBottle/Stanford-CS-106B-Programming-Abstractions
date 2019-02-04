/*
 * CS 106B Trailblazer
 * This file implements code related to colors for the assignment.
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * @author Marty Stepp, based on past code by Keith Schwarz
 * @version 2014/11/19 (initial version for 14au)
 */

#ifndef _color_h
#define _color_h

#include <string>

/*
 * Represents a cost of 'infinity' for use in your various algorithms as needed.
 */
#define POSITIVE_INFINITY (1.0 / 0.0)
#define NEGATIVE_INFINITY (-1.0 / 0.0)

/*
 * An enumerated type representing a color for a node during an execution of
 * Dijkstra's algorithm or A* search.
 */
typedef int Color;
extern const Color UNCOLORED, WHITE, GRAY, YELLOW, GREEN, RED;

/*
 * Converts an integer color like 0xffdd88 into three separate red/green/blue
 * components from 0-255.
 */
void colorToRGB(int color, int& r, int& g, int& b);

/*
 * Converts an integer color like 0xffdd88 into a hex color string like "#ffdd88".
 */
std::string colorToString(int color);

/*
 * Converts a grayscale amount from 0.0 - 1.0 into a brightness RGB value from 0-255.
 */
int grayscaleToRGB(double brightness);

/*
 * Converts a grayscale amount from 0.0 - 1.0 into a hex color string like "#cccccc".
 */
std::string grayscaleToString(double brightness);

/*
 * Converts three RGB values from 0-255 into a hex color string like "#cccccc".
 */
std::string rgbToColor(int r, int g, int b);

/*
 * Converts three RGB values from 0-255 into an integer color like 0xffdd88.
 */
int rgbToRGB(int r, int g, int b);

/*
 * Converts three RGB values from 0-255 into a grayscale amount from 0.0 - 1.0,
 * scaled up a bit to make dark colors not quite so dark.
 */
void scaleBrightness(double brightness, int& r, int& g, int& b);

#endif // _color_h
