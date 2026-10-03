#include "Graph.hpp"
#include <iostream>
#include <vector>
#include <random>
#include <chrono>

void runLargeScaleBenchmark(int numNodes, int numEdges) {
    std::cout << "==========================================\n";
    std::cout << " SCALE TEST: " << numNodes << " Nodes | " << numEdges << " Edges\n";
    std::cout << "==========================================\n";

    Graph network(numNodes);

    std::mt19937 rng(42); // Deterministic seed for reproducible benchmarks
    std::uniform_int_distribution<int> nodeDist(0, numNodes - 1);
    std::uniform_real_distribution<double> weightDist(0.5, 50.0);

    std::cout << "Generating synthetic network topology...\n";
    auto genStart = std::chrono::high_resolution_clock::now();

    // 1. Build a baseline spanning backbone so the graph is guaranteed connected
    for (int i = 0; i < numNodes - 1; ++i) {
        network.addEdge(i, i + 1, weightDist(rng));
    }

    // 2. Add remaining random edges
    int remainingEdges = numEdges - (numNodes - 1);
    for (int i = 0; i < remainingEdges; ++i) {
        int u = nodeDist(rng);
        int v = nodeDist(rng);
        if (u != v) {
            network.addEdge(u, v, weightDist(rng));
        }
    }

    auto genEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> genTime = genEnd - genStart;
    std::cout << "Graph constructed in: " << genTime.count() << " ms\n\n";

    // 3. Benchmark Dijkstra from opposite ends of the network
    int startNode = 0;
    int endNode = numNodes - 1;

    std::cout << "Resolving shortest path from Node 0 to Node " << endNode << "...\n";
    
    auto solveStart = std::chrono::high_resolution_clock::now();
    PathResult result = network.dijkstra(startNode, endNode);
    auto solveEnd = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> solveTimeMs = solveEnd - solveStart;
    std::chrono::duration<double, std::micro> solveTimeUs = solveEnd - solveStart;

    if (result.exists) {
        std::cout << "Status        : SUCCESS\n";
        std::cout << "Hops in Path  : " << result.path.size() << " nodes\n";
        std::cout << "Total Latency : " << result.totalCost << " ms\n";
        std::cout << "Runtime (ms)  : " << solveTimeMs.count() << " ms\n";
        std::cout << "Runtime (us)  : " << solveTimeUs.count() << " microseconds\n";
    } else {
        std::cout << "Status        : FAILED (No path found)\n";
    }
    std::cout << "==========================================\n\n";
}

int main() {
    // Warm-up & moderate scale
    runLargeScaleBenchmark(1000, 5000);

    // High-scale production simulation
    runLargeScaleBenchmark(10000, 50000);

    // Extreme stress test
    runLargeScaleBenchmark(50000, 250000);

    return 0;
}