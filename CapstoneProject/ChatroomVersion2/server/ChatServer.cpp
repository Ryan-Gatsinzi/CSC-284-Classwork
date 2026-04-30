#include <iostream>
#include <string>
#include <cstring>
#include <thread>
#include <vector>
#include <algorithm>
#include <mutex>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define closesocket close
#endif

using namespace std;

std::mutex clientMutex;

vector<int> activeClients;

void removeClient(int clientSocket){
    std::lock_guard<std::mutex> lock(clientMutex);
    auto it = find_if(activeClients.begin(), activeClients.end(), [clientSocket](int client){
        return client == clientSocket;
    });

    activeClients.erase(it);
}

void broadcastMessage(const vector<int> &activeClients, const char *buffer, int bytesReceived)
{

    for (int client : activeClients)
    {
        if (send(client, buffer, bytesReceived, 0) < 0)
        {
            std::cerr << "Send failed\n";
        }
    }
}

void handleClient(int listenSocket, int clientSocket, const sockaddr_in& clientAddr)
{
    if (clientSocket < 0)
    {
        std::cerr << "Accept failed\n";
        closesocket(listenSocket);
#ifdef _WIN32
        WSACleanup();
#endif

        return;
    }

    char clientIp[INET_ADDRSTRLEN];
    std::memset(clientIp, 0, sizeof(clientIp));

    inet_ntop(AF_INET, &clientAddr.sin_addr, clientIp, sizeof(clientIp));
    std::cout << "Client connected from "
              << clientIp << ":" << ntohs(clientAddr.sin_port) << '\n';

    char buffer[4096];

    bool isRunning = true;
    while (isRunning)
    {
        std::memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (bytesReceived > 0)
        {
            std::string message(buffer, bytesReceived);
            std::cout << "\nClient: " << message << " to: ";

            for (int client : activeClients)
            {
                cout << client << ", ";
                if (send(client, buffer, bytesReceived, 0) < 0)
                {
                    std::cerr << "Send failed\n";
                }
            }
        }
        else if (bytesReceived == 0)
        {
            std::cout << "Client disconnected\n";
            removeClient(clientSocket);
            isRunning = false;
        }
        else
        {
            std::cerr << "Receive failed\n";
            removeClient(clientSocket);
            isRunning = false;
        }
    }

    closesocket(clientSocket);
}

int main(int argc, char* argv[])
{
    string ip = (argc > 1 ? argv[1] : "127.0.0.1");
    int port = (argc > 2 ? stoi(argv[2]) : 54000);

#ifdef _WIN32
    WSADATA data;
    if (WSAStartup(MAKEWORD(2, 2), &data) != 0)
    {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }
#endif

    int listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSocket < 0)
    {
        std::cerr << "Cannot create socket\n";
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);

     if (inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr) <= 0)
    {
        std::cerr << "Invalid address\n";
        closesocket(listenSocket);
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }

    if (bind(listenSocket, (sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        std::cerr << "Bind failed\n";
        closesocket(listenSocket);
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }

    if (listen(listenSocket, SOMAXCONN) < 0)
    {
        std::cerr << "Listen failed\n";
        closesocket(listenSocket);
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }

    vector<thread> threads;

    while (true)
    {
        sockaddr_in clientAddr{};
        socklen_t clientSize = sizeof(clientAddr);

        int clientSocket = accept(listenSocket, (sockaddr *)&clientAddr, &clientSize);
        activeClients.push_back(clientSocket);

        threads.emplace_back(handleClient, listenSocket, clientSocket, clientAddr);
    }

    closesocket(listenSocket);

#ifdef _WIN32
    WSACleanup();
#endif

    return 0;
}