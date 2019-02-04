/*
 * CS 106B Trailblazer
 * WorldAbstract is an abstract virtual class representing different types of world graphs.
 * Each type of world is a subclass of this class.
 * See WorldAbstract.cpp for implementation of each member.
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * @author Marty Stepp, Keith Schwarz, et al
 * @version 2014/11/19 (initial version for 14au)
 */

#ifndef _WorldAbstract_h
#define _WorldAbstract_h

#include <iostream>
#include <string>
#include "gobjects.h"
#include "grid.h"
#include "gtypes.h"
#include "gwindow.h"
#include "observable.h"
#include "point.h"
#include "vector.h"
#include "Color.h"
#include "World.h"

/*
 * An enumerated type representing how large the world is.
 */
enum WorldSize {
    TINY_WORLD,
    SMALL_WORLD,
    MEDIUM_WORLD,
    LARGE_WORLD,
    HUGE_WORLD
};

class WorldAbstract : public World, public Observable, public Observer {
public:
    /*
     * Maximum number of rows or columns we allow in a world.
     * This is a safety feature to prevent an OOM on a malformed input file.
     */
    static const int MAX_ROWS;
    static const int MAX_COLS;
    
    /* px of margin around the window */
    static const int WINDOW_MARGIN;
    
    /* px thickness of path lines */
    static const double PATH_LINE_WIDTH;
    
    /* Special constant objects passed when Observable notifications are issued. */
    static const void* EVENT_PATH_SELECTION_READY;
    static const void* EVENT_VERTEX_COLORED;
    
    /* Colors for paths, backgrounds, and selected vertices. */
    static const std::string COLOR_BACKGROUND_STR;
    static const std::string COLOR_HOVER_STR;
    static const std::string COLOR_PATH_STR;
    static const std::string COLOR_SELECTION_STR;
    static const int COLOR_BACKGROUND;
    static const int COLOR_HOVER;
    static const int COLOR_PATH;
    static const int COLOR_SELECTION;
    
    /*
     * Constructor; creates a new empty world on the given graphical window.
     */
    WorldAbstract(GWindow* gwnd);

    /*
     * Destructor; frees the memory used by the world.
     */
    virtual ~WorldAbstract();
    
    /*
     * Removes any lines from the screen indicating a searched-for path.
     * If redraw is true, also repaints the entire graph.
     */
    virtual void clearPath(bool redraw = true);
    
    /*
     * Removes any shapes from the screen indicating a selected vertex.
     * If redraw is true, also repaints the entire graph.
     */
    virtual void clearSelection(bool redraw = true);
    
    /*
     * Returns the graph that represents this world.
     */
    virtual BasicGraph* getGraph() const;
    
    /*
     * Returns the width/height in pixels that this graph would like to be.
     * Used to set the window's canvas size.
     */
    virtual const GDimension& getPreferredSize() const;
    
    /*
     * Returns the ending vertex that is currently selected in the graph (NULL if none).
     */
    virtual Vertex* getSelectedEnd() const;

    /*
     * Returns the starting vertex that is currently selected in the graph (NULL if none).
     */
    virtual Vertex* getSelectedStart() const;
    
    /*
     * Sets the ending vertex that is currently selected in the graph
     * to the given vertex.
     */
    virtual void setSelectedEnd(Vertex* v);
    
    /*
     * Sets the starting vertex that is currently selected in the graph
     * to the given vertex.
     */
    virtual void setSelectedStart(Vertex* v);
    
    /*
     * Reads graph data from the given filename.
     * This method simply forwards to read(istream&).
     */
    virtual bool read(const std::string& filename);
    
    // pure virtual functions (implemented in each subclass)
    /*
     * Draws this world on the screen.
     */
    virtual void draw() = 0;
    
    /*
     * Draws the given path on the screen.
     */
    virtual void drawPath(Vector<Vertex*>& path) = 0;
    
    /*
     * Returns a description of this type of world, such as "two-dimensional mazes".
     */
    virtual std::string getDescription(double x, double y) const = 0;
    
    /*
     * Returns a description of this type of world, such as "maze" or "map".
     */
    virtual std::string getType() const = 0;
    
    /*
     * Called by the GUI when the user clicks on the graph.
     */
    virtual void handleClick(double x, double y) = 0;
    
    /*
     * Called by the GUI when the user moves the mouse on the graph.
     */
    virtual void handleMove(double x, double y) = 0;
    
    /*
     * Tries to read a world file from the specified stream.  On success, returns
     * true and updates the input parameters to mark the type of the world and
     * the world contents.  On failure, returns false.
     */
    virtual bool read(std::istream& input) = 0;
    
    /*
     * Since World.h extends Observer, the following pure virtual member
     * is also expected to be implemented by subclasses:
     * Called by the graph when a vertex's color has changed.
     * Repaints that vertex in its new color.
     */
    // virtual void update(Observable *obs, void *arg = NULL);
    
protected:
    // member variables
    GWindow* gwnd;                    // graphical window for drawing graph
    double windowWidth;               // width/height to use for display
    double windowHeight;
    GDimension preferredSize;         // size graph would like to be
    BasicGraph* graph;                // the graph itself
    Vertex* selectedStart;            // currently selected start/end vertices
    Vertex* selectedEnd;              //   from clicks (NULL if none)
    //Vertex* hover;                    // mouse-over hover vertex (NULL if none)
    Vector<GLine*> highlightedPath;   // highlighted path lines (empty if none)
    
    /*
     * Reads lines from input file until it finds one that is not blank
     * and does not begin with '#'.  Returns true if found and false on EOF.
     * Used by file-reading code to ignore blank lines and comments.
     */
    bool getNonEmptyLine(std::istream& input, std::string& line);
};

#endif // _WorldAbstract_h
