/*******************
 * Author: Ryan Gatsinzi Udatinya
 * Filename: ChatClient.cpp
 * last_updated: 07_05_2026
 *******************/

#include "curses.h"
#include <vector>
#include <random>
#include <algorithm>
#include <string.h>
#include <string>
#include <stdlib.h>
#include "Display.cpp"
#include <thread>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include "../Room.cpp"
#include "../User.cpp"

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

#define MAX_INPUT 256
#define MAX_LINES 1000
#define WHITE_BLACK 1
#define YELLOW_BLACK 2
#define CYAN_BLACK 3

std::mutex chatMutex;
std::atomic<bool> running = true; // atomic - prevent race condition

void deserialize(std::vector<std::string>& container, std::string serializedData){
    std::string data = "";
        for(int i = serializedData.find("__msg__:") + 8; i < serializedData.length(); i++){ // 8 is length of __message__:
        if(serializedData[i] == '\n'){
            container.push_back(data);
            data = "";
        }else
            data.push_back(serializedData[i]);
    }
}

void draw_output(WINDOW *win, Display &disp)
{
    std::lock_guard<std::mutex> lock(chatMutex);
    int maxY, maxX;
    int lineCount = disp.getData().size(), scrollPos = disp.getScroll();
    getmaxyx(win, maxY, maxX);
    wclear(win);
    box(win, 0, 0);
    int color = WHITE_BLACK; // default color is white text, black background

    // Show visible lines
    for (int i = 0; i < maxY - 2 && i < disp.getData().size(); ++i)
    {
        if (disp.getId() == 1)
            color = (i <= 2 && disp.getScroll() == 0 ? YELLOW_BLACK : WHITE_BLACK);
        else if (disp.getId() == 2)
        {
            color = CYAN_BLACK;
        }

        wattron(win, COLOR_PAIR(color));

        if (disp.getId() == 1 && i > 3)
        {
            mvwprintw(win, i + 1, 1, "%s", std::string(disp.getData()[i + scrollPos]).c_str());
        }
        else if (disp.getId() == 2 && i == disp.getHighlight())
        {
            wattron(win, A_REVERSE);
            mvwprintw(win, i + 1, 1, "%s", disp.getData()[i + scrollPos].c_str());
            wattroff(win, A_REVERSE);
        }
        else
        {
            mvwprintw(win, i + 1, 1, "%s", disp.getData()[i + scrollPos].c_str());
        }

        wattroff(win, COLOR_PAIR(color));
    }

    // Simulated scroll bar
    if (lineCount > maxY - 2)
    {
        int barHeight = maxY - 2;
        int barPos = (scrollPos * barHeight) / lineCount;
        mvwaddch(win, barPos + 1, maxX - 2, ACS_CKBOARD);
    }

    wnoutrefresh(win);
}

void scrollChat(WINDOW *inputWin, Display &chatboxDisp, bool &onChat, int ch)
{
    if (ch == KEY_UP && chatboxDisp.getScroll() > 0)
    {
        chatboxDisp.setScroll(chatboxDisp.getScroll() - 1);
    }
    else if (ch == KEY_DOWN && chatboxDisp.getScroll() < chatboxDisp.getCount() - (chatboxDisp.getHeight() - 2))
    {
        chatboxDisp.setScroll(chatboxDisp.getScroll() + 1);
    }
}

void scrollRoom(WINDOW *roomlistWin, Display &roomlistDisp, bool &onChat, int clientSocket, int ch)
{
    curs_set(0); // turn cursor visiblity off
    if (ch == KEY_UP)
    {
        roomlistDisp.setHighlight((roomlistDisp.getHighlight() - 1) >= 0 ? roomlistDisp.getHighlight() - 1 : roomlistDisp.getCount() - 1);
    }
    else if (ch == KEY_DOWN)
    {
        roomlistDisp.setHighlight((roomlistDisp.getHighlight() + 1) <= roomlistDisp.getCount() - 1 ? roomlistDisp.getHighlight() + 1 : 0);
    }
    else if(ch == '\n'  || ch == KEY_ENTER){
        std::string msg = ".JOIN_ROOM " + roomlistDisp.getHighlightedItem();
        send(clientSocket, msg.c_str(), msg.size(), 0);
    }
}

void recieve(int clientSocket, Display &chatboxDisp, Display &roomlistDisp, WINDOW *win)
{
    char buffer[4096];
    while (running)
    {
        std::memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (bytesReceived > 0)
        {
            if (chatboxDisp.getCount() < MAX_LINES)
            {
                std::string response(buffer, bytesReceived);
                
                int room_join_index = response.find("__type__:room_join");
                int chat_index = response.find("__type__:chat");
                int room_list_index = response.find("__type__:room_list");

                if(room_join_index > 0 && room_join_index < response.size()){
                    chatboxDisp.getData().clear();
                    deserialize(chatboxDisp.getData(), response);
                }
                else if(chat_index > 0 && chat_index < response.size())
                    chatboxDisp.getData().push_back(response.substr(response.find("__msg__:") + 8)); // 8 is the length of __message__:

                else if(room_list_index > 0 && room_list_index < response.size())
                    roomlistDisp.getData().push_back(response.substr(response.find("__msg__:") + 8));


                if (chatboxDisp.getCount() > chatboxDisp.getHeight() - 2)
                    chatboxDisp.setScroll(chatboxDisp.getCount() - (chatboxDisp.getHeight() - 2));
            }
        }
        else
        {
            running = false;
        }
    }
}

int connectClient(std::string ip, int port)
{

#ifdef _WIN32
    WSADATA data;
    if (WSAStartup(MAKEWORD(2, 2), &data) != 0)
    {
        std::cerr << "WSAStartup failed\n";
        exit(0);
    }
#endif

    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0)
    {
        std::cerr << "Failed to create socket\n";
#ifdef _WIN32
        WSACleanup();
#endif
        exit(0);
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr) <= 0)
    {
        std::cerr << "Invalid address\n";
        closesocket(clientSocket);
        #ifdef _WIN32
        WSACleanup();
        #endif
        return 1;
    }
    
    if (connect(clientSocket, (sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        std::cerr << "Connection failed\n";
        closesocket(clientSocket);
        #ifdef _WIN32
        WSACleanup();
        #endif
        exit(0);
    }

    return clientSocket;
}
void toggleRoomlist(WINDOW *&chatboxWin, bool &showRoomlist){
    if(showRoomlist){
        wresize(chatboxWin, LINES * 80 / 100, COLS);
        showRoomlist = false;
    }else{
        wresize(chatboxWin, LINES * 80 / 100, COLS * 80 / 100);
        showRoomlist = true;

    }
}

int main(int argc, char *argv[])
{
    std::string ip = (argc > 2 ? argv[2] : "127.0.0.1");
    int port = (argc > 3 ? std::stoi(argv[3]) : 54000);
    //client <username> <ip> <port>
    std::vector<std::string> roomlist;
    std::vector<std::string> roomData;

    int clientSocket = connectClient(ip, port);

    char buffer[4096];
  
    std::string response = "";

    // get serialized list of all room names
    do{
        std::memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

        response = std::string(buffer, bytesReceived);
    }while(!response.find("__type__:room_list_initial_connection"));
        
    deserialize(roomlist, response);

    // send username to server
    std::string username = (argc > 1 ? argv[1] : "__anonymous__");
    send(clientSocket, username.c_str(), username.size(), 0);
    
    //get serialized list of all room data
   do{
        std::memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

        response = std::string(buffer, bytesReceived);
    }while(!response.find("__type__:room_join"));
    
    deserialize(roomData, response);

    WINDOW *chatboxWin, *roomlistWin, *inputWin;
    std::string input;

    // Init
    initscr();
    cbreak();
    noecho();
    start_color();

    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_CYAN, COLOR_BLACK);

    Display chatboxDisp(1, LINES * 80 / 100, COLS * 80 / 100, roomData);

    Display roomlistDisp(2, LINES * 80 / 100, COLS - (COLS * 80 / 100), roomlist);

    Display inputboxDisp(3, LINES - (LINES * 80 / 100), COLS);

    chatboxWin = newwin(chatboxDisp.getHeight(), chatboxDisp.getWidth(), 0, 0);

    roomlistWin = newwin(roomlistDisp.getHeight(), roomlistDisp.getWidth(), 0, chatboxDisp.getWidth());

    inputWin = newwin(inputboxDisp.getHeight(), inputboxDisp.getWidth(), chatboxDisp.getHeight(), 0);

    scrollok(chatboxWin, TRUE);
    keypad(inputWin, TRUE);
    nodelay(inputWin, TRUE);
    keypad(roomlistWin, TRUE);

    bool onChat = true;
    bool showRoomlist = true;
    // run thread that will listen to incoming messages
    std::thread thrd(recieve, clientSocket, std::ref(chatboxDisp), std::ref(roomlistDisp), std::ref(chatboxWin));

    while (running)
    {
        // Draw outputs
        draw_output(chatboxWin, chatboxDisp);

        if(showRoomlist)
            draw_output(roomlistWin, roomlistDisp);

        // Draw input
        werase(inputWin);
        box(inputWin, 0, 0);
        mvwprintw(inputWin, 1, 2, "Enter text for Window %d (type 'exit' to quit): ", 1);
        wnoutrefresh(inputWin);

        // Input
        mvwprintw(inputWin, 2, 2, "%s", input.c_str());
        wmove(inputWin, 2, 2 + input.size());
        wnoutrefresh(inputWin);
        doupdate();

        int ch = wgetch(inputWin);

        if (ch != ERR) 
        {
            if ((ch == '\n' || ch == KEY_ENTER) && onChat)
            {
                if(input.find(".JOIN_ROOM") == 0){
                
                    auto it = std::find_if(roomlistDisp.getData().begin(), roomlistDisp.getData().end(), [&input](std::string& room_id){
                        return input.substr(11) == room_id; // 11 is the size of ".JOIN_ROOM " - including the space
                    });

                    if(it != roomlistDisp.getData().end())
                        roomlistDisp.setHighlight(std::distance(roomlistDisp.getData().begin(), it));
                }
                if (input == "exit" || input == ".EXIT")
                    running = false;
                
                else if(input.find(".LIST_ROOMS") == 0){
                    toggleRoomlist(chatboxWin, showRoomlist);
                }

                // Send message to server
                else if (send(clientSocket, input.c_str(), input.size(), 0) < 0)
                {
                    std::cerr << "Send failed\n";
                    running = false;
                }

                input.clear();

            }
            else if ((ch == KEY_BACKSPACE || ch == 127 || ch == 8) && onChat)
            {
                if (!input.empty())
                    input.pop_back();
            }

            else if (isprint(ch) && onChat)
            {
                input.push_back((char)ch);
            }
            else // handle scrolling
            {
                if (ch == KEY_RIGHT && showRoomlist)
                    onChat = false;
                else if (ch == KEY_LEFT)
                    onChat = true;
                // if focus on chat window
                if (onChat)
                    scrollChat(inputWin, chatboxDisp, onChat, ch);
                else
                    scrollRoom(roomlistWin, roomlistDisp, onChat, clientSocket, ch);

                curs_set(1); // turn on cursor
            }
        }
        napms(20); // lil delay
    }

    delwin(chatboxWin);
    delwin(roomlistWin);
    delwin(inputWin);
    endwin();
    
    
    closesocket(clientSocket);
    
    #ifdef _WIN32
        WSACleanup();
    #endif
    
    thrd.join(); //wait for thread to finish running then program can end
    return 0;
}