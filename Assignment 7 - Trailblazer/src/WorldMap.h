/*
 * CS 106B Trailblazer
 * WorldMap is a class representing map-based types of world graphs
 * that draw an image background with circular vertices on top.
 * See WorldMap.cpp for implementation of each member.
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * @author Marty Stepp, Keith Schwarz, et al
 * @version 2014/11/19 (initial version for 14au)
 */

#ifndef _WorldMap_h
#define _WorldMap_h

#include "gbufferedimage.h"
#include "gobjects.h"
#include "WorldAbstract.h"

class WorldMap : public WorldAbstract {
public:
    /* constants for vertex/edge colors, sizes, thickness, fonts, etc. */
    static const std::string EDGE_COLOR;
    static const double EDGE_LINE_WIDTH;
    static const double VERTEX_LINE_WIDTH;
    static const double VERTEX_RADIUS;
    static const std::string VERTEX_FILL_COLOR;
    static const std::string VERTEX_FILL_COLOR_HIGHLIGHTED;
    static const std::string LABEL_FONT_NAME;
    static const int LABEL_FONT_SIZE;
    static const std::string LABEL_FONT_STRING;
    
    /*
     * Constructor; creates a new empty world on the given graphical window.
     */
    WorldMap(GWindow* gwnd);

    /*
     * Destructor; frees the memory used by the world.
     */
    virtual ~WorldMap();
    
    // implementations of pure virtual functions from World class
    // (see World.h for documentation)
    virtual void draw();
    virtual void drawPath(Vector<Vertex*>& path);
    virtual std::string getDescription(double x, double y) const;
    virtual std::string getType() const;
    virtual void handleClick(double x, double y);
    virtual void handleMove(double x, double y);
    virtual double heuristic(Vertex* v1, Vertex* v2);
    virtual bool read(std::istream& input);
    virtual void update(Observable *obs, void *arg = NULL);

private:
    GImage* backgroundImage;   // background image to draw behind vertices
    bool heuristicEnabled;     // whether to allow heuristic function (default false)
    
    /*
     * Draws the given edge as a line with arrowhead at the end.
     */
    void drawEdgeArrow(Edge* e) const;
    
    /*
     * Draws the given vertex as a possibly filled circle of a given color.
     */
    void drawVertexCircle(Vertex* v, std::string color, bool fill = true);
    
    /*
     * Maps from x/y positions on screen to vertices in the graph.
     */
    Vertex* getVertex(double x, double y) const;
};

#endif // _WorldMap_h
