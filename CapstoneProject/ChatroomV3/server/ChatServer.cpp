/*******************
 * Author: Ryan Gatsinzi Udatinya
 * Filename: ChatServer.cpp
 * last_updated: 07_05_2026
 *******************/

#include <iostream>
#include <string>
#include <cstring>
#include <thread>
#include <vector>
#include <algorithm>
#include <mutex>
#include <random>

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

#include "../Room.cpp"
#include "../User.cpp"

using namespace std;

std::mutex serverMutex;


void removeClient(vector<Room>& rooms, User& user)
{
    std::lock_guard<std::mutex> lock(serverMutex);
    auto room_it = find_if(rooms.begin(), rooms.end(), [&user](Room& room){
         return room.getRoomID() == user.getRoomID(); 
        });
    auto user_it = find_if(room_it->getUsers().begin(), room_it->getUsers().end(), [&user](User& user_in_room){
        return user.getUsername() == user_in_room.getUsername(); 
    });
    
    room_it->getUsers().erase(user_it);
}

bool roomExists(std::vector<Room> &rooms, std::string room_id)
{
    auto room_it = find_if(rooms.begin(), rooms.end(), [&room_id](Room &room){
        return room.getRoomID() == room_id; 
    });

    return room_it != rooms.end();
}

void createRoom(vector<Room> &rooms, User &user, string room_id)
{
    if (room_id != "" && !roomExists(rooms, room_id))
    {
        Room room(room_id);
        room.setRoomData({"Welcome to the chat!", "This is a placeholder message.", "More messages will appear here later", ""});
        rooms.push_back(room);

        string message = "__user__:" + user.getUsername() + "\n__type__:room_list\n" + "__msg__:" + room_id;

        // sending the new room to all users
        for (int i = 0; i < rooms.size(); i++)
        {
            for (int j = 0; j < rooms[i].getUsers().size(); j++)
                send(rooms[i].getUsers()[j].getClientSocket(), message.c_str(), message.length(), 0);
        }
    }
}

void removeUserFromRoom(vector<Room> &rooms, User &user)
{
    auto prev_room = find_if(rooms.begin(), rooms.end(), [&user](Room &room){
        return room.getRoomID() == user.getRoomID();
    });

    if (prev_room != rooms.end())
        prev_room->removeUser(user);
}

string serializeRoomData(Room &room)
{
    std::string data_serailized = "";
    for (string &data : room.getRoomData())
    {
        data_serailized += data + "\n";
    }

    return data_serailized;
}

void joinRoom(vector<Room> &rooms, User &user, string room_id, Room *&current_room)
{
    auto new_room_it = find_if(rooms.begin(), rooms.end(), [room_id](Room &room){
        return room.getRoomID() == room_id;
     });

    if (room_id != "" && new_room_it != rooms.end() && (!current_room || current_room->getRoomID() != room_id))  // if the user entered a room id that exists and is not the current room
    {                                   
        removeUserFromRoom(rooms, user); // remove user from previous room

        // add user to new room
        user.setRoomID(room_id);
        try
        {
            new_room_it->addUser(user);
        }
        catch (exception &e)
        {
            cout << e.what();
        }

        current_room = &(*new_room_it); // change current room to new room

        // send all room data to recently joined user
        string message = "__user__:" + user.getUsername() + "\n__type__:room_join\n" + "__msg__:" + serializeRoomData(*new_room_it);
        send(user.getClientSocket(), message.c_str(), message.length(), 0);
    }
}

bool handleCommand(string message, User &user, vector<Room> &rooms, Room *&current_room)
{
    std::lock_guard<std::mutex> lock(serverMutex);
    const int NUM_COMMANDS = 4;
    string commands[NUM_COMMANDS] = {".CREATE_ROOM", ".JOIN_ROOM", ".LIST_ROOMS", ".EXIT"};

    for (int i = 0; i < NUM_COMMANDS; i++)
    {
        if (message.find(commands[i]) == 0)  // if a command is found in the message and its at the beginning of the message
        {
            string command_info = message.substr(commands[i].length() + 1); // +1 in order to skip the space between <command> and command_data e.g <command> command_data
            switch (i)
            {
            case 0:
                createRoom(rooms, user, command_info);
                break;
            case 1:
                joinRoom(rooms, user, command_info, current_room);
                break;
            }

            return true;
        }
    }

    return false;
}

string serializeRooms(vector<Room> &rooms)
{
    std::string rooms_serailized = "";
    for (Room &room : rooms)
    {
        rooms_serailized += room.getRoomID() + "\n";
    }

    return rooms_serailized;
}

bool nameExists(std::vector<Room> &rooms, std::string username)
{
    auto room_it = find_if(rooms.begin(), rooms.end(), [username](Room &room){
        auto user_it = find_if(room.getUsers().begin(), room.getUsers().end(), [username](User& user){
            return user.getUsername() == username;
        });

        return user_it != room.getUsers().end(); });

    return room_it != rooms.end();
}

string generateUsername(vector<Room> &rooms)
{
    random_device engine;
    uniform_int_distribution<int> dist(1000, 9999);
    string random_username;

    do
    {
        random_username = "anonymous" + to_string(dist(engine));
    } while (nameExists(rooms, random_username));

    return random_username;
}

void handleClient(int listenSocket, int clientSocket, const sockaddr_in &clientAddr, vector<Room> &rooms)
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
    std::cout << "Client connected from " << clientIp << ":" << ntohs(clientAddr.sin_port) << '\n';

    // send list of all rooms when client connects to server

    string message = "__user__:server\n__type__:room_list_initial_connection\n__msg__:" + serializeRooms(rooms);
    send(clientSocket, message.c_str(), message.length(), 0);

    // get unique username from connected user
    char buffer[4096];
    std::memset(buffer, 0, sizeof(buffer));
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

    std::string username(buffer, bytesReceived);
    if (username == "__anonymous__" || nameExists(rooms, username))
    {
        username = generateUsername(rooms);
    }

    // create user object
    User user(clientSocket, clientIp, username);

    // join user to Lobby by default
    Room *current_room = nullptr;
    joinRoom(rooms, user, "Lobby", current_room);

    bool isRunning = true;
    while (isRunning)
    {
        std::memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (bytesReceived > 0)
        {
            std::string message(buffer, bytesReceived);
            // cout << endl << message << endl;

            if (!handleCommand(message, user, rooms, current_room))
            { // check if message is a command

                current_room->addRoomData(user.getUsername() + ": " + message); // add message to roomData

                message = "__user__:" + user.getUsername() + "\n__type__:chat\n__msg__:" + user.getUsername() + ": " + message;

                // std::cout << endl << message.substr(message.find("__msg__:") + 8) << " to: ";

                for (auto it = current_room->getUsers().begin(); it != current_room->getUsers().end(); it++)
                {
                    // cout << it->getUsername() << ", ";

                    if (send(it->getClientSocket(), message.c_str(), message.length(), 0) < 0)
                    {
                        std::cerr << "Send failed\n";
                    }
                }
            }
        }
        else
        {
            removeClient(rooms, user);
            isRunning = false;
        }
    }

    closesocket(clientSocket);
}

int main(int argc, char *argv[])
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

    vector<Room> rooms = {Room("Lobby")};
    rooms[0].setRoomData({"Welcome to the chat!", "This is a placeholder message.", "More messages will appear here later", ""});
    vector<thread> threads;

    while (true)
    {
        sockaddr_in clientAddr{};
        socklen_t clientSize = sizeof(clientAddr);

        int clientSocket = accept(listenSocket, (sockaddr *)&clientAddr, &clientSize);

        threads.emplace_back(handleClient, listenSocket, clientSocket, clientAddr, std::ref(rooms));
    }

    closesocket(listenSocket);

#ifdef _WIN32
    WSACleanup();
#endif

    return 0;
}