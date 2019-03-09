/*
 * CS 106B Trailblazer
 * This file contains implementations of the members of the WorldTerrain class.
 * See WorldTerrain.h for declarations and documentation of each member.
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * @author Marty Stepp, Keith Schwarz, et al
 * @version 2014/11/19 (initial version for 14au)
 */

#include "WorldTerrain.h"
#include <cmath>

const double WorldTerrain::ALTITUDE_PENALTY = 100.0;

WorldTerrain::WorldTerrain(GWindow* gwnd, WorldSize size)
        : WorldGrid(gwnd, getRowsCols(size)) {
    // empty
}

double WorldTerrain::costFunction(int r1, int c1, int r2, int c2) const {
    // The cost of moving from a location to itself is 0.
    if (r1 == r2 && c1 == c2) {
        return 0.0;
    }

    // Confirm that the locations are adjacent to one another.
    int drow = std::abs(r2 - r1);
    int dcol = std::abs(c2 - c1);
    if (drow > 1 || dcol > 1) {
        error("Non-adjacent locations passed into cost function.");
    }

    // Determine the absolute distance between the points.
    double distance = std::sqrt(double(drow * drow + dcol * dcol));
    double dheight = std::fabs(worldGrid.get(r2, c2) - worldGrid.get(r1, c1));
    return distance + ALTITUDE_PENALTY * dheight;
}

int WorldTerrain::getRowsCols(WorldSize size) const {
    static const int kTerrainNumRows[] = {10, 33, 65, 129, 257};
    return kTerrainNumRows[size];
}

std::string WorldTerrain::getType() const {
    return "terrain";
}

double WorldTerrain::heuristic(int r1, int c1, int r2, int c2) {
    // direct distance plus a penalty related to height difference
    int drow = r2 - r1;
    int dcol = c2 - c1;
    double dheight = std::fabs(worldGrid.get(r2, c2) - worldGrid.get(r1, c1));
    return std::sqrt((double) (drow * drow + dcol * dcol)) + ALTITUDE_PENALTY * dheight;
}
