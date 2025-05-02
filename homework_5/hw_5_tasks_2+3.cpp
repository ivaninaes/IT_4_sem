#include <iostream>
#include <vector>
#include <thread>
#include <random>
#include <algorithm>
#include <mutex>
//Функция для заполнения части массива случайными числами
void fill_array(std::vector<int>& array, size_t start, size_t end) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 1000);

    for (size_t i = start; i < end; ++i) {
        array[i] = dis(gen);
    }
}
//Функция для слияния частей массива
void merge(std::vector<int>& array, size_t left, size_t middle, size_t right) {
    size_t n1 = middle - left + 1;
    size_t n2 = right - middle;
    std::vector<int> leftArray(n1);
    std::vector<int> rightArray(n2);
    std::copy(array.begin() + left, array.begin() + middle + 1, leftArray.begin());
    std::copy(array.begin() + middle + 1, array.begin() + right + 1, rightArray.begin());
    size_t i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (leftArray[i] <= rightArray[j]) {
            array[k++] = leftArray[i++];
        } else {
            array[k++] = rightArray[j++];
        }
    }
    while (i < n1) {
        array[k++] = leftArray[i++];
    }
    while (j < n2) {
        array[k++] = rightArray[j++];
    }
}
//Параллельная сортировка слиянием
void parallel_merge_sort(std::vector<int>& array, size_t left, size_t right, size_t depth = 0) {
    if (left < right) {
        size_t middle = left + (right - left) / 2;

        if (depth < std::thread::hardware_concurrency()) {
            std::thread left_thread(parallel_merge_sort, std::ref(array), left, middle, depth + 1);
            parallel_merge_sort(array, middle + 1, right, depth + 1);
            left_thread.join();
        } else {
            parallel_merge_sort(array, left, middle, depth + 1);
            parallel_merge_sort(array, middle + 1, right, depth + 1);
        }

        merge(array, left, middle, right);
    }
}
int main() {
    const size_t size = 1000;
    std::vector<int> array(size);
//Заполнение массива случайными числами
    const size_t num_threads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    size_t chunk_size = size / num_threads;
    for (size_t i = 0; i < num_threads; ++i) {
        size_t start = i * chunk_size;
        size_t end = (i == num_threads - 1) ? size : start + chunk_size;
        threads.emplace_back(fill_array, std::ref(array), start, end);
    }
    for (auto& t : threads) {
        t.join();
    }
//Сортировка
    parallel_merge_sort(array, 0, array.size() - 1);
    for (int i = 0; i < 1000; ++i) {
        std::cout << array[i] << std::endl;
    }
    return 0;
}
