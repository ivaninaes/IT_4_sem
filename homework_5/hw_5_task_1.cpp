#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
std::mutex cout_mutex;
void print_thread_info(int thread_id, int total_threads) {
    std::lock_guard<std::mutex> lock(cout_mutex); //Синхронизация доступа к std::cout
    std::cout << "Thread ID: " << thread_id
              << ", number of threads: " << total_threads
              << ", fundamentally important message: HelloWorld" << std::endl;
}
int main() {
    const int num_threads = 8;
    std::vector<std::thread> threads;
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(print_thread_info, i, num_threads);
    }
    for (auto& thread : threads) {
        thread.join();
    }
    return 0;
}
