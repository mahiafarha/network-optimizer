#include "Graph.hpp"
#include "PriorityQueue.hpp"
#include <limits>
#include <algorithm>
#include <fstream>
#include <unordered_set>

Graph::Graph(int vertices) : numVertices(vertices), adjList(vertices) {}

void Graph::addEdge(int u, int v, double weight, bool bidirectional) {
    if (u < 0 || u >= numVertices || v < 0 || v >= numVertices) {
        std::cerr << "Error: Router " << u << " or " << v << " does not exist!\n";
        return;
    }

    adjList[u].push_back({v, weight});
    if (bidirectional) {
        adjList[v].push_back({u, weight});
    }
}

void Graph::setNodeLabel(int id, const std::string& label) {
    if (id >= 0 && id < numVertices) {
        nodeLabels[id] = label;
    }
}

std::string Graph::getNodeLabel(int id) const {
    auto it = nodeLabels.find(id);
    if (it != nodeLabels.end()) {
        return it->second;
    }
    return "Router-" + std::to_string(id);
}

int Graph::getNumVertices() const {
    return numVertices;
}

struct QueueElement {
    int id;
    double cost;

    bool operator<(const QueueElement& other) const {
        return cost < other.cost;
    }
};

PathResult Graph::dijkstra(int startNode, int endNode) const {
    if (startNode < 0 || startNode >= numVertices || endNode < 0 || endNode >= numVertices) {
        return {{}, 0.0, false};
    }

    const double INF = std::numeric_limits<double>::infinity();
    std::vector<double> dist(numVertices, INF);
    std::vector<int> parent(numVertices, -1);

    MinPriorityQueue<QueueElement> pq;

    dist[startNode] = 0.0;
    pq.push({startNode, 0.0});

    while (!pq.empty()) {
        QueueElement current = pq.top();
        pq.pop();

        int u = current.id;
        double currentCost = current.cost;

        if (currentCost > dist[u]) {
            continue;
        }

        if (u == endNode) {
            break;
        }

        for (const auto& edge : adjList[u]) {
            int v = edge.to;
            double newDist = currentCost + edge.weight;

            if (newDist < dist[v]) {
                dist[v] = newDist;
                parent[v] = u;
                pq.push({v, newDist});
            }
        }
    }

    if (dist[endNode] == INF) {
        return {{}, INF, false};
    }

    std::vector<int> path;
    for (int curr = endNode; curr != -1; curr = parent[curr]) {
        path.push_back(curr);
    }
    std::reverse(path.begin(), path.end());

    return {path, dist[endNode], true};
}

void Graph::exportToDOT(const std::string& filename, const std::vector<int>& highlightedPath) const {
    std::ofstream out(filename);
    if (!out.is_open()) {
        std::cerr << "Failed to open " << filename << " for writing.\n";
        return;
    }

    std::unordered_set<long long> pathEdges;
    if (highlightedPath.size() > 1) {
        for (size_t i = 0; i < highlightedPath.size() - 1; ++i) {
            int u = highlightedPath[i];
            int v = highlightedPath[i + 1];
            pathEdges.insert(((long long)std::min(u, v) << 32) | std::max(u, v));
        }
    }

    out << "graph Network {\n";
    out << "    layout=neato;\n";
    out << "    node [shape=box, style=\"filled,rounded\", fontname=\"Helvetica\", fontsize=10];\n";
    out << "    edge [fontname=\"Helvetica\", fontsize=9];\n\n";

    for (int i = 0; i < numVertices; ++i) {
        bool inPath = std::find(highlightedPath.begin(), highlightedPath.end(), i) != highlightedPath.end();
        out << "    " << i << " [label=\"" << getNodeLabel(i) << "\"";
        if (inPath) {
            out << ", fillcolor=\"#A7F3D0\", color=\"#059669\", penwidth=2.5";
        } else {
            out << ", fillcolor=\"#F1F5F9\", color=\"#94A3B8\"";
        }
        out << "];\n";
    }

    out << "\n";

    for (int u = 0; u < numVertices; ++u) {
        for (const auto& edge : adjList[u]) {
            int v = edge.to;
            if (u < v) {
                long long key = ((long long)u << 32) | v;
                bool isPathEdge = pathEdges.count(key) > 0;

                out << "    " << u << " -- " << v << " [label=\"" << edge.weight << "ms\"";
                if (isPathEdge) {
                    out << ", color=\"#059669\", penwidth=3.0";
                } else {
                    out << ", color=\"#CBD5E1\", penwidth=1.0";
                }
                out << "];\n";
            }
        }
    }

    out << "}\n";
    out.close();
}