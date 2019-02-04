/*
 * CS 106B Trailblazer
 * This file contains implementations of the members of the WorldMap class.
 * See WorldMap.h for declarations and documentation of each member.
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * @author Marty Stepp, Keith Schwarz, et al
 * @version 2014/11/19 (initial version for 14au)
 */

#include "WorldMap.h"
#include <cmath>
#include <fstream>
#include <sstream>
#include <vector>
#include "filelib.h"
#include "gmath.h"
#include "strlib.h"

const std::string WorldMap::EDGE_COLOR("#888888");
const double WorldMap::VERTEX_RADIUS = 8;
const double WorldMap::EDGE_LINE_WIDTH = 1;
const double WorldMap::VERTEX_LINE_WIDTH = 2;
const std::string WorldMap::VERTEX_FILL_COLOR("White");
const std::string WorldMap::VERTEX_FILL_COLOR_HIGHLIGHTED("Red");
const std::string WorldMap::LABEL_FONT_NAME = "SansSerif";
const int WorldMap::LABEL_FONT_SIZE = 11;
const std::string WorldMap::LABEL_FONT_STRING = WorldMap::LABEL_FONT_NAME + "-" + integerToString(WorldMap::LABEL_FONT_SIZE);

WorldMap::WorldMap(GWindow* gwnd)
        : WorldAbstract(gwnd),
          backgroundImage(NULL),
          heuristicEnabled(false) {
    // empty
}

WorldMap::~WorldMap() {
    clearSelection(false);
    clearPath(false);
    
    // clean up any pointers to allocated data about each vertex
    if (graph) {
        for (Vertex* v : *graph) {
            if (v && v->extraData) {
                delete (Point*) v->extraData;
                v->extraData = NULL;
            }
        }
    }
    
    if (backgroundImage) {
        delete backgroundImage;
    }
}

static void polarMove(double x, double y, double r, double theta, double& newX, double& newY) {
    double dx = std::cos(theta) * r;
    double dy = std::sin(theta) * -r;
    newX = x + dx;
    newY = y - dy;
//    std::cout << "polarMove(x=" << x << ", y=" << y << ", r=" << r << ", 0=" << theta
//              << ") => (x=" << newX << ", y=" << newY << ")" << std::endl;
}

void WorldMap::drawEdgeArrow(Edge* e) const {
    Point* p1 = (Point*) e->start->extraData;
    Point* p2 = (Point*) e->finish->extraData;
    double p1x = p1->getX();
    double p1y = p1->getY();
    double p2x = p2->getX();
    double p2y = p2->getY();
    
    // shift the line's endpoints inward a bit to the edges of the ovals
    double dy = p2y - p1y;
    double dx = p2x - p1x;
    double theta = std::atan2(dy, dx);
    double r = VERTEX_RADIUS + VERTEX_LINE_WIDTH;
    polarMove(p1x, p1y, r, theta, p1x, p1y);        // start
    polarMove(p2x, p2y, r, theta - PI, p2x, p2y);   // end
    
    GLine line(p1x, p1y, p2x, p2y);
    line.setColor(EDGE_COLOR);
    line.setLineWidth(EDGE_LINE_WIDTH);
    gwnd->draw(&line);
    
    // draw edge weight in middle
    if (e->cost >= 0.00000001 || e->cost < -0.00000001) {
        double midx = p1x + (p2x - p1x) / 2;
        double midy = p1y + (p2y - p1y) / 2;
        GLabel weightLabel(realToString(e->cost), midx, midy);
        weightLabel.setFont(LABEL_FONT_STRING);
        weightLabel.setColor(EDGE_COLOR);
        gwnd->draw(&weightLabel);
    }
    
    // draw arrowhead (arrow flails separated by 20 degrees)
    bool CLOSED_ARROWHEAD = true;
    double ARROWHEAD_LENGTH = 6;
    double ARROWHEAD_ANGLE = PI / 8.0;
    double backward = std::atan2(-dy, -dx);
    
    // polar to cartesian: x is cos, y is sin
    double backLX, backLY, backRX, backRY;
    polarMove(p2x, p2y, ARROWHEAD_LENGTH, backward + ARROWHEAD_ANGLE, backLX, backLY);
    polarMove(p2x, p2y, ARROWHEAD_LENGTH, backward - ARROWHEAD_ANGLE, backRX, backRY);
    
    if (CLOSED_ARROWHEAD) {
        GPolygon arrowhead;
        arrowhead.addVertex(p2x, p2y);
        arrowhead.addEdge(backLX - p2x, backLY - p2y);
        arrowhead.addEdge(backRX - backLX, backRY - backLY);
        arrowhead.setLineWidth(EDGE_LINE_WIDTH);
        arrowhead.setColor(EDGE_COLOR);
        arrowhead.setFilled(true);
        arrowhead.setFillColor(EDGE_COLOR);
        gwnd->draw(&arrowhead);
    } else {
        GLine arrowheadLine1(p2x, p2y, backLX, backLY);
        arrowheadLine1.setColor(EDGE_COLOR);
        arrowheadLine1.setLineWidth(EDGE_LINE_WIDTH);
        gwnd->draw(&arrowheadLine1);
        
        GLine arrowheadLine2(p2x, p2y, backRX, backRY);
        arrowheadLine2.setColor(EDGE_COLOR);
        arrowheadLine2.setLineWidth(EDGE_LINE_WIDTH);
        gwnd->draw(&arrowheadLine2);
    }
}

void WorldMap::drawVertexCircle(Vertex* v, std::string color, bool fill) {
    Point* p = (Point*) v->extraData;
    GOval oval(p->getX() - VERTEX_RADIUS,
               p->getY() - VERTEX_RADIUS,
               2 * VERTEX_RADIUS, 2 * VERTEX_RADIUS);
    oval.setLineWidth(VERTEX_LINE_WIDTH);
    oval.setFilled(fill);
    if (v == selectedStart || v == selectedEnd) {
        oval.setColor(COLOR_SELECTION_STR);
    }
    
    if (fill) {
        oval.setFillColor(color);
    } else {
        oval.setColor(color);
    }
    gwnd->draw(&oval);
}

void WorldMap::draw() {
    if (backgroundImage) {
        gwnd->draw(backgroundImage);
    }
    for (Vertex* v : *graph) {
        // draw oval for vertex
        drawVertexCircle(v, VERTEX_FILL_COLOR);
        
        // draw text label of vertex name
        Point* p = (Point*) v->extraData;
        GLabel label(v->name,
                     p->getX() + VERTEX_RADIUS + 2,
                     p->getY() + VERTEX_RADIUS);
        label.setFont(LABEL_FONT_STRING);
        gwnd->draw(&label);
    }
    
    for (Edge* e : graph->getEdgeSet()) {
        drawEdgeArrow(e);
    }
}

void WorldMap::drawPath(Vector<Vertex*>& path) {
    for (int i = 1; i < path.size(); i++) {
        // highlight connection between path[i - 1] and path[i]
        Point* p1 = (Point*) path[i - 1]->extraData;
        Point* p2 = (Point*) path[i]->extraData;
        GLine* connection = new GLine(p1->getX(), p1->getY(), p2->getX(), p2->getY());
        connection->setColor(COLOR_PATH);
        connection->setLineWidth(PATH_LINE_WIDTH);
        gwnd->add(connection);
        highlightedPath.add(connection);
    }
}

std::string WorldMap::getDescription(double x, double y) const {
    std::ostringstream out;
    out << "(" << (int) x << "," << (int) y << ")";
    return out.str();
}

std::string WorldMap::getType() const {
    return "map";
}

Vertex* WorldMap::getVertex(double x, double y) const {
    for (Vertex* v : *graph) {
        Point* p = (Point*) v->extraData;
        GOval oval(p->getX() - VERTEX_RADIUS,
                   p->getY() - VERTEX_RADIUS,
                   2 * VERTEX_RADIUS, 2 * VERTEX_RADIUS);
        if (oval.contains(x, y)) {
            return v;
        }
    }
    return NULL;
}

void WorldMap::handleClick(double x, double y) {
    // figure out what vertex was clicked
    Vertex* v = getVertex(x, y);
    if (!v) {
        return;
    }
    drawVertexCircle(v, COLOR_SELECTION_STR, /* fill */ false);
    
    if (!selectedStart) {
        selectedStart = v;
    } else if (!selectedEnd) {
        selectedEnd = v;
        
        // both click vertices are ready now, so notify GUI
        // so that it can do its path search
        notifyObservers((void*) EVENT_PATH_SELECTION_READY);
    } else {
        clearSelection(false);
        clearPath(true);
        selectedStart = v;
        drawVertexCircle(v, COLOR_SELECTION_STR, /* fill */ false);
    }
}

void WorldMap::handleMove(double /*x*/, double /*y*/) {
    // empty
}

double WorldMap::heuristic(Vertex* v1, Vertex* v2) {
    // straight-line x/y distance
    if (heuristicEnabled) {
        Point* p1 = (Point*) v1->extraData;
        Point* p2 = (Point*) v2->extraData;
        double dx = p2->getX() - p1->getX();
        double dy = p2->getY() - p1->getY();
        return std::sqrt(dx * dx + dy * dy);
    } else {
        return 0.0;
    }
}

bool WorldMap::read(std::istream& input) {
    if (graph) {
        delete graph;
    }
    graph = new BasicGraph();
    
    std::string line;
    if (!getNonEmptyLine(input, line)) {   // "IMAGE"
        std::cerr << "Invalid input file; missing \"IMAGE\" header" << std::endl;
        return false;
    }
        
    std::string imageFile;
    if (!getNonEmptyLine(input, imageFile)) {
        std::cerr << "Invalid input file; missing image file name" << std::endl;
        return false;
    }
    if (!fileExists(imageFile)) {
        std::cerr << "Invalid input file; specified image file \""
                  << imageFile << "\" does not exist" << std::endl;
        return false;
    }
    backgroundImage = new GImage(imageFile);
    
    int width = 0;
    if (!getNonEmptyLine(input, line)) {
        std::cerr << "Invalid input file; missing width" << std::endl;
        return false;
    }
    if (!stringIsInteger(line)) {
        std::cerr << "Invalid input file; non-integer width \""
                  << line << "\"" << std::endl;
        return false;
    }
    width = stringToInteger(line);
    
    int height = 0;
    if (!getNonEmptyLine(input, line)) {
        std::cerr << "Invalid input file; missing height" << std::endl;
        return false;
    }
    if (!stringIsInteger(line)) {
        std::cerr << "Invalid input file; non-integer height \""
                  << line << "\"" << std::endl;
        return false;
    }
    height = stringToInteger(line);
    preferredSize = GDimension(width, height);
    windowWidth = width;
    windowHeight = height;
    
    getline(input, line);  // VERTICES
    while (getNonEmptyLine(input, line)) {
        // "Hobbiton;147;86"
        std::vector<std::string> tokens = stringSplit(line, ";");
        if (tokens.size() >= 1 && (tokens[0] == "ARCS" || tokens[0] == "EDGES")) {
            break;
        } else if (tokens.size() < 3) {
            continue;
        }
        
        std::string name = trim(tokens[0]);
        if (graph->containsVertex(name)) {
            std::cerr << "Invalid input file; duplicate vertex \""
                      << name << "\"" << std::endl;
            return false;
        }
        
        if (!stringIsInteger(tokens[1]) || !stringIsInteger(tokens[2])) {
            std::cerr << "Invalid input file; non-integer coordinates for vertex \""
                      << name << "\"" << std::endl;
            return false;
        }
        int vertexX = stringToInteger(tokens[1]);
        int vertexY = stringToInteger(tokens[2]);
        if (vertexX < 0 || vertexY < 0) {
            std::cerr << "Invalid input file; negative coordinates for vertex \""
                      << name << "\"" << std::endl;
            return false;
        }
        
        Vertex* v = graph->addVertex(name);
        v->extraData = (void*) new Point(vertexX, vertexY);
        v->addObserver(this);
    }
    
    while (getNonEmptyLine(input, line)) {
        // "Hobbiton;Southfarthing;1"
        std::vector<std::string> tokens = stringSplit(line, ";");
        if (tokens.size() < 3) {
            break;
        }
        std::string name1 = trim(tokens[0]);
        std::string name2 = trim(tokens[1]);
        
        if (!graph->containsVertex(name1)) {
            std::cerr << "Invalid input file; when reading edge between \""
                      << name1 << "\" and \"" << name2
                      << "\", graph does not contain a vertex named \""
                      << name1 << "\"" << std::endl;
            return false;
        }
        if (!graph->containsVertex(name2)) {
            std::cerr << "Invalid input file; when reading edge between \""
                      << name1 << "\" and \"" << name2
                      << "\", graph does not contain a vertex named \""
                      << name2 << "\"" << std::endl;
            return false;
        }
        
        if (!stringIsReal(tokens[2])) {
            std::cerr << "Invalid input file; non-numeric weight for edge between \""
                      << name1 << "\" and \"" << name2 << "\"" << std::endl;
            return false;
        }
        
        double weight = stringToReal(tokens[2]);
        if (weight < 0) {
            std::cerr << "Invalid input file; negative weight for edge between \""
                      << name1 << "\" and \"" << name2 << "\"" << std::endl;
            return false;
        }

        // edges are undirected (both ways) by default
        bool directed = false;
        if (tokens.size() >= 4 && stringIsBool(tokens[3])) {
            directed = stringToBool(tokens[3]);
        }
        
        graph->addEdge(name1, name2, weight, /* directed */ directed);
    }
    
    return true;
}

void WorldMap::update(Observable* obs, void* /*arg*/) {
    Vertex* v = (Vertex*) obs;
    int r, g, b;
    colorToRGB(v->getColor(), r, g, b);
    std::string color = rgbToColor(r, g, b);
    drawVertexCircle(v, color);
    notifyObservers((void*) EVENT_VERTEX_COLORED);
}
