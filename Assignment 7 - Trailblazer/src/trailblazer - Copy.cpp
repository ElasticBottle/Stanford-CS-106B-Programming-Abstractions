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
bool notInSameCluster(Vector<HashSet<Vertex*>>& vertexCluster, Vertex* start, Vertex* end);
Vector<HashSet<Vertex*>> mergeClusters(Vector<HashSet<Vertex*>>& vertexCluster, Vertex* start, Vertex* end);
int searchFor(Vertex* vert, Vector<HashSet<Vertex*>>& vertexCluster, HashSet<Vertex*>& otherCluster);

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
 * @brief dijkstrasAlgorithm
 * @param graph
 * @param start
 * @param end
 * @return
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
 * @brief aStar
 * @param graph
 * @param start
 * @param end
 * @return
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
 * @brief kruskal
 * @param graph
 * @return
 */
Set<Edge*> kruskal(BasicGraph& graph) {
    Set<Edge*> mst;

    //Initialization of vertex clusters and priorityQueue
    Vector<HashSet<Vertex*>> vertexCluster;
    for (Vertex* vert : graph.getVertexSet()) {
        HashSet<Vertex*> cluster;
        cluster.add(vert);
        vertexCluster.add(cluster);
    }

    PriorityQueue<Edge*> pq;
    for (Edge* edge : graph.getEdgeSet()) {
        pq.enqueue(edge, edge->weight);
    }


    while(vertexCluster.size() > 1) {
        Edge* highestPEdge = pq.dequeue();
        if (notInSameCluster(vertexCluster, highestPEdge->start, highestPEdge->end)) {
            vertexCluster = mergeClusters(vertexCluster, highestPEdge->start, highestPEdge->end);
            mst.add(highestPEdge);
        }
    }

    return mst;
}

/**
 * @brief notInSameCluster checks to see if two given Vertex are in different vertexClusters.
 * @param cluster provides the HashSet from which membership of the vertices is checked against.
 * @param start provides one of the two vertex from which cluster is checked for membership.
 * @param end provides one of the two vertex from which cluster is checked for membership.
 * @return true if the cluster contains one vertex but not the other, false otherwise.
 */
bool notInSameCluster(Vector<HashSet<Vertex*>>& VertexCluster, Vertex* start, Vertex* end) {
    for (HashSet<Vertex*> cluster : VertexCluster) {
        if ((cluster.contains(start) && !cluster.contains(end))
                || (cluster.contains(end) && !cluster.contains(start))) {
            return true;
        }
    }
    return false;
}

/**
 * @brief mergeClusters takes two vertex which have been predetermined to be in
 * different vertex clusters and merges them together
 * @param vertexCluster provides the vector containing all the different vertex clusters
 * @param start provides one of the vertex
 * @param end provides the second vertex, it is of different vertex cluster from start
 * @return the vertex cluster after it has merged the vertex clusters of start and end together
 */
Vector<HashSet<Vertex*>> mergeClusters(Vector<HashSet<Vertex*>>& vertexCluster, Vertex* start, Vertex* end) {
    HashSet<Vertex*> cluster;
    HashSet<Vertex*> otherCluster;

    for (int i = 0; i < vertexCluster.size(); ++i) {
        if (vertexCluster[i].contains(start)
                || vertexCluster[i].contains(end)) {

            cluster = vertexCluster[i];
            vertexCluster.remove(i);
            if (cluster.contains(start)) {
                vertexCluster.remove(searchFor(end, vertexCluster, otherCluster));
                break;
            }
            vertexCluster.remove(searchFor(start, vertexCluster, otherCluster));
            break;
        }
    }
    vertexCluster.add(cluster + otherCluster);
    return vertexCluster;
}

/**
 * @brief searchFor finds a vertex within the vertex cluster and assigns it to a hashset
 * which is passed by reference
 * @param vert provides the vertex to search for
 * @param vertexCluster provides the vertex cluster in which search is performed on
 * @param otherCluster provides the reference value to a hashset in which will be assigned
 * the vertex cluster that vert belongs too
 * @return the index in which the vertex cluster for vert was found at so that the caller can
 * choose to remove the vertex cluster that vert belongs too from vertexCluster
 */
int searchFor(Vertex* vert, Vector<HashSet<Vertex*>>& vertexCluster, HashSet<Vertex*>& otherCluster) {
    for (int i = 0; i < vertexCluster.size(); ++i) {
        if (vertexCluster[i].contains(vert)) {
            otherCluster = vertexCluster[i];
            return i;
        }
    }
    return -1;
}
