/*******************
 * Author: Ryan Gatsinzi Udatinya
 * Filename: chatroom.cpp
 * last_updated: 28_04_2026
 * Desc: This is the second version of a chatroom program that displays a chat box, an input box and a menu on the side
 * It also allows for multiple clients to connect to and a server, send and recieve messages in real time.
 *
 *******************/

#include "curses.h"
#include <vector>
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
std::atomic<bool> running = true; // atomic - preven race condition

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
            mvwprintw(win, i + 1, 1, "%s", std::string("You: " + disp.getData()[i + scrollPos]).c_str());
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

void scrollRoom(WINDOW *roomlistWin, Display &roomlistDisp, bool &onChat, int ch)
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
}

void recieve(int clientSocket, Display &chatboxDisp, WINDOW *win)
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
                chatboxDisp.getData().push_back(std::string(buffer, bytesReceived));

                if (chatboxDisp.getCount() > chatboxDisp.getHeight() - 2)
                    chatboxDisp.setScroll(chatboxDisp.getCount() - (chatboxDisp.getHeight() - 2));
            }
        }
        else if (bytesReceived == 0)
        {
            std::cout << "Server disconnected\n";
            exit(0);
        }
        else
        {
            std::cerr << "Receive failed\n";
            exit(0);
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

int main(int argc, char *argv[])
{
    std::string ip = (argc > 1 ? argv[1] : "127.0.0.1");
    int port = (argc > 2 ? std::stoi(argv[2]) : 54000);
    int clientSocket = connectClient(ip, port);

    WINDOW *chatboxWin, *roomlistWin, *inputWin;
    std::string input;

    // Buffers
    std::vector<std::string> lines1 = {"Welcome to the chat!", "This is a placeholder message.", "More messages will appear here later", ""};
    std::vector<std::string> lines2 = {"General", "Sport", "Programming", "Music", "Random"};

    // Init
    initscr();
    cbreak();
    noecho();
    start_color();

    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_CYAN, COLOR_BLACK);

    Display chatboxDisp(1, LINES * 80 / 100, COLS * 80 / 100, lines1);

    Display roomlistDisp(2, LINES * 80 / 100, COLS - (COLS * 80 / 100), lines2);

    Display inputboxDisp(3, LINES - (LINES * 80 / 100), COLS);

    chatboxWin = newwin(chatboxDisp.getHeight(), chatboxDisp.getWidth(), 0, 0);

    roomlistWin = newwin(roomlistDisp.getHeight(), roomlistDisp.getWidth(), 0, chatboxDisp.getWidth());

    inputWin = newwin(inputboxDisp.getHeight(), inputboxDisp.getWidth(), chatboxDisp.getHeight(), 0);

    scrollok(chatboxWin, TRUE);
    keypad(inputWin, TRUE);
    nodelay(inputWin, TRUE);
    keypad(roomlistWin, TRUE);

    bool onChat = true;

    // run thread that will 
    std::thread thrd(recieve, clientSocket, std::ref(chatboxDisp), std::ref(chatboxWin));

    while (running)
    {
        // Draw outputs
        draw_output(chatboxWin, chatboxDisp);
        draw_output(roomlistWin, roomlistDisp);

        // Draw input
        werase(inputWin);
        box(inputWin, 0, 0);
        // mvwprintw(inputWin, 1, 2, "Enter text for Window %d (type 'exit' to quit): ", toWin1 ? 1 : 2);
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
            if (ch == '\n' || ch == 10 || ch == KEY_ENTER)
            {
                if (input == "exit")
                    running = false;

                // Send message to server
                else if (send(clientSocket, input.c_str(), input.size(), 0) < 0)
                {
                    std::cerr << "Send failed\n";
                    running = false;
                }

                input.clear();
            }

            else if (ch == KEY_BACKSPACE || ch == 127 || ch == 8)
            {
                if (!input.empty())
                    input.pop_back();
            }

            else if (isprint(ch))
            {
                input.push_back((char)ch);
            }
            else // handle scrolling
            {
                if (ch == KEY_RIGHT)
                    onChat = false;
                else if (ch == KEY_LEFT)
                    onChat = true;
                // if focus on chat window
                if (onChat)
                    scrollChat(inputWin, chatboxDisp, onChat, ch);
                else
                    scrollRoom(roomlistWin, roomlistDisp, onChat, ch);

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