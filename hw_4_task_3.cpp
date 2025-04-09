#include <iostream>
using namespace std;
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
//сортировка вставками
void insertionsort(std::vector<int> &arr, int low, int high) {
    for (int i = low + 1; i <= high; ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= low && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}
//быстрая сортировка
int partition(std::vector<int> &arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high - 1; ++j) {
        if (arr[j] < pivot) {
            ++i;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}
void quicksort(std::vector<int> &arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);
    }
}
//гибрид
void hybridsort(std::vector<int> &arr, int low, int high) {
    while (low < high) {
        if (high - low < 10) {
            insertionsort(arr, low, high);
            break;
        } else {
            int pi = partition(arr, low, high);
            if (pi - low < high - pi) {
                hybridsort(arr, low, pi - 1);
                low = pi + 1;
            } else {
                hybridsort(arr, pi + 1, high);
                high = pi - 1;
            }
        }
    }
}
//замеряем время уииии
template <typename Func>
double measure(Func func, std::vector<int> arr, int low, int high) {
    auto start = std::chrono::high_resolution_clock::now();
    func(arr, low, high);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    return diff.count();
}
std::vector<int> generate_random_array(int size) {
    std::vector<int> arr(size);
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(1, 10000);
    for (int &i : arr) {
        i = dist(rng);
    }
    return arr;
}
int main() {
    std::vector<int> sizes = {1000, 5000, 10000};
    for (int size : sizes) {
        std::vector<int> arr1 = generate_random_array(size);
        std::vector<int> arr2 = arr1;
        double plain = measure(quicksort, arr1, 0, size - 1);
        double opt = measure(hybridsort, arr2, 0, size - 1);
        std::cout << "Array Size: " << size << "\n";
        std::cout << "Quicksort Time: " << plain << " seconds\n";
        std::cout << "Hybrid Sort Time: " << opt << " seconds\n";
        std::cout << "-----------------\n";
    }
    return 0;
}