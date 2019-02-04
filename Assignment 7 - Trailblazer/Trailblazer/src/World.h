/*
 * CS 106B Trailblazer
 * World is an interface (pure virtual class) representing different types
 * of world graphs. Each type of world is a subclass of this class.
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * @author Marty Stepp, Keith Schwarz, et al
 * @version 2014/11/19 (initial version for 14au)
 */

#ifndef _World_h
#define _World_h

#include "basicgraph.h"

class World {
public:
    /*
     * Returns the graph that represents this world.
     */
    virtual BasicGraph* getGraph() const = 0;
    
    /*
     * Returns an estimation or 'heuristic' about the distance
     * between vertices v1 and v2.
     * The heuristic function is guaranteed to be an 'admissible heuristic',
     * meaning that it is never an overestimation of the distance.
     * This can be used in path-search algorithms such as A*.
     */
    virtual double heuristic(Vertex* v1, Vertex* v2) = 0;
};

// private section
/*
 * Sets current world in use.
 * A hack so that heuristic function will work.
 */
void setCurrentWorld(World* world);



#endif // _World_h
