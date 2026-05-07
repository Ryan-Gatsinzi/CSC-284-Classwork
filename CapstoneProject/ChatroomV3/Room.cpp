/*******************
 * Author: Ryan Gatsinzi Udatinya
 * Filename: Room.cpp
 * last_updated: 07_05_2026
 *******************/

#ifndef ROOM_CPP
#define ROOM_CPP

#include <algorithm>
#include <vector>
#include <string>
#include "User.cpp"

class ExistingUserException : public std::exception{
    public:
        const char* what() const noexcept override{
            return "A user with this username already exists";
        }
};
class Room {
    private:
        std::string room_id;
        std::vector<User> users;
        std::vector<std::string> roomData;
    
    public:
        Room(std::string room_id) : room_id(room_id){}

        std::string getRoomID() { return room_id; }

        std::vector<User>& getUsers() { return users; }

        std::vector<std::string>& getRoomData() { return roomData; }

        void setRoomID(std::string room_id) { this->room_id = room_id; }

        void addRoomData(std::string data){ roomData.push_back(data); }

        void setRoomData(std::vector<std::string> roomData) { this->roomData = roomData; }

        void addUser(User user) {
            // find User object with same username as new user being added to the room
            auto it = find_if(users.begin(), users.end(), [&user](User& user_obj){
                return user_obj.getUsername() == user.getUsername();
            });

            //if no user has the same username as the incoming user then add user to room
            if(it == users.end())
                users.push_back(user);
            else
                throw ExistingUserException();
        }

        void removeUser(User user_to_remove){
            auto user_it = find_if(users.begin(), users.end(), [&user_to_remove](User& user){
                return user.getUsername() == user_to_remove.getUsername();
            });

            if(user_it != users.end())
                users.erase(user_it);
        }

};

#endif