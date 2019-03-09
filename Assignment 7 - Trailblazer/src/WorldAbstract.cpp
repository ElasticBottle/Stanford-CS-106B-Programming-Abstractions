/*
 * CS 106B Trailblazer
 * This file contains implementations of the members of the World class.
 * See World.h for declarations and documentation of each member.
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * @author Marty Stepp, Keith Schwarz, et al
 * @version 2014/11/19 (initial version for 14au)
 */

#include "WorldAbstract.h"
#include <fstream>

const int WorldAbstract::MAX_ROWS = 400;
const int WorldAbstract::MAX_COLS = 400;
const int WorldAbstract::WINDOW_MARGIN = 5;
const double WorldAbstract::PATH_LINE_WIDTH = 3.0;
const void* WorldAbstract::EVENT_PATH_SELECTION_READY = (void*) 0xf00d000;
const void* WorldAbstract::EVENT_VERTEX_COLORED       = (void*) 0xf00d100;
const std::string WorldAbstract::COLOR_BACKGROUND_STR("Black");
const std::string WorldAbstract::COLOR_HOVER_STR("#bbbb00");
const std::string WorldAbstract::COLOR_PATH_STR("Red");
const std::string WorldAbstract::COLOR_SELECTION_STR("Red");
const int WorldAbstract::COLOR_BACKGROUND = 0x0;
const int WorldAbstract::COLOR_HOVER = 0xbbbb00;
const int WorldAbstract::COLOR_PATH = 0xff0000;
const int WorldAbstract::COLOR_SELECTION = 0xff0000;

WorldAbstract::WorldAbstract(GWindow* gwnd)
        : gwnd(gwnd),
          windowWidth(0),
          windowHeight(0),
          graph(NULL),
          selectedStart(NULL),
          selectedEnd(NULL) {
    windowWidth = gwnd->getWidth() - 2 * WINDOW_MARGIN;
    windowHeight = gwnd->getHeight() - 2 * WINDOW_MARGIN;
}

WorldAbstract::~WorldAbstract() {
    if (graph) {
        delete graph;
        graph = NULL;
    }
}


void WorldAbstract::clearPath(bool redraw) {
    for (GLine* line : highlightedPath) {
        gwnd->remove(line);
        delete line;
    }
    highlightedPath.clear();
    
    if (redraw) {
        draw();
        if (!gwnd->isRepaintImmediately()) {
            gwnd->repaint();
        }
    }
}

void WorldAbstract::clearSelection(bool redraw) {
    setSelectedStart(NULL);
    setSelectedEnd(NULL);
    if (redraw) {
        draw();
        if (!gwnd->isRepaintImmediately()) {
            gwnd->repaint();
        }
    }
}

BasicGraph* WorldAbstract::getGraph() const {
    return graph;
}

const GDimension& WorldAbstract::getPreferredSize() const {
    return preferredSize;
}

Vertex* WorldAbstract::getSelectedStart() const {
    return selectedStart;
}

Vertex* WorldAbstract::getSelectedEnd() const {
    return selectedEnd;
}

bool WorldAbstract::read(const std::string& filename) {
    std::ifstream input;
    input.open(filename.c_str());
    if (input.fail()) {
        return false;
    }
    return read(input);
}

void WorldAbstract::setSelectedStart(Vertex* v) {
    selectedStart = v;
}

void WorldAbstract::setSelectedEnd(Vertex* v) {
    selectedEnd = v;
}

bool WorldAbstract::getNonEmptyLine(std::istream& input, std::string& line) {
    std::string lineOut;
    while (getline(input, lineOut)) {
        trimInPlace(lineOut);
        if (!lineOut.empty() && lineOut[0] != '#') {
            line = lineOut;
            return true;
        }
    }
    return false;
}
