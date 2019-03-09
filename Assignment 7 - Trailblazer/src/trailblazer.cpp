/*
 * FileName: trailblazer.cpp
 * ----------------------------
 * Implements the graph searching algorithm that the trailblazer app will use.
 */

#include "trailblazer.h"

using namespace std;

Vector<Vertex*> depthFirstSearchHelper(BasicGraph& graph, Vertex* start, Vertex* end
                                 , Vector<Vertex*>& path, Set<Vertex*>& visited);
Vector<Vertex*> breadthFirstSearchHelper(BasicGraph& graph, Vertex* start, Vertex* end
                                   , Queue<Vector<Vertex*>>& path, Set<Vertex*>& visited);
HashMap<Vertex*, Vertex*> dijkstrasAlgorithmHelper(BasicGraph& graph, Vertex* end
                                   , HashMap<Vertex*, double>& costMap, HashMap<Vertex*, Vertex*>& predecessor
                                         , PriorityQueue<Vertex*>& toExplore, Set<Vertex*>& visited
                                                   , bool needHeuristic);
Vector<Vertex*> makePath(HashMap<Vertex*, Vertex*>& predecessor, Vertex* end
                         ,  Vector<Vertex*>& path);
bool notInSameCluster(HashMap<Vertex*, HashSet<Vertex*>*>& vertexCluster, Edge* highestPEdge);
void mergeClusters(HashMap<Vertex*, HashSet<Vertex*>*>& vertexCluster, Edge* highestPEdge);

/**
 * @brief depthFirstSearch finds a path between two Vertex should one exists
 * @param graph provides the graph containing information about the various Vertices, Edges etc.
 * @param start Provides the starting vertex from the a the path finding Algorithm begins
 * @param end Povides the ending vertext to allow for the algorithm to know when to terminate
 * @return Vector<Vertex*> from start to end should one exists. Returns a empty Vector otherwise
 */
Vector<Vertex*> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    Vector<Vertex*> path;
    Set<Vertex*> visited;
    return depthFirstSearchHelper(graph, start, end, path, visited);
}

/**
 * @brief depthFirstSearchHelper Helper function to depthFirstSearch by taking in two additional paremeters
 * @param graph provides the graph containing information about the various Vertices, Edges etc.
 * @param start Provides the starting vertex from the a the path finding Algorithm begins
 * @param end Povides the ending vertext to allow for the algorithm to know when to terminate
 * @param path Vector<Vertex*> which stores information about the paath from start to end.
 * @param visited A set that contains the Vertices that have been searched.
 * @return Vector<Vertex*> from start to end should one exists. Returns a empty Vector otherwise
 */
Vector<Vertex*> depthFirstSearchHelper(BasicGraph& graph, Vertex* start, Vertex* end
                                 , Vector<Vertex*>& path, Set<Vertex*>& visited) {
    path.add(start);
    visited.add(start);
    start->setColor(GREEN);
    //Checks each of the neighboring Vertex.
    for (Vertex* neighbor : graph.getNeighbors(start)) {
        //If DFS(neighbour to end point) is found, return the path.
        if (path[path.size() - 1] == end) {
            path[path.size() - 1]->setColor(GREEN);
            return path;
        }
        //Only visit neighboring vertex if it hasn't been visited before
        if (!visited.contains(neighbor)) {
            neighbor->setColor(YELLOW);
            path = depthFirstSearchHelper(graph, neighbor, end, path, visited);
        }
        //If DFS(neighbour to end point) is found, return the path.
        if (path[path.size() - 1] == end) {
            path[path.size() - 1]->setColor(GREEN);
            return path;
        }
    }
    //All of the neighbouring vertex fails and so this vertex by extension
    //is not the right one since it leads only to dead-end and is removed
    //from the path.
    path[path.size() - 1]->setColor(GRAY);
    path.remove(path.size() - 1);
    return path;
}

/**
 * @brief breadthFirstSearch finds the path from the start vertex to the end by finding first finding
 * the path from the start vertex to all neighbouring vertex and then immediately backtracking from there.
 * This algorithm returns the shortest possible path between the two vertex if such a path exists
 * @param graph provides the details of the graph, including the neighbouring vertex of start
 * @param start provides the veretex from which a path is supposed to be found from
 * @param end provides the vertex from which the the algorithm will attempt to reach from start
 * @return a vector containing the vertices it traverse from the start to reach the end should a path
 * exists, returns an empty set otherwise.
 */
Vector<Vertex*> breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    Queue<Vector<Vertex*>> path;
    Set<Vertex*> visited;
    return breadthFirstSearchHelper(graph, start, end, path, visited);
}

/**
 * @brief breadthFirstSearch finds the path from the start vertex to the end by finding first finding
 * the path from the start vertex to all neighbouring vertex and then immediately backtracking from there.
 * This algorithm returns the shortest possible path between the two vertex if such a path exists
 * @param graph provides the details of the graph, including the neighbouring vertex of start
 * @param start provides the veretex from which a path is supposed to be found from
 * @param end provides the vertex from which the the algorithm will attempt to reach from start
 * @param path is a queue containing vector<vertex*> which stores information about the neighbours that have
 * not been visitied by the algorithm and are to be explored.
 * The algorithm will keep running as long as path is not empty and the end vertex has nto been reached
 * @param visited contains the set of vertex that has been visited by the algorithm. Vertices that have been
 * visited will not be explored again.
 * @return a vector containing the vertices it traverse from the start to reach the end should a path
 * exists, returns an empty set otherwise.
 */
Vector<Vertex*> breadthFirstSearchHelper(BasicGraph& graph, Vertex* start, Vertex* end
                                   , Queue<Vector<Vertex*>>& path, Set<Vertex*>& visited) {
    Vector<Vertex*> initial;
    initial.add(start);
    path.enqueue(initial);
    start->setColor(YELLOW);

    while (!path.isEmpty()) {
        Vector<Vertex*> toExplore = path.dequeue();
        Vertex* exploring = toExplore.get(toExplore.size() - 1);
        if (exploring == end) {
            exploring->setColor(GREEN);
            return toExplore;
        }
        for (Vertex* neighbor : graph.getNeighbors(exploring)) {
            if (!visited.contains(neighbor)) {
                toExplore.add(neighbor);
                neighbor->setColor(YELLOW);
                path.enqueue(toExplore);
                toExplore.remove(toExplore.size() - 1);
            }
        }
        exploring->setColor(GREEN);
        visited.add(exploring);
    }
    return initial;
}

/**
 * @brief dijkstrasAlgorithm implements the Dijkstra graph searching algorithm.
 * Calls a helper function
 * The algorithm for Dijkstra is as follow:
 *  - Initialise a starting vertex with the cost of 0 and put it in a priority queue (PQ)
 *  - Every other vertex has a cost of infinity at the start.
 *  - While PQ is not empty, dequeue the highest priority vertex
 *  - Check the cost from the highest priority vertex to its neighbouring vertex
 *  - If the cost is less then the cost calculated to get there so far, update the cost
 *    while remembering the predecessor vertex
 * @param graph provides the graph data structure in which the shortest path is found in.
 * @param start provides the starting vertex
 * @param end provides the ending vertex
 * @return the vector containing the vertex needed to be traverse to go from the start
 * to the end vertex
 */
Vector<Vertex*> dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end) {
    //Initialise everything V with the cost of infinity except the starting V with the cost of 0.
    HashMap<Vertex*, double> costMap;
    HashMap<Vertex*, Vertex*> predecessor;
    for (Vertex* v : graph.getVertexSet()) {
        costMap.put(v, 999999999999);
        predecessor.put(v, v);
    }
    costMap[start] = 0;
    predecessor[start] = NULL;

    //Put start onto a pq.
    PriorityQueue<Vertex*> toExplore;
    toExplore.add(start, costMap[start]);
    start->setColor(YELLOW);

    //While the pq is not empty, dequeue the highest priority V and explore
    //its unvisited neighbours.
    Set<Vertex*> visited;
    predecessor = dijkstrasAlgorithmHelper(graph, end, costMap, predecessor, toExplore, visited, false);

    //Backtracking from the end to figure out the path if one exists.
    Vector<Vertex*> path;
    if (predecessor[end] != end) {
        return makePath(predecessor, end, path);
    }
    return path;
}

/**
 * @brief dijkstrasAlgorithmHelper is a helper function to dijkstrasAlgorithm and handles
 * the exploration of neighbouring vertices while the priority queue is not empty
 * @param graph provides the data structure containing information about the vertices and edges
 * @param end allows for checking so that the algorithm can terminate early upon finding the
 * shortest path to the end vertec (algorithm will find shortest path from start vertex to everywhere
 * else otherwise)
 * @param costMap is a HashMap that contains the mapping for each vertex to its cost required to get there
 * from the start vertex in dijkstrasAlgorithm
 * @param predecessor is a HashMap mapping the each vertex to its predecessors, mapped to itself if there is
 * no predecessor
 * @param toExplore is the priority queue that contains the vertex that are left to explore
 * @param visited is a set containing all the visited vertices to ensure that repetition doesn't happen
 * @param needHeuristic provides a way to extend this helper function for use with the aStar search as well.
 * @return a HashMap mapping the each vertex to its predecessors
 */
HashMap<Vertex*, Vertex*> dijkstrasAlgorithmHelper(BasicGraph& graph, Vertex* end
                                   , HashMap<Vertex*, double>& costMap, HashMap<Vertex*, Vertex*>& predecessor
                                         , PriorityQueue<Vertex*>& toExplore, Set<Vertex*>& visited
                                                   , bool needHeuristic) {
    while (!toExplore.isEmpty()) {
        //dequeue the highest priority V and marking it as visited.
        Vertex* exploring = toExplore.dequeue();
        exploring->setColor(GREEN);
        if (exploring == end) {
            return predecessor;
        }
        visited.add(exploring);

        //Exploring the unvisited neighbours
        for (Vertex* neighbor : graph.getNeighbors(exploring)) {
            double cost = costMap[exploring] + graph.getEdge(exploring, neighbor)->weight;
            //If new cost is less than the old cost of getting to neighbor vertex, update it,
            //enqueue the neighbor vertex to be further explored with its predecessor remembered
            if (!visited.contains(neighbor) && cost < costMap[neighbor]) {
                    costMap[neighbor] = cost;
                    (needHeuristic) ? toExplore.add(neighbor, cost + heuristicFunction(neighbor, end))
                                    : toExplore.add(neighbor, cost);
                    neighbor->setColor(YELLOW);
                    predecessor[neighbor] = exploring;
            }
        }
    }
    cout << costMap << endl;
    return predecessor;
}


/**
 * @brief aStar is a variantion of Dijkstra's algorithm that uses admissable heuristics
 * to speed up the shortest path search process. heuristics is added to the cost of the vertex
 * when enqueueing it ontot the pq
 * @param graph provides the graph data structure contianing the information on vertices and edges
 * @param start provides the start vertex from which the shortest path begins from
 * @param end provides the end vertex so that the algorithm knows when to terminate. Else, like
 * Dijkstra, it will find the shortest path from the start vertex to every other vertices on the
 * graph.
 * @return A vector containig the shortest path of vertices from the start to the end should such
 * a path exist.
 */
Vector<Vertex*> aStar(BasicGraph& graph, Vertex* start, Vertex* end) {
    //Initialise everything V with the cost of infinity except the starting V with the cost of 0.
    HashMap<Vertex*, double> costMap;
    HashMap<Vertex*, Vertex*> predecessor;
    for (Vertex* v : graph.getVertexSet()) {
        costMap.put(v, 999999999999);
        predecessor.put(v, v);
    }
    costMap[start] = 0;
    predecessor[start] = NULL;

    //Put start onto a pq.
    PriorityQueue<Vertex*> toExplore;
    toExplore.add(start, costMap[start] + heuristicFunction(start, end));
    start->setColor(YELLOW);

    //While the pq is not empty, dequeue the highest priority V and explore
    //its unvisited neighbours.
    Set<Vertex*> visited;
    predecessor = dijkstrasAlgorithmHelper(graph, end, costMap, predecessor, toExplore, visited, true);

    //Backtracking from the end to figure out the path if one exists.
    Vector<Vertex*> path;
    if (predecessor[end] != end) {
        return makePath(predecessor, end, path);
    }
    return path;
}

/**
 * @brief makePath takes a HashMap containing each vertice's predecessor and making a vector path from it
 * @param predecessor provides the HashMap containing the from each vertex to its predecessor
 * @param end provides the ending vertex from which backtracking will happen from
 * @param path is a Vector<Vertex*> containing the path from the start to end vertex once completed
 * @return path, a Vector<Vertex*> containing the path from the start Vertex to the end Vertex.
 */
Vector<Vertex*> makePath(HashMap<Vertex*, Vertex*>& predecessor, Vertex* end, Vector<Vertex*>& path) {
    if (end == NULL) {
        return path;
    } else {
        path.insert(0, end);
        makePath(predecessor, predecessor[end], path);
    }
    return path;
}

/**
 * @brief kruskal implements Kruskal;s algorithm which finds the minimum spanning tree in a
 * graph.
 * The algorithm for Kruskals is as follows:
 *  - Remove all edges from the graph and put them into a priority queue(pq)
 *  - Put each vertice into its own vertex cluster to begin.
 *  - While there are more than 1 vertex clusters:
 *  -   Remove an edege from the pq.
 *  -   If the edge contains vertices from different vertex cluster:
 *  -       Add the edge to the Set<Edge> and merge the two vertex cluster together.
 *  -   Otherwise, ignore that edge.
 * @param graph provides the graph data structure for which the minimum spanning tree is to
 * be foudn for.
 * @return a Set<Edge> containing all the edges in the minimum spanning tree.
 */
Set<Edge*> kruskal(BasicGraph& graph) {
    Set<Edge*> mst;

    //Initialization of vertex clusters and priorityQueue
    HashMap<Vertex*, HashSet<Vertex*>*> vertexCluster;
    int numClusters = 0;
    for (Vertex* vert : graph.getVertexSet()) {
        HashSet<Vertex*>* clusterPointer = new HashSet<Vertex*>;
        clusterPointer->add(vert);
        vertexCluster.add(vert, clusterPointer);
        numClusters++;
    }

    PriorityQueue<Edge*> pq;
    for (Edge* edge : graph.getEdgeSet()) {
        pq.enqueue(edge, edge->weight);
    }


    while(numClusters > 1) {
        Edge* highestPEdge = pq.dequeue();
        if (notInSameCluster(vertexCluster, highestPEdge)) {
            mergeClusters(vertexCluster, highestPEdge);
            numClusters--;
            mst.add(highestPEdge);
        }
    }

    return mst;
}

/**
 * @brief notInSameCluster checks to see if two given Vertex are in different vertexClusters.
 * @param vertexCluster provides the HashSet from which membership of the vertices is checked against.
 * @param highestPEdge provides the edge containing the two vertices (which makes up the edge) for which
 * membership to two diffrent cluster is checked upon.
 * @return true if the cluster contains one vertex but not the other, false otherwise.
 */
bool notInSameCluster(HashMap<Vertex*, HashSet<Vertex*>*>& vertexCluster, Edge* highestPEdge) {
    return !vertexCluster[highestPEdge->start]->contains(highestPEdge->end);
}

/**
 * @brief mergeClusters takes two vertex which have been predetermined to be in
 * different vertex clusters and merges them together
 * @param vertexCluster provides the HashMap containing a mapping of the individual cluster to
 * the vertex cluster that they belong too.
 * @param highestPEdge provides the edge containing two vertices. The start is added to the
 * end's hashset of vertex in the vertexCluster map and vice versa
 */
void mergeClusters(HashMap<Vertex*, HashSet<Vertex*>*>& vertexCluster, Edge* highestPEdge) {
    *vertexCluster[highestPEdge->start] += *vertexCluster[highestPEdge->end];

    HashSet<Vertex*>* temp = vertexCluster[highestPEdge->end];
    for (Vertex* vert : *temp) {
        vertexCluster[vert] = vertexCluster[highestPEdge->start];
    }
    delete temp;
}
