/*
 * CS 106B Trailblazer
 * WorldTerrain is a class representing world graphs that are 2D terrain grids.
 * Every square of a terrain is an elevation as a real number between 0.0 and 1.0.
 * Every edge in a maze is between neighboring squares and costs a differing amount
 * depending on the elevations of the two squares.
 * Going 'up' to a higher elevation is more costly than going 'down' to a lower one.
 * See WorldTerrain.cpp for implementation of each member.
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * @author Marty Stepp, Keith Schwarz, et al
 * @version 2014/11/19 (initial version for 14au)
 */

#ifndef _WorldTerrain_h
#define _WorldTerrain_h

#include "WorldGrid.h"

class WorldTerrain : public WorldGrid {
public:
    static const double ALTITUDE_PENALTY;

    /*
     * Constructor; creates a new empty world on the given graphical window with
     * the given world size.
     */
    WorldTerrain(GWindow* gwnd, WorldSize size);
    
    // implementations of pure virtual functions from World and WorldGrid classes
    // (see World.h and WorldGrid.h for documentation)
    double costFunction(int r1, int c1, int r2, int c2) const;
    virtual int getRowsCols(WorldSize size) const;
    virtual std::string getType() const;
    virtual double heuristic(int r1, int c1, int r2, int c2);
};

#endif // _WorldTerrain_h
