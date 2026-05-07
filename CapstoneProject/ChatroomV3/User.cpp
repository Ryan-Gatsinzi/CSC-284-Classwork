#ifndef USER_CPP
#define USER_CPP

#include <string>

class User{
    private: 
        std::string username, ip, roomID;
        int clientSocket;

    public:
        // User() : User(0, "", "", "all"){}

        User(int clientSocket = 0, std::string ip = "", std::string username = "", std::string roomID = "all") : clientSocket(clientSocket), ip(ip), username(username), roomID(roomID){}

        std::string getUsername(){ return username; }

        std::string getIP(){ return ip; }

        std::string getRoomID() { return roomID; }

        int getClientSocket() { return clientSocket; }

        void setUsername(std::string username){ this->username = username; }

        void setIP(std::string ip){ this->ip = ip; }

        void setRoomID(std::string roomID) { this->roomID = roomID; }
        
};

#endif