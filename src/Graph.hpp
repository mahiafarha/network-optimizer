#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>

// Represents one network connection / cable
struct Edge {
    int to;          // Destination router ID
    double weight;   // Latency / cost in milliseconds
};

// Represents the result of finding a path
struct PathResult {
    std::vector<int> path; // Sequence of router IDs: e.g. [0, 2, 4]
    double totalCost;      // Total latency in ms
    bool exists;           // True if a path connects start to target
};

class Graph {
private:
    int numVertices;
    std::vector<std::vector<Edge>> adjList;
    std::unordered_map<int, std::string> nodeLabels; // Friendly router names

public:
    explicit Graph(int vertices);

    void addEdge(int u, int v, double weight, bool bidirectional = true);
    void setNodeLabel(int id, const std::string& label);
    std::string getNodeLabel(int id) const;
    int getNumVertices() const;

    // Dijkstra's Shortest Path Algorithm
    PathResult dijkstra(int startNode, int endNode) const;
    void exportToDOT(const std::string& filename, const std::vector<int>& highlightedPath = {}) const;
};