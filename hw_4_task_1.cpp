#include <set>
#include <vector>
#include <iostream>
using namespace std;
#include <chrono>
#include <algorithm>
#include <random>
//bubble sort
void bubblesort(std::vector<int> &arr) {
    int n = arr.size();
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}
//quick sort
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
//merge sort
void merge(std::vector<int> &arr, int low, int mid, int high) {
    int n1 = mid - low + 1; //левый подмассив
    int n2 = high - mid; //правый подмассив
    std::vector<int> L(n1 + 1);
    std::vector<int> R(n2 + 1);
    for (int i = 0; i < n1; ++i) {
        L[i] = arr[low + i];
    }
    for (int j = 0; j < n2; ++j) {
        R[j] = arr[mid + 1 + j];
    }
    //сливаем обратно
    int i = 0, j = 0;
    int k = low;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            ++i;
        }
        else {
            arr[k] = R[j];
            ++j;
        }
        ++k;
    }
    //копируем оставшиеся элементы L и R, если есть
    while (i < n1) {
        arr[k] = L[i];
        ++i;
        ++k;
    }
    while (j < n2) {
        arr[k] = R[j];
        ++j;
        ++k;
    }
}
void mergesort(std::vector<int> &arr, int low, int high) {
    if (low < high) {
        //нашли середину, рекурсивно сортируем половинки
        int mid = (low + high) / 2;
        mergesort(arr, low, mid);
        mergesort(arr, mid + 1, high);
        //сливаем
        merge(arr, low, mid, high);
    }
}
//selection sort
void selectionsort(std::vector<int> &arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        int min = i;
        for (int j = i + 1; j < n; ++j) {
            if (arr[j] < arr[min]) {
                min = j;
            }
        }
        std::swap(arr[i], arr[min]);
    }
}
//замеры времени ура
template <typename Func>
double measuretime(Func func, std::vector<int> &arr) {
    auto start = std::chrono::high_resolution_clock::now();
    func(arr);
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double>(end - start).count();
}
int main() {
    std::vector<int> size = {1000, 5000, 10000};
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 10000);
    for (int s : size) {
        std::vector<int> arr(s);
        std::generate(arr.begin(), arr.end(), [&]() { return dis(gen); });
        std::cout << "ARRAY SIZE: " << s << "\n";
        std::vector<std::pair<std::string, double>> timings;

        auto arr_copy = arr;
        double time = measuretime(bubblesort, arr_copy);
        timings.emplace_back("Bubble sort", time);

        arr_copy = arr;
        time = measuretime([&](std::vector<int>& a) { quicksort(a, 0, a.size() - 1); }, arr_copy);
        timings.emplace_back("Quick sort", time);

        arr_copy = arr;
        time = measuretime([&](std::vector<int>& a) { mergesort(a, 0, a.size() - 1); }, arr_copy);
        timings.emplace_back("Merge sort", time);

        arr_copy = arr;
        time = measuretime(selectionsort, arr_copy);
        timings.emplace_back("Selection sort", time);
        std::sort(timings.begin(), timings.end(), [](const auto& a, const auto& b) {
            return a.second < b.second;
        });
        std::cout << "Efficiency ranking:\n";
        for (const auto& [name, time] : timings) {
            std::cout << name << ": " << time << " seconds\n";
        }
        std::cout << "\n";
    }
    return 0;
}