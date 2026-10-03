#pragma once

#include <vector>
#include <stdexcept>
#include <utility>

template <typename T>
class MinPriorityQueue {
private:
    // This vector holds our tree elements
    std::vector<T> heap;

    // Helper: float a smaller value up towards the top
    void siftUp(size_t index) {
        while (index > 0) {
            size_t parent = (index - 1) / 2;
            if (heap[index] < heap[parent]) {
                std::swap(heap[index], heap[parent]);
                index = parent;
            } else {
                break;
            }
        }
    }

    // Helper: sink a larger value down towards the bottom
    void siftDown(size_t index) {
        size_t size = heap.size();
        while (true) {
            size_t left = 2 * index + 1;
            size_t right = 2 * index + 2;
            size_t smallest = index;

            if (left < size && heap[left] < heap[smallest]) {
                smallest = left;
            }
            if (right < size && heap[right] < heap[smallest]) {
                smallest = right;
            }

            if (smallest != index) {
                std::swap(heap[index], heap[smallest]);
                index = smallest;
            } else {
                break;
            }
        }
    }

public:
    MinPriorityQueue() = default;

    // Checks if the queue is empty
    bool empty() const { 
        return heap.empty(); 
    }

    // Returns how many items are in the queue
    size_t size() const { 
        return heap.size(); 
    }

    // Add a new value into the heap
    void push(const T& value) {
        heap.push_back(value);
        siftUp(heap.size() - 1);
    }

    // Look at the smallest item on top
    T top() const {
        if (heap.empty()) {
            throw std::runtime_error("PriorityQueue is empty!");
        }
        return heap.front();
    }

    // Remove the smallest item from the top
    void pop() {
        if (heap.empty()) {
            throw std::runtime_error("PriorityQueue is empty!");
        }
        heap.front() = std::move(heap.back());
        heap.pop_back();
        if (!heap.empty()) {
            siftDown(0);
        }
    }
};