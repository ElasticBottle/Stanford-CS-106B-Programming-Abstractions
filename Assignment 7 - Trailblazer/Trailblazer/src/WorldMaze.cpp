/*
 * CS 106B Trailblazer
 * This file contains implementations of the members of the WorldMaze class.
 * See World.h for declarations and documentation of each member.
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * @author Marty Stepp, Keith Schwarz, et al
 * @version 2014/11/19 (initial version for 14au)
 */

#include "WorldMaze.h"
#include "gmath.h"
#include "grid.h"
#include "random.h"
#include "set.h"
#include "trailblazer.h"

const int WorldMaze::MAZE_WALL = 0;
const int WorldMaze::MAZE_FLOOR = 1;

void WorldMaze::createRandomMaze(WorldSize size) {
    clearSelection(/* redraw */ false);
    
    int rowsCols = getRowsCols(size) / 2 + 1;
    worldGrid.resize(rowsCols, rowsCols);
    worldGrid.fill(MAZE_FLOOR);
    graph = gridToGraph(worldGrid);
    
    // assign random weights to the edges
    // give each edge a 'random' weight;
    // put all edges into a priority queue, sorted by weight
    Set<Edge*> edgeSet = graph->getEdgeSet();
    int edgeCount = edgeSet.size();
    for (Edge* edge : edgeSet) {
        int weight = randomInteger(1, edgeCount * 1000);
        edge->cost = weight;
    }
    
    // run the student's Kruskal algorithm to get a minimum spanning tree (MST)
    Set<Edge*> mst = kruskal(*graph);
    
    // convert the MST/graph back into a maze grid
    // (insert a 'wall' between any neighbors that do not have a connecting edge)
    graph->clearEdges();
    for (Edge* edge : mst) {
        graph->addEdge(edge->start, edge->finish);
        graph->addEdge(edge->finish, edge->start);
    }

    // physical <-> logical size; a maze of size MxN has 2M-1 x 2N-1 grid cells.
    // cells in row/col 0, 2, 4, ... are open squares (floors), and cells in 
    // row/col 1, 3, 5, ... are blocked (walls).
    int digits = countDigits(rowsCols);
    int worldSize = rowsCols * 2 - 1;
    worldGrid.resize(worldSize, worldSize);
    worldGrid.fill(MAZE_WALL);
    
    pxPerWidth = (double) windowWidth / worldSize;
    pxPerHeight = (double) windowHeight / worldSize;
    
    for (int row = 0; row < worldSize; row++) {
        for (int col = 0; col < worldSize; col++) {
            if (row % 2 == 0 && col % 2 == 0) {
                worldGrid.set(row, col, MAZE_FLOOR);
            }
        }
    }
    
    for (int row = 0; row < rowsCols; row++) {
        int gridRow = row * 2;
        for (int col = 0; col < rowsCols; col++) {
            int gridCol = col * 2;
            std::string name = vertexName(row, col, digits);
            
            // decide whether to put open floor between neighbors
            // (if there is an edge between them)
            for (int dr = -1; dr <= 1; dr++) {
                int nr = row + dr;
                int gridNr = gridRow + dr;
                for (int dc = -1; dc <= 1; dc++) {
                    int nc = col + dc;
                    int gridNc = gridCol + dc;
                    if ((nr != row && nc != col)
                            || (nr == row && nc == col)
                            || !worldGrid.inBounds(gridNr, gridNc)) {
                        continue;
                    }
                    std::string neighborName = vertexName(nr, nc, digits);
                    if (graph->containsEdge(name, neighborName)) {
                        worldGrid.set(gridNr, gridNc, MAZE_FLOOR);
                    }
                }
            }
        }
    }
    
    delete graph;
    graph = gridToGraph(worldGrid);
}

WorldMaze::WorldMaze(GWindow* gwnd, WorldSize size)
        : WorldGrid(gwnd, getRowsCols(size)) {
    // empty
}

double WorldMaze::costFunction(int r1, int c1, int r2, int c2) const {
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

    // Moving diagonally costs infinitely much.
    if (drow == 1 && dcol == 1) {
        return POSITIVE_INFINITY;
    } else if (worldGrid.get(r1, c1) == MAZE_WALL
               || worldGrid.get(r2, c2) == MAZE_WALL) {
        // See if we're moving to or from a wall.
        return POSITIVE_INFINITY;
    } else {
        // neighboring floor squares have a cost of 1 to travel between them
        return 1.0;
    }
}

int WorldMaze::getRowsCols(WorldSize size) const {
    static const int NUM_ROWS_MAZE[] = {6, 11, 31, 81, 161};
    return NUM_ROWS_MAZE[size];
}

std::string WorldMaze::getType() const {
    return "maze";
}

double WorldMaze::heuristic(int r1, int c1, int r2, int c2) {
    // straight-line "Manhattan" distance
    return std::abs(r2 - r1) + std::abs(c2 - c1);
}
