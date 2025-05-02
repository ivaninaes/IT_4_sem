#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <windows.h>

#define PIPE_NAME "\\\\.\\pipe\\GuessGamePipe"

void client_function(int client_id, int min_number, int max_number, int request_interval) {
    std::srand(std::time(0) + client_id);

    std::cout << "Client " << client_id << " started." << std::endl;

    while (true) {
        int guess = std::rand() % (max_number - min_number + 1) + min_number;

        HANDLE hPipe;
        while (true) {
            hPipe = CreateFile(
                PIPE_NAME,
                GENERIC_READ | GENERIC_WRITE,
                0,
                nullptr,
                OPEN_EXISTING,
                0,
                nullptr);

            if (hPipe != INVALID_HANDLE_VALUE) {
                break;
            }

            std::cerr << "Client " << client_id << " failed to connect to pipe. Retrying..." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        std::string message = std::to_string(client_id) + " " + std::to_string(guess);
        DWORD bytesWritten;
        WriteFile(hPipe, message.c_str(), message.size(), &bytesWritten, nullptr);

        char buffer[512];
        DWORD bytesRead;
        ReadFile(hPipe, buffer, sizeof(buffer) - 1, &bytesRead, nullptr);
        buffer[bytesRead] = '\0';

        std::cout << "Client " << client_id << " received: " << buffer << std::endl;

        CloseHandle(hPipe);

        std::this_thread::sleep_for(std::chrono::seconds(request_interval));
    }
}

int main() {
    std::ifstream config("client_config.txt");
    if (!config.is_open()) {
        std::cerr << "Failed to open client_config.txt" << std::endl;
        return 1;
    }

    int num_clients = 0, min_number = 0, max_number = 0, request_interval = 0;
    std::string line;

    while (std::getline(config, line)) {
        size_t pos = line.find('=');
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            int value = std::stoi(line.substr(pos + 1));

            if (key == "num_clients") {
                num_clients = value;
            } else if (key == "min_number") {
                min_number = value;
            } else if (key == "max_number") {
                max_number = value;
            } else if (key == "request_interval") {
                request_interval = value;
            }
        }
    }
    config.close();

    if (num_clients <= 0) {
        std::cerr << "Invalid number of clients: " << num_clients << std::endl;
        return 1;
    }

    std::thread clients[num_clients];

    for (int i = 0; i < num_clients; ++i) {
        clients[i] = std::thread(client_function, i, min_number, max_number, request_interval);
    }

    for (int i = 0; i < num_clients; ++i) {
        clients[i].join();
    }

    return 0;
}
