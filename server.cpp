#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <map>
#include <chrono>
#include <thread>
#include <windows.h>

#define PIPE_NAME "\\\\.\\pipe\\GuessGamePipe"

// Переместим client_times на глобальный уровень
std::map<int, std::chrono::time_point<std::chrono::steady_clock>> client_times;

void handle_client(HANDLE hPipe, int secret_number, int request_interval) {
    char buffer[512];
    DWORD bytesRead;

    while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &bytesRead, nullptr)) {
        buffer[bytesRead] = '\0';
        int client_id, guess;
        sscanf(buffer, "%d %d", &client_id, &guess);

        auto now = std::chrono::steady_clock::now();
        if (client_times.find(client_id) != client_times.end()) {
            auto last_time = client_times[client_id];
            auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - last_time).count();
            if (duration < request_interval) {
                std::string response = "Queue";
                WriteFile(hPipe, response.c_str(), response.size(), &bytesRead, nullptr);
                std::cout << "Client " << client_id << " is in queue. Request interval: " << duration << " seconds." << std::endl;
                continue;
            }
        } else {
            std::cout << "New client " << client_id << " connected." << std::endl;
        }

        // Update the last request time for the client
        client_times[client_id] = now;

        std::string response;
        if (guess == secret_number) {
            response = "Yes";
            std::cout << "Client " << client_id << " guessed correctly: " << guess << std::endl;
        } else {
            response = "No";
            std::cout << "Client " << client_id << " guessed incorrectly: " << guess << std::endl;
        }

        WriteFile(hPipe, response.c_str(), response.size(), &bytesRead, nullptr);
    }

    DisconnectNamedPipe(hPipe);
    CloseHandle(hPipe);
}

int main() {
    std::ifstream config("server_config.txt");
    if (!config.is_open()) {
        std::cerr << "Failed to open server_config.txt" << std::endl;
        return 1;
    }

    int min_number = 0, max_number = 0, request_interval = 0;
    std::string line;

    while (std::getline(config, line)) {
        size_t pos = line.find('=');
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            int value = std::stoi(line.substr(pos + 1));

            if (key == "min_number") {
                min_number = value;
            } else if (key == "max_number") {
                max_number = value;
            } else if (key == "request_interval") {
                request_interval = value;
            }
        }
    }
    config.close();

    std::srand(std::time(0));
    int secret_number = std::rand() % (max_number - min_number + 1) + min_number;
    std::cout << "min_number: " << min_number << std::endl;
    std::cout << "max_number: " << max_number << std::endl;
    std::cout << "request_interval: " << request_interval << std::endl;
    std::cout << "Secret number: " << secret_number << std::endl;

    while (true) {
        HANDLE hPipe = CreateNamedPipe(
            PIPE_NAME,
            PIPE_ACCESS_DUPLEX,
            PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
            PIPE_UNLIMITED_INSTANCES,
            512, 512,
            0, nullptr);

        if (hPipe == INVALID_HANDLE_VALUE) {
            std::cerr << "Failed to create named pipe." << std::endl;
            return 1;
        }

        std::cout << "Waiting for client connection..." << std::endl;
        if (ConnectNamedPipe(hPipe, nullptr) || GetLastError() == ERROR_PIPE_CONNECTED) {
            std::thread client_thread(handle_client, hPipe, secret_number, request_interval);
            client_thread.detach();
        } else {
            CloseHandle(hPipe);
        }
    }

    return 0;
}
