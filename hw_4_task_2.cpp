#include <iostream>
using namespace std;
#include <vector>
#include <chrono>
#include <algorithm>
#include <random>
void bubblesort(std::vector<int> &arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}
void optimised_bubblesort(std::vector<int> &arr) {
    int n = arr.size();
    bool flag = true;
    for (int i = 0; i < n - 1; ++i) {
        flag = false;
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
                flag = true;
            }
        }
        if (!flag) {
            break; //заканчиваем, раз все уже отсортировано
        }
    }
}
//погнали замерять время
template <typename Func>
double measure(Func func, std::vector<int> arr) {
    auto start = std::chrono::high_resolution_clock::now();
    func(arr);
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
        std::vector<int> arr = generate_random_array(size);
        double plain = measure(bubblesort, arr);
        double opt = measure(optimised_bubblesort, arr);
        std::cout << "Array Size: " << size << "\n";
        std::cout << "Original Bubble Sort Time: " << plain << " seconds\n";
        std::cout << "Optimized Bubble Sort Time: " << opt << " seconds\n";
        std::cout << "-----------------\n";
    }
    return 0;
}