/*
 * CS 106B Trailblazer
 * WorldGrid is an abstract virtual class representing Grid-based types
 * of world graphs.  It is extended by WorldMaze and WorldTerrain.
 * See WorldGrid.cpp for implementation of each member.
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * @author Marty Stepp, Keith Schwarz, et al
 * @version 2014/11/19 (initial version for 14au)
 */

#ifndef _WorldGrid_h
#define _WorldGrid_h

#include "WorldAbstract.h"
#include "gbufferedimage.h"
#include "grid.h"

class WorldGrid : public WorldAbstract {
public:
    /*
     * The size, in pixels, of the displayed world.
     * Number of padding pixels between the border of the window and the
     * start of the content.
     * Constants controlling the amount we should adjust the size of the window
     * to reflect the extra space used up by the border and controls.
     */
    static const int WINDOW_WIDTH;
    static const int WINDOW_HEIGHT;
    
    /*
     * Constructor; creates a new empty world on the given graphical window with
     * the given number of rows and columns.
     */
    WorldGrid(GWindow* gwnd, int rowsCols);
    
    /*
     * Destructor; frees the memory used by the world.
     */
    virtual ~WorldGrid();
    
    // implementations of pure virtual functions from World class
    // (see World.h for documentation)
    virtual void clearSelection(bool redraw = true);
    virtual void draw();
    virtual void drawPath(Vector<Vertex*>& path);
    virtual std::string getDescription(double x, double y) const;
    virtual BasicGraph* gridToGraph(const Grid<double>& world);
    virtual void handleClick(double x, double y);
    virtual void handleMove(double x, double y);
    virtual double heuristic(Vertex* v1, Vertex* v2);
    virtual bool read(std::istream& input);
    virtual void update(Observable *obs, void *arg = NULL);
    
    // pure virtual functions
    /*
     * Computes and returns the cost of going from vertex (r1, c1) to (r2, c2).
     * If they are not neighbors, returns POSITIVE_INFINITY.
     * This is implemented by subclasses for maze/terrain.
     */
    virtual double costFunction(int r1, int c1, int r2, int c2) const = 0;
    
    /*
     * Returns the number of rows and columns a graph of the given world size
     * should have.  For example, a 'tiny' maze might have 6 rows/cols while
     * a 'tiny' terrain might have 11.
     */
    virtual int getRowsCols(WorldSize size) const = 0;
    
    /*
     * Returns an estimation or 'heuristic' about the distance
     * between row/col positions (r1, c1) and (r2, c2).
     * This can be used in path-search algorithms such as A*.
     */
    virtual double heuristic(int r1, int c1, int r2, int c2) = 0;
    
protected:
    double pxPerWidth;        // w/h of each grid rectangle on screen, in px
    double pxPerHeight;
    Grid<double> worldGrid;   // internal grid representation of graph
    GRect* clickRect1;        // rectangles for selected start/end red squares
    GRect* clickRect2;
    
    /*
     * Computes the x/y location of the center of the square at the given row/col.
     */
    virtual void findMidpoint(int row, int col, double& x, double& y) const;
    
    /*
     * Converts the given row/col position into the top-left x/y coordinates
     * of that grid rectangle on the screen.
     */
    virtual void rowColToXY(int row, int col, double& x, double& y) const;
    
    /*
     * Returns a string of the name to give to the vertex at the given row/col position,
     * zero-padded to the given number of digits, such as "r034c007".
     */
    virtual std::string vertexName(int row, int col, int digits = 0) const;
    
    /*
     * Converts the given x/y coordinates from a grid rectangle into the
     * row/col position of that rectangle in the grid.
     */
    virtual void xyToRowCol(double x, double y, int& row, int& col) const;
};


/*
 * A small structure for representing individual row/column locations in a grid.
 * Used for mapping between Vertex objects and row/col indexes in an internal grid.
 */
struct GridLocation {
    int row;
    int col;
    GridLocation(int r = 0, int c = 0) : row(r), col(c) {
        // empty
    }
};

/*
 * A small structure for representing a pair of row/column locations in a grid.
 * Used for mapping between Edge objects and pairs of row/col indexes in an internal grid.
 */
struct GridLocationPair {
    int row1;
    int col1;
    int row2;
    int col2;
    GridLocationPair(int r1 = 0, int c1 = 0, int r2 = 0, int c2 = 0)
        : row1(r1), col1(c1), row2(r2), col2(c2) {
        // empty
    }
};

#endif // _WorldGrid_h
