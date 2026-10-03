#include "Graph.hpp"
#include <iostream>
#include <chrono>

int main() {
    // Construct a 7-node network topology
    Graph network(7);

    network.setNodeLabel(0, "Gateway-A");
    network.setNodeLabel(1, "Core-1");
    network.setNodeLabel(2, "Core-2");
    network.setNodeLabel(3, "Dist-1");
    network.setNodeLabel(4, "Dist-2");
    network.setNodeLabel(5, "Edge-1");
    network.setNodeLabel(6, "Gateway-B");

    network.addEdge(0, 1, 4.2);
    network.addEdge(0, 2, 2.7);
    network.addEdge(1, 2, 1.5);
    network.addEdge(1, 3, 5.0);
    network.addEdge(2, 4, 3.8);
    network.addEdge(3, 4, 1.2);
    network.addEdge(3, 5, 2.1);
    network.addEdge(4, 5, 4.0);
    network.addEdge(5, 6, 1.9);
    network.addEdge(4, 6, 6.4);

    int start = 0;
    int target = 6;

    std::cout << "Benchmarking Dijkstra execution from " 
              << network.getNodeLabel(start) << " to " 
              << network.getNodeLabel(target) << "...\n";

    // High-resolution clock starts
    auto t1 = std::chrono::high_resolution_clock::now();
    PathResult result = network.dijkstra(start, target);
    auto t2 = std::chrono::high_resolution_clock::now();
    // Clock ends

    // Compute execution time in microseconds (us)
    std::chrono::duration<double, std::micro> elapsed_us = t2 - t1;

    if (result.exists) {
        std::cout << "\nOptimal Route Resolved in " << elapsed_us.count() << " microseconds (sub-millisecond resolution)\n";
        std::cout << "Path: ";
        for (size_t i = 0; i < result.path.size(); ++i) {
            std::cout << network.getNodeLabel(result.path[i]);
            if (i + 1 < result.path.size()) {
                std::cout << " -> ";
            }
        }
        std::cout << "\nTotal Latency: " << result.totalCost << " ms\n\n";

        network.exportToDOT("network_route.dot", result.path);
        std::cout << "Updated network_route.dot\n";
    } else {
        std::cout << "No route available.\n";
    }

    return 0;
}